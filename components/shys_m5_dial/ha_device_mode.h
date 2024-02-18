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
                HaDevice& device;

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
                    int newValue = this->value + rotaryStepWidth;
                    newValue = getNextToRotaryStepwidth(newValue);

                    if(newValue > this->maxValue){
                        setValue(endlessRotaryValue?this->minValue:this->maxValue);
                    } else {
                        setValue(newValue);
                    }
                }

                void reduceCurrentValue(){
                    int newValue = this->value - rotaryStepWidth;
                    newValue = getNextToRotaryStepwidth(newValue);

                    if(newValue >= this->minValue){
                        setValue(newValue);
                    } else {
                        setValue(endlessRotaryValue?this->maxValue:this->minValue);
                    }
                }

                int getNextToRotaryStepwidth(int val){
                    int rst = (val % rotaryStepWidth);
                    if(rst >= (rotaryStepWidth/2)){
                        val += rotaryStepWidth-rst;
                    } else {
                        val -= rst;
                    }
                    return val;
                }

                uint16_t getValueForXPosition(uint16_t x){
                    return map(x, 0, M5Dial.Display.width(), this->minValue, this->maxValue);
                }

                uint16_t getValueForYPosition(uint16_t y){
                    return this->maxValue - map(y, 0, M5Dial.Display.height(), this->minValue, this->maxValue) + this->minValue;
                }

                uint16_t getDisplayPositionX(uint16_t currentValue){
                    return map(currentValue, this->minValue, this->maxValue, 0, M5Dial.Display.width());
                }

                uint16_t getDisplayPositionY(uint16_t currentValue){
                    return map(currentValue, this->minValue, this->maxValue, 0, M5Dial.Display.height());
                }


                virtual void sendValueToHomeAssistant(int value) = 0;

            public:
                HaDeviceMode(HaDevice& device) : device(device) {}

                virtual void refreshDisplay(M5DialDisplay& display, bool init) = 0;
                virtual void registerHAListener() = 0;


                virtual bool onTouch(M5DialDisplay& display, uint16_t x, uint16_t y) {
                    return false;
                }

                virtual bool onSwipe(M5DialDisplay& display, const char * direction) {
                    return false;
                }

                virtual bool onRotary(M5DialDisplay& display, const char * direction) {                    
                    return false;
                }

                virtual bool onButton(M5DialDisplay& display, const char * clickType) {
                    return false;
                }


                void updateHomeAssistantValue(){
                    if(this->isValueModified() && this->isApiCallNeeded() ) {
                        lastApiCall = esphome::millis();

                        this->sendValueToHomeAssistant(getValue());

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

                void setRotaryStepWidth(int stepWidth){
                    this->rotaryStepWidth = stepWidth;
                }

        };
    }
}