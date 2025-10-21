#pragma once

#include "m5dial_display.h"

#include "ha_device_mode.h"
#include <ArduinoJson.h>

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDevice {
            protected:
                int index;
                std::string entity;
                std::string name;

                HaApi haApi;

                static const size_t bufferSize = 1024;
                JsonDocument jsonBuffer;
                JsonObject modeConfig;

                uint16_t currentModeCount = 0;
                uint16_t currentModeIndex = 0;

                int apiSendDelay = 1000;
                int apiSendLock = 2000;

                int rotaryStepWidth = 10;

                bool modeChanged = true;

                bool locked = false;

                std::vector<HaDeviceMode*> deviceModes = {};

                void addMode(HaDeviceMode* newMode){
                    newMode->registerHAListener();

                    newMode->setApiSendDelay(getApiSendDelay());
                    newMode->setApiSendLock(getApiSendLock());
                    newMode->setRotaryStepWidth(getRotaryStepWidth());

                    this->deviceModes.push_back(newMode);
                    currentModeCount++;
                }

                HaDeviceMode* getCurrentMode(){
                    return deviceModes[currentModeIndex];
                }

                void loadModesConfigJson(const std::string& modesJsonString){
                    DeserializationError error = deserializeJson(jsonBuffer, modesJsonString);
                    ESP_LOGW("DEVICE", "Modes JSON: %s", modesJsonString.c_str());

                    if (error) {
                        ESP_LOGW("DEVICE", "Fehler beim Parsen des JSON: %s", error.c_str());
                        return;
                    }
                    this->modeConfig = jsonBuffer.as<JsonObject>();
                }

                virtual bool onTouch(M5DialDisplay& display, uint16_t x, uint16_t y){
                    ESP_LOGD("DEVICE", "HaDevice.onTouch: %i/%i", x, y);
                    return getCurrentMode()->onTouch(display, x, y);
                }

                virtual bool onRotary(M5DialDisplay& display, const char * direction){
                    ESP_LOGD("DEVICE", "HaDevice.onRotary: %s", direction);
                    return getCurrentMode()->onRotary(display, direction);
                }

                virtual bool onButton(M5DialDisplay& display, const char * clickType){
                    ESP_LOGD("DEVICE", "HaDevice.onButton: %s", clickType);
                    return getCurrentMode()->onButton(display, clickType);
                }

                virtual void onLoop(){
                    getCurrentMode()->onLoop();
                }

            public:
                HaDevice(const std::string& entity_id, const std::string& name, const std::string& modesJsonString) : entity(entity_id), name(name) {
                    loadModesConfigJson(modesJsonString);
                }    

                virtual void init() = 0;
                
                std::string getName(){
                    return this->name;
                }

                std::string getEntityId(){
                    return this->entity;
                }

                int getValue(){
                    return getCurrentMode()->getValue();
                }

                uint16_t getMaxRotary(){
                    return getCurrentMode()->getMaxValue();
                }

                void setRotaryStepWidth(int stepWidth){
                    this->rotaryStepWidth = stepWidth;
                }
                int getRotaryStepWidth(){
                    return this->rotaryStepWidth;
                }
                
                void setApiSendDelay(int delayInMs){
                    this->apiSendDelay = delayInMs;
                }
                int getApiSendDelay(){
                    return this->apiSendDelay;
                }
                
                void setApiSendLock(int delayInMs){
                    this->apiSendLock = delayInMs;
                }
                int getApiSendLock(){
                    return this->apiSendLock;
                }

                int getCurrentModeIndex(){
                    return this->currentModeIndex;
                }

                bool isLocked(){
                    return this->locked;
                }

                void setLocked(bool isLocked){
                    this->locked = isLocked;
                }
                
                JsonObject getModeConfig(){
                    return this->modeConfig;
                }

                void nextMode(){
                    if(currentModeIndex >= currentModeCount-1){
                        currentModeIndex = 0;
                    } else {
                        currentModeIndex++;
                    }
                    modeChanged = true;
                }

                void previousMode(){
                    if(currentModeIndex >= 1){
                        currentModeIndex--;
                    } else {
                        currentModeIndex = currentModeCount-1;
                    }
                    modeChanged = true;
                }

                int getModeIndex(HaDeviceMode* mode){
                    for(int i=0; i<currentModeCount; i++){
                        if(this->deviceModes[i] == mode){
                            return i;
                        }
                    }

                    return -1;
                }

                void setCurrentModeIndex(int newMode){
                    currentModeIndex = newMode;
                    modeChanged = true;
                }

                void refreshDisplay(M5DialDisplay& display, bool deviceChanged){
                    if(!display.isScreensaverRunning()){
                        getCurrentMode()->refreshDisplay(display, deviceChanged || modeChanged);
                        modeChanged = false;
                    }
                }

                bool isDisplayRefreshNeeded(){
                    return this->getCurrentMode()->isDisplayRefreshNeeded();
                }

                void updateHomeAssistantValue(){
                    getCurrentMode()->updateHomeAssistantValue();
                }

                float getRadiusFromCoord(M5DialDisplay& display, float touchX, float touchY) {
                    return display.getRadiusFromCoord(touchX, touchY);
                }

                bool doOnSwipe(M5DialDisplay& display, const char * direction){
                    if(this->isLocked()){
                        return false;
                    }

                    return false; // getCurrentMode()->onSwipe(display, *this, direction);
                }

                bool doOnTouch(M5DialDisplay& display, uint16_t x, uint16_t y){
                    if(this->isLocked()){
                        return false;
                    }

                    return this->onTouch(display, x, y);
                }

                bool doOnRotary(M5DialDisplay& display, const char * direction){
                    if(this->isLocked()){
                        return false;
                    }

                    return this->onRotary(display, direction);
                }

                bool doOnButton(M5DialDisplay& display, const char * clickType){
                    if(this->isLocked()){
                        return false;
                    }

                    return this->onButton(display, clickType);
                }

                void doOnLoop(){
                    this->onLoop();
                }

        };

    }
}