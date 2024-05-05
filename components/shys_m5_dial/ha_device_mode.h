#pragma once

#include "ha_device.h"
#include "ha_api.h"
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
                bool minMaxLimitActive = true;

                int rotaryStepWidth = 10;

                int apiSendDelay = 1000;
                int apiSendLock = 2000;
            
                bool currentValueModified = false;
                unsigned long lastApiCall = 0;
                unsigned long lastValueUpdate = 0;

                bool endlessRotaryValue = false;

                bool displayRefreshNeeded = false;
                double lastDisplayRefresh = 0;

                HaApi haApi;
                HaDevice& device;


                HaDevice& getDevice() const {
                    return this->device; 
                }

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
                    int newValue = this->getValue() + rotaryStepWidth;
                    newValue = getNextToRotaryStepwidth(newValue);

                    if(newValue > this->maxValue && minMaxLimitActive){
                        setValue(endlessRotaryValue?this->minValue:this->maxValue);
                    } else {
                        setValue(newValue);
                    }
                }

                void reduceCurrentValue(){
                    int newValue = this->getValue() - rotaryStepWidth;
                    newValue = getNextToRotaryStepwidth(newValue);

                    if(newValue >= this->minValue && minMaxLimitActive){
                        setValue(newValue);
                    } else {
                        setValue(endlessRotaryValue?this->maxValue:this->minValue);
                    }
                }

                int getNextToRotaryStepwidth(int val){
                    if(rotaryStepWidth > 1){
                        int rst = (val % rotaryStepWidth);
                        if(rst >= (rotaryStepWidth/2)){
                            val += rotaryStepWidth-rst;
                        } else {
                            val -= rst;
                        }
                    }
                    return val;
                }

                uint16_t getValueForXPosition(uint16_t x){
                    return map(x, 0, M5Dial.Display.width(), this->minValue, this->maxValue);
                }

                uint16_t getValueForYPosition(uint16_t y){
                    return this->maxValue - map(y, 0, M5Dial.Display.height(), this->minValue, this->maxValue);
                }

                uint16_t getValueForAnglePercentageArc(uint16_t angle){
                    if(angle<60){
                        angle = 60;
                    } else if(angle>300){
                        angle = 300;
                    }
                    return this->maxValue - map(angle, 60, 300, 0, this->maxValue-this->minValue);
                }

                uint16_t getDisplayPositionX(uint16_t currentValue){
                    return map(currentValue, this->minValue, this->maxValue, 0, M5Dial.Display.width());
                }

                uint16_t getDisplayPositionY(uint16_t currentValue){
                    return M5Dial.Display.height() - map(currentValue, this->minValue, this->maxValue, 0, M5Dial.Display.height());
                }

                float  getAngleFromCoord(M5DialDisplay& display, uint16_t touchX, uint16_t touchY) {
                    float  dx = touchX - (display.getWidth() / 2.0f);
                    float  dy = (display.getHeight() / 2.0f) - touchY;
                    float  angle = (atan2 (dy,dx) * 180 / M_PI) + 90;

                    if (angle < 0) {
                        angle += 360.0f;
                    }

                    return angle;
                }

                float getRadiusFromCoord(M5DialDisplay& display, float touchX, float touchY) {
                    return display.getRadiusFromCoord(touchX, touchY);
                }

                bool defaultOnTouchY(M5DialDisplay& display, uint16_t x, uint16_t y){
                    if(y > display.getHeight() * .97){
                        y = display.getHeight();
                    } else if (y < display.getHeight() * .03){
                        y = 0;
                    }

                    uint16_t result = this->getValueForYPosition(y);
                    result = getNextToRotaryStepwidth(result);

                    this->setValue(result); 
                    ESP_LOGD("TOUCH", "Aktuellen Wert auf %i gesetzt", result);
                    
                    return true;                    
                }

                bool defaultOnTouch(M5DialDisplay& display, uint16_t x, uint16_t y)  {
                    if(getRadiusFromCoord(display,x,y)<70){
                        return false;
                    }

                    float angle = getAngleFromCoord(display, x, y);

                    uint16_t result = getValueForAnglePercentageArc(angle);
                    result = getNextToRotaryStepwidth(result);

                    this->setValue(result); 
                    ESP_LOGD("TOUCH", "Aktuellen Wert auf %i gesetzt", result);
                    
                    return true;                    
                }

                bool defaultOnRotary(M5DialDisplay& display, const char * direction)  {
                    if (strcmp(direction, ROTARY_LEFT)==0){
                        this->reduceCurrentValue();
                    } else if (strcmp(direction, ROTARY_RIGHT)==0){
                        this->raiseCurrentValue();
                    }

                    return true;
                }

                virtual void sendValueToHomeAssistant(int value) = 0;

            public:
                HaDeviceMode(HaDevice& device) : device(device) {}

                virtual void refreshDisplay(M5DialDisplay& display, bool init) = 0;
                virtual void registerHAListener() = 0;


                virtual bool isDisplayRefreshNeeded(){
                    return displayRefreshNeeded;
                }

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

                virtual void onLoop(){}


                void updateHomeAssistantValue(){
                    if(this->isValueModified() && this->isApiCallNeeded() ) {
                        lastApiCall = esphome::millis();

                        this->sendValueToHomeAssistant(getValue());

                        currentValueModified = false;
                    }
                }

                virtual int getValue(){
                    if(this->value > this->getMaxValue() && minMaxLimitActive){
                        return this->getMaxValue();
                    }
                    if(this->value < this->getMinValue() && minMaxLimitActive){
                        return this->getMinValue();
                    }

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

                void setMinValue(int val){
                    this->minValue = val;
                }

                int getMaxValue(){
                    return this->maxValue;
                }

                void setMaxValue(int val){
                    this->maxValue = val;
                }

                bool isValueModified(){
                    return this->currentValueModified;
                }

                void setRotaryStepWidth(int stepWidth){
                    this->rotaryStepWidth = stepWidth;
                }

                void setApiSendDelay(int delayInMs){
                    this->apiSendDelay = delayInMs;
                }
                
                void setApiSendLock(int delayInMs){
                    this->apiSendLock = delayInMs;
                }

                void setMinMaxLimitActive(bool limitActive){
                    this->minMaxLimitActive = limitActive;
                }
        };
    }
}