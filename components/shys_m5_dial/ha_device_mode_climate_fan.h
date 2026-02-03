#pragma once
#include "ha_device_mode.h"
#include <vector>
#include <sstream>
#include <iostream>

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModeClimateFan: public esphome::shys_m5_dial::HaDeviceMode {
            protected:

                std::vector<std::string> fanModes = {};
                bool manualModes = false;

                void sendValueToHomeAssistant(int value) override {
                    // not choose Playlist on change value.
                    // selection activated by button pressed
                }

                void showModeSelection(M5DialDisplay& display){
                    LovyanGFX* gfx = display.getGfx();
                    uint16_t height = gfx->height();
                    uint16_t width  = gfx->width();

                    gfx->setTextColor(MAROON);
                    gfx->setTextDatum(middle_center);

                    gfx->startWrite();                      // Secure SPI bus

                    display.clear();

                    // Device Name
                    display.setFontsize(1);
                    gfx->drawString(this->device.getName().c_str(),
                                    width / 2,
                                    height / 2 - 80);

                    if(this->getMaxValue()>0){
                        if(this->getValue()>0){
                            display.setFontsize(.7);
                            gfx->drawString(this->fanModes[this->getValue()-1].c_str(),
                                            width / 2,
                                            height / 2-40); 
                        }

                        display.setFontsize(1.5);
                        gfx->drawString(this->fanModes[this->getValue()].c_str(),
                                        width / 2,
                                        height / 2);

                        if(this->getValue()<this->getMaxValue()){
                            display.setFontsize(.7);
                            gfx->drawString(this->fanModes[this->getValue()+1].c_str(),
                                            width / 2,
                                            height / 2+40); 
                        }

                        display.setFontsize(1);
                        gfx->drawString("Fan Mode",
                                        width / 2,
                                        height / 2 +80);

                    }

                    gfx->endWrite();                      // Release SPI bus
                }

            public:
                HaDeviceModeClimateFan(HaDevice& device) : HaDeviceMode(device){
                    this->setMaxValue(0);
                }

                void refreshDisplay(M5DialDisplay& display, bool init) override {
                    this->showModeSelection(display);
                    ESP_LOGD("DISPLAY", "Climate-Fan-Modus");

                    this->displayRefreshNeeded = false;
                }

                void registerHAListener() override {
                    JsonObject modeConfig = this->device.getModeConfig();

                    if (modeConfig["fan_mode"].is<JsonObject>() && !manualModes) {
                        api::global_api_server->subscribe_home_assistant_state(
                                    this->device.getEntityId().c_str(),
                                    optional<std::string>("fan_modes"), 
                                    std::function<void(const std::string&)>([this](const std::string &state) {

                            ESP_LOGI("HA_API", "Got Climate Fan-Mode %s for %s", state.c_str(), this->device.getEntityId().c_str());

                            std::string str = state;
                            str.erase(std::remove(str.begin(), str.end(), '['), str.end());
                            str.erase(std::remove(str.begin(), str.end(), ']'), str.end());
                            str.erase(std::remove(str.begin(), str.end(), '\''), str.end());
                            str.erase(std::remove(str.begin(), str.end(), ' '), str.end());

                            std::stringstream ss(str);
                            std::vector<std::string> result;
                            std::string token;
                            int i = 0;
                            while (std::getline(ss, token, ',')) {
                                result.push_back(token);
                                i++;
                            }

                            this->fanModes = result;
                            this->setMaxValue(i-1);
                        }));
                    }
                }

                bool onRotary(M5DialDisplay& display, const char * direction) override {
                    this->defaultOnRotary(display, direction);
                    return true;
                }

                bool onButton(M5DialDisplay& display, const char * clickType) override {
                    if (strcmp(clickType, BUTTON_SHORT)==0){
                        const std::string fanMode = this->fanModes[this->getValue()].c_str();

                        haApi.setClimateFanMode(this->device.getEntityId(), fanMode);
                        
                        return true;
                    }

                    return false;
                }

               /**
                * 
                */
                void loadFanModes(JsonObject modeConfig){
                    this->fanModes = {};

                    if (modeConfig["fan_mode"].is<JsonObject>()) {
                        JsonObject fanModeConfig = modeConfig["fan_mode"];

                        if (fanModeConfig["modes"].is<JsonArray>()) {
                            JsonArray sources = fanModeConfig["modes"].as<JsonArray>();

                            for(std::string fanMode : sources){
                                this->fanModes.push_back(fanMode);
                                ESP_LOGD("CLIMATE", "Add Climate Fan-Mode: %s on %s", fanMode.c_str(), this->device.getEntityId().c_str());
                            }
                            this->setMaxValue(this->fanModes.size()-1);

                            manualModes = this->fanModes.size() > 0;
                        }
                    }
                }

        };
    }
}
