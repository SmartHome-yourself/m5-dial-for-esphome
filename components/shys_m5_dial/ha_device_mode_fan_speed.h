#pragma once
#include "globals.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModeFanSpeed: public esphome::shys_m5_dial::HaDeviceMode {
            protected:
                bool changeableDirection = false;
                const char* direction = FAN_DIRECTION_FORWARD;

                bool stateIsOn = false;

                void changeDirection(){
                    if(changeableDirection){
                        if (strcmp(direction, FAN_DIRECTION_FORWARD) == 0){
                            this->publishDirection(FAN_DIRECTION_REVERSE);
                        } else {
                            this->publishDirection(FAN_DIRECTION_FORWARD);
                        }
                    }
                    
                }

                void sendValueToHomeAssistant(int value) override {
                    haApi.setFanSpeed(this->device.getEntityId(), value);
                }

                void showFanMenu(M5DialDisplay& display, uint16_t currentValue){
                    LovyanGFX* gfx = display.getGfx();
                    uint16_t height = gfx->height();
                    uint16_t width  = gfx->width();

                    gfx->setTextColor(MAROON);
                    gfx->setTextDatum(middle_center);

                    gfx->startWrite();                      // Secure SPI bus

                    gfx->fillRect(0, 0, width, this->getDisplayPositionY(currentValue), RED);
                    gfx->fillRect(0, this->getDisplayPositionY(currentValue), width, height, YELLOW);

                    display.setFontsize(3);
                    gfx->drawString((String(currentValue) + "%").c_str(),
                                    width / 2,
                                    height / 2 - 30);
                    
                    display.setFontsize(1);
                    gfx->drawString(this->device.getName().c_str(),
                                    width / 2,
                                    height / 2 + 20);
                    gfx->drawString(this->changeableDirection?direction:"Speed",
                                    width / 2,
                                    height / 2 + 50);  

                    gfx->endWrite();                      // Release SPI bus
                }

            public:
                HaDeviceModeFanSpeed(HaDevice& device) : HaDeviceMode(device){}

                void setState(const std::string& newState){
                    this->stateIsOn = (strcmp(newState.c_str(), "on") == 0);
                }

                int getValue() override {
                    return this->stateIsOn ? this->value : 0;
                }
                

                void setChangeableDirection(bool isChangeable){
                    this->changeableDirection = isChangeable;
                }

                void publishDirection(const std::string& newDirection){
                    setDirection(newDirection);
                    haApi.setFanDirection(this->device.getEntityId(), this->direction);
                }

                void setDirection(const std::string& newDirection){
                    if (strcmp(newDirection.c_str(), FAN_DIRECTION_FORWARD) == 0){
                        this->direction = FAN_DIRECTION_FORWARD;
                    } else if (strcmp(newDirection.c_str(), FAN_DIRECTION_REVERSE) == 0){
                        this->direction = FAN_DIRECTION_REVERSE;
                    }

                    displayRefreshNeeded = true;
                    
                    ESP_LOGD("DEVICE", "set direction: %s", this->direction);
                }

                void refreshDisplay(M5DialDisplay& display, bool init) override {
                    ESP_LOGD("DISPLAY", "refresh Display: Speed-Modus");
                    this->showFanMenu(display, getValue());
                    
                    this->displayRefreshNeeded = false;
                }

                void registerHAListener() override {
                    std::string attrNameState = "";
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                attrNameState, 
                                [this](const std::string &state) {
                        if(this->isValueModified()){
                            return;
                        }

                        this->setState(state);
                        ESP_LOGI("HA_API", "Got State %s for %s", state.c_str(), this->device.getEntityId().c_str());
                    });

                    std::string attrNamePercentage = "percentage";
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                attrNamePercentage, 
                                [this](const std::string &state) {
                        if(this->isValueModified()){
                            return;
                        }
                        auto val = parse_number<int>(state);
                        if (!val.has_value()) {
                            this->setReceivedValue(0);
                            ESP_LOGD("HA_API", "No Percentage value in %s for %s", state.c_str(), this->device.getEntityId().c_str());
                        } else {
                            this->setReceivedValue(round((int)val.value()));
                            ESP_LOGI("HA_API", "Got Percentage value %i for %s", val.value(), this->device.getEntityId().c_str());
                        }
                    });

                    if(this->changeableDirection){
                        std::string attrNameDirection = "direction";
                        api::global_api_server->subscribe_home_assistant_state(
                                    this->device.getEntityId().c_str(),
                                    attrNameDirection, 
                                    [this](const std::string &state) {
                            if(this->isValueModified()){
                                return;
                            }

                            setDirection(state.c_str());
                            ESP_LOGI("HA_API", "Got direction value %s for %s", state.c_str(), this->device.getEntityId().c_str());
                    });
                    }
                }

                bool onTouch(M5DialDisplay& display, uint16_t x, uint16_t y) override {
                    return this->defaultOnTouch(display, x, y);
                }

                bool onRotary(M5DialDisplay& display, const char * direction) override {
                    if(! this->stateIsOn){
                        if (strcmp(direction, ROTARY_LEFT)==0){
                            this->publishDirection(FAN_DIRECTION_REVERSE);
                            this->stateIsOn = true;
                            this->value = 0;
                        } else if (strcmp(direction, ROTARY_RIGHT)==0){
                            this->publishDirection(FAN_DIRECTION_FORWARD);
                            this->stateIsOn = true;
                            this->value = 0;
                        }
                    }

                    if (strcmp(direction, ROTARY_LEFT)==0){
                        if(strcmp(this->direction, FAN_DIRECTION_FORWARD)==0){
                            this->reduceCurrentValue();
                        } else {
                            this->raiseCurrentValue();
                        }
                        
                    } else if (strcmp(direction, ROTARY_RIGHT)==0){
                        if(strcmp(this->direction, FAN_DIRECTION_FORWARD)==0){
                            this->raiseCurrentValue();
                        } else {
                            this->reduceCurrentValue();
                        }
                    }

                    if(this->value == 0){
                        this->stateIsOn = false;
                    }

                    return true;
                }

                bool onButton(M5DialDisplay& display, const char * clickType) override {
                    if (strcmp(clickType, BUTTON_SHORT)==0){
                        haApi.toggleFan(this->device.getEntityId());
                        return true;
                    } else if (strcmp(clickType, BUTTON_LONG)==0){
                        this->changeDirection();
                        return true;
                    } 
                    return false;
                }

        };
    }
}