#pragma once
#include <sstream>
#include <iostream>
#include <ctime>
#include <iomanip>

#include "ha_device_mode_percentage.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModeTimerHandling: public esphome::shys_m5_dial::HaDeviceModePercentage {
            protected:
                esphome::time::RealTimeClock* localTime;

                int updateTimerDelay = 3000;

                std::string duration = "";
                std::string timerState = "";
                std::string remaining = "";
                std::string finishesAt = "";

                int timeToChange = 0;

                unsigned long lastRefresh = 0;

                void sendValueToHomeAssistant(int value) override {
                }

                // wandelt 00:01:30 in 90 um (HA-Timer Zeitangabe in Sekunden)
                float durationToSeconds(const std::string &duration_string) {
                    std::istringstream iss(duration_string);
                    int hours, minutes, seconds;
                    char colon;
                    iss >> hours >> colon >> minutes >> colon >> seconds;
                    return hours * 3600 + minutes * 60 + seconds;
                }


                bool isTimerActive(){
                    const char* curr_state = this->timerState.c_str();
                    if(strcmp(curr_state, "active")==0){
                        return true;
                    }

                    return false;
                }

                bool isTimerPaused(){
                    const char* curr_state = this->timerState.c_str();
                    if(strcmp(curr_state, "paused")==0){
                        return true;
                    }

                    return false;
                }
                
                void raiseTimeToChange(){
                    int newValue = this->timeToChange + this->rotaryStepWidth;
                    newValue = this->getNextToRotaryStepwidth(newValue);
                    this->timeToChange = newValue;
                }

                void reduceTimeToChange(){
                    int newValue = this->timeToChange - this->rotaryStepWidth;
                    newValue = this->getNextToRotaryStepwidth(newValue);

                    if(newValue < 0){
                        newValue = 0;
                    }

                    this->timeToChange = newValue;
                }

                std::time_t parseISO8601(const std::string& iso_string) {
                    int year, month, day, hour, minute, second;
                    sscanf(iso_string.c_str(), "%d-%d-%dT%d:%d:%d", &year, &month, &day, &hour, &minute, &second);

                    struct tm timeinfo;
                    timeinfo.tm_year = year - 1900; // Jahr seit 1900
                    timeinfo.tm_mon = month - 1;    // Monat (0-11)
                    timeinfo.tm_mday = day;         // Tag (1-31)
                    timeinfo.tm_hour = hour;        // Stunde (0-23)
                    timeinfo.tm_min = minute;       // Minute (0-59)
                    timeinfo.tm_sec = second;       // Sekunde (0-59)

                    std::time_t time = std::mktime(&timeinfo);

                    return time;                
                }

                std::string getRemainingDuration(const std::string& iso_string) {
                    if(iso_string.empty()){
                        return "0:00:00";
                    }

                    std::time_t future_time = parseISO8601(iso_string) + this->localTime->now().timezone_offset();
                    std::time_t current_time = localTime->now().timestamp;

                    char c[20];
                    char f[20];
                    strftime(c, sizeof(c), "%Y-%m-%d %H:%M:%S", localtime(&current_time));
                    strftime(f, sizeof(f), "%Y-%m-%d %H:%M:%S", localtime(&future_time));

                    ESP_LOGD("TIMER", "Now: %s / Finish Time: %s", c, f);

                    if (future_time <= current_time) {
                        return "0:00:00"; 
                    }

                    int remaining_time = static_cast<int>(difftime(future_time, current_time));

                    return getTimeString(remaining_time);
                }

                std::string getTimeString(int timeInSeconds){
                    bool negative = timeInSeconds < 0;
                    if(negative){
                        timeInSeconds = timeInSeconds*(-1);
                    }

                    int hours = timeInSeconds / 3600;
                    int minutes = (timeInSeconds % 3600) / 60;
                    int seconds = timeInSeconds % 60;

                    std::ostringstream oss;
                    const char* prefix = negative ? "-":"";
                    oss << prefix << hours << ":"
                        << std::setw(2) << std::setfill('0') << minutes << ":"
                        << std::setw(2) << std::setfill('0') << seconds;

                    return oss.str();
                }
                
            public:
                HaDeviceModeTimerHandling(HaDevice& device) : HaDeviceModePercentage(device){
                    this->setLabel("Timer");
                    this->setUnit("");
                    this->setCustomValue("");
                    this->setIcon(STOPWATCH_IMG, 4900);
                    this->useCustomValue(true);
                    this->setMinMaxLimitActive(false);
                }

                void registerHAListener() {
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                optional<std::string>(), 
                                std::function<void(const std::string&)>([this](const std::string &state) {
                        if(this->isValueModified()){
                            return;
                        }

                        this->timerState = state;

                        if(this->isTimerPaused()){
                            this->activateBar(true);
                        } else if(this->isTimerActive()){
                            this->activateBar(true);
                        } else {
                            this->activateBar(false);
                        }

                        ESP_LOGI("HA_API", "Got state %s for %s", state.c_str(), this->device.getEntityId().c_str());

                        this->displayRefreshNeeded = true;
                    }));

                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                optional<std::string>("duration"), 
                                std::function<void(const std::string&)>([this](const std::string &state) {
                        if(this->isValueModified()){
                            return;
                        }

                        this->duration = state;
                        ESP_LOGI("HA_API", "Got duration %s for %s", state.c_str(), this->device.getEntityId().c_str());

                        this->displayRefreshNeeded = true;
                    }));

                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                optional<std::string>("remaining"), 
                                std::function<void(const std::string&)>([this](const std::string &state) {
                        if(this->isValueModified()){
                            return;
                        }

                        this->remaining = state;
                        ESP_LOGI("HA_API", "Got remaining %s for %s", state.c_str(), this->device.getEntityId().c_str());

                        this->displayRefreshNeeded = true;
                    }));

                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                optional<std::string>("finishes_at"), 
                                std::function<void(const std::string&)>([this](const std::string &state) {
                        if(this->isValueModified()){
                            return;
                        }

                        this->finishesAt = state;
                        ESP_LOGI("HA_API", "Got finish-time %s for %s", state.c_str(), this->device.getEntityId().c_str());

                        this->displayRefreshNeeded = true;
                    }));
                };


                void refreshDisplay(M5DialDisplay& display, bool init) override {
                    ESP_LOGD("DISPLAY", "refresh Display: Timer-Modus");
                    LovyanGFX* gfx = display.getGfx();
                    uint16_t height = gfx->height();
                    uint16_t width  = gfx->width();

                    this->showPercentageMenu(display);

                    std::string tmp = "";
                    if(this->isTimerPaused()){
                        tmp = this->remaining;
                    } else if(this->isTimerActive()){
                        tmp = this->getRemainingDuration(this->finishesAt);
                    } else {
                        tmp = this->duration;
                    }

                    display.setFontsize(1.5);
                    gfx->drawString(tmp.c_str(),
                                    width / 2,
                                    height / 2 - 65);


                    if(this->timeToChange != 0){
                        display.setFontsize(1.2);
                        gfx->drawString(this->getTimeString(this->timeToChange).c_str(),
                                        width / 2,
                                        height / 2 + 60);
                    }


                    displayRefreshNeeded = false;
                }

                void setLocalTime(esphome::time::RealTimeClock* time_comp) {
                    this->localTime = time_comp;
                }


                bool onRotary(M5DialDisplay& display, const char * direction) override {
                    if(!this->isTimerActive()){
                        if(strcmp(direction, ROTARY_LEFT)==0){
                            this->reduceTimeToChange();
                        } else if(strcmp(direction, ROTARY_RIGHT)==0){
                            this->raiseTimeToChange();
                        }
                    }

                    displayRefreshNeeded = true;
                    return true;
                }

                bool onButton(M5DialDisplay& display, const char * clickType) override {
                    if (strcmp(clickType, BUTTON_SHORT)==0){
                        if(this->isTimerActive()){
                            haApi.timerPause(this->device.getEntityId());
                            return true;
                        } else {
                            if(this->timeToChange > 0){
                                haApi.timerStart(this->device.getEntityId(), this->timeToChange);
                                this->timeToChange = 0;
                                return true;
                            } else {
                                haApi.timerStart(this->device.getEntityId());
                                return true;
                            }
                        }
                    } 
                    return false;
                }


                void onLoop() override {
                    if(strcmp(this->timerState.c_str(), "active")==0){
                        bool timebasedRefresh = (this->lastRefresh + 1000) < esphome::millis();
                        if(timebasedRefresh){
                            this->lastRefresh = esphome::millis();

                            this->setReceivedValue(100 * durationToSeconds(this->getRemainingDuration(this->finishesAt)) / durationToSeconds(this->duration) );

                            displayRefreshNeeded = true;
                        }
                    }
                }

        };
    }
}
