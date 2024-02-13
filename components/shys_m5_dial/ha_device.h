#pragma once
#include "ha_device_mode.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceMode;
        class HaDevice {
            protected:
                int index;
                String entity;
                String name;

                uint16_t currentModeCount;
                uint16_t currentModeIndex = 0;
                uint16_t maxRotary = 100;
                int lastValue = 0;

                bool modeChanged = true;

                std::vector<HaDeviceMode*> deviceModes = {};

                void addMode(HaDeviceMode* newMode){
                    this->deviceModes.push_back(newMode);
                    currentModeCount++;
                }

                HaDeviceMode* getCurrentMode(){
                    return deviceModes[currentModeIndex];
                }

                void initDefault(String entity_id, String name){
                    this->entity       = entity_id;
                    this->name         = name;
                    this->lastValue    = 0;
                }


            public:
                virtual void init(String entity_id, String name) = 0;
                
                String getName(){
                    return this->name;
                }

                String getEntityId(){
                    return this->entity;
                }

                int getValue(){
                    return getCurrentMode()->getValue();
                }

                uint16_t getMaxRotary(){
                    return getCurrentMode()->getMaxValue();
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

                void refreshDisplay(M5DialDisplay& display, bool deviceChanged){
                    getCurrentMode()->refreshDisplay(display, *this, deviceChanged || modeChanged);
                    modeChanged = false;
                }

                void updateHomeAssistantValue(){
                    getCurrentMode()->updateHomeAssistantValue(*this);
                }

                bool onTouch(M5DialDisplay& display, uint16_t x, uint16_t y){
                    return getCurrentMode()->onTouch(display, *this, x, y);
                }

                bool onSwipe(M5DialDisplay& display, const char * direction){
                    return getCurrentMode()->onSwipe(display, *this, direction);
                }

                bool onRotary(M5DialDisplay& display, const char * direction){
                    return getCurrentMode()->onRotary(display, *this, direction);
                }

                bool onButton(M5DialDisplay& display, const char * clickType){
                    return getCurrentMode()->onButton(display, *this, clickType);
                }



        };

    }
}