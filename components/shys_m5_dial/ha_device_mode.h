#pragma once
#include "ha_device.h"
#include "m5dial_display.h"
#include "M5Dial.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDevice;
        class HaDeviceMode {
            protected:
                int value = 0;
                int minValue = 0;
                int maxValue = 100;

                int rotaryStepWidth = 10;

                int apiSendDelay = 1000; // Verzögerung nach Wert-Änderung (um nicht jeden Wert beim drehen des Rades zu senden)
                int apiSendLock = 2000; // Wartezeit zwischen einzelnden API-Aufrufen
            
                bool currentValueModified = false;
                unsigned long lastApiCall = 0;
                unsigned long lastValueUpdate = 0;

                bool endlessRotaryValue = false;

                HaApi haApi;

                bool isApiCallNeeded(){
                    if ( esphome::millis() - lastApiCall < apiSendLock ) {
                        return false;
                    }

                    if ( esphome::millis() - lastValueUpdate < apiSendDelay ) {
                        return false;
                    }

                    return true;
                }

                void raiseCurrentValue(){
                    if(this->value <= this->maxValue - rotaryStepWidth){
                        setValue(this->value + rotaryStepWidth);
                    } else {
                        setValue(endlessRotaryValue?this->minValue:this->maxValue);
                    }
                }

                void reduceCurrentValue(){
                    if(this->value >= this->minValue + rotaryStepWidth){
                        setValue(this->value - rotaryStepWidth);
                    } else {
                        setValue(endlessRotaryValue?this->maxValue:this->minValue);
                    }
                }

                virtual void sendValueToHomeAssistant(HaDevice& device, int value) = 0;

            public:
                virtual void refreshDisplay(M5DialDisplay& display, HaDevice& currentDevice, bool init) = 0;


                virtual bool onTouch(M5DialDisplay& display, HaDevice& currentDevice, uint16_t x, uint16_t y) {
                    return false;
                }

                virtual bool onSwipe(M5DialDisplay& display, HaDevice& currentDevice, const char * direction) {
                    return false;
                }

                virtual bool onRotary(M5DialDisplay& display, HaDevice& currentDevice, const char * direction) {                    
                    return false;
                }

                virtual bool onButton(M5DialDisplay& display, HaDevice& currentDevice, const char * clickType) {
                    return false;
                }


                void updateHomeAssistantValue(HaDevice& device){
                    if(this->isValueModified() && this->isApiCallNeeded() ) {
                        lastApiCall = esphome::millis();

                        this->sendValueToHomeAssistant(device, getValue());

                        currentValueModified = false;
                    }
                }

                int getValue(){
                    return this->value;
                }

                void setValue(int val){
                    this->value = val;
                    this->lastValueUpdate = esphome::millis();
                    this->currentValueModified = true;
                }

                void setReceivedValue(int val){
                    this->value = val;
                }


                int getMinValue(){
                    return this->minValue;
                }

                int getMaxValue(){
                    return this->maxValue;
                }

                bool isValueModified(){
                    return this->currentValueModified;
                }


        };
    }
}