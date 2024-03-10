#pragma once

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModeClimateTemperature: public esphome::shys_m5_dial::HaDeviceMode {
            protected:
                std::string hvac_mode = "none";

                std::string getHvacMode(){
                    return this->hvac_mode;
                }

                void setHvacMode(const std::string& newMode){
                    this->hvac_mode = newMode;
                }

                void sendValueToHomeAssistant(int value) override {
                    haApi.setClimateTemperature(this->device.getEntityId(), value);
                }

                void showTemperatureMenu(M5DialDisplay& display){
                    LovyanGFX* gfx = display.getGfx();

                    uint16_t currentValue = getValue();

                    uint16_t height = gfx->height();
                    uint16_t width  = gfx->width();

                    gfx->setTextColor(MAROON);
                    gfx->setTextDatum(middle_center);
                    
                    gfx->startWrite();                      // Secure SPI bus

                    gfx->fillRect(0, 0, width, this->getDisplayPositionY(currentValue) , RED);
                    gfx->fillRect(0, this->getDisplayPositionY(currentValue), width, height, YELLOW);

                    display.setFontsize(3);
                    gfx->drawString(String(currentValue).c_str(),
                                    width / 2,
                                    height / 2 - 30);                        
                    
                    display.setFontsize(1);
                    gfx->drawString(this->device.getName().c_str(),
                                    width / 2,
                                    height / 2 + 20);
                    gfx->drawString("Temperature",
                                    width / 2,
                                    height / 2 + 50);  

                    gfx->endWrite();                      // Release SPI bus
                }

            public:
                HaDeviceModeClimateTemperature(HaDevice& device) : HaDeviceMode(device){
                    this->maxValue = 40;
                }

                void refreshDisplay(M5DialDisplay& display, bool init) override {
                    this->showTemperatureMenu(display);
                    ESP_LOGD("DISPLAY", "Temperature-Modus");
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

                        this->setHvacMode(state.c_str());
                        ESP_LOGI("HA_API", "Got Mode %s for %s", state.c_str(), this->device.getEntityId().c_str());
                    });

                    std::string attrNameTemp = "temperature";
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                attrNameTemp, 
                                [this](const std::string &state) {

                        if(this->isValueModified()){
                            return;
                        }

                        auto val = parse_number<float>(state);

                        if (!val.has_value()) {
                            this->setReceivedValue(0);
                            ESP_LOGD("HA_API", "No Temperature value in %s for %s", state.c_str(), this->device.getEntityId().c_str());
                        } else {
                            this->setReceivedValue(int(val.value()));
                            ESP_LOGI("HA_API", "Got Temperature value %i for %s", int(val.value()), this->device.getEntityId().c_str());
                        }
                    });
                }


                bool onTouch(M5DialDisplay& display, uint16_t x, uint16_t y) override {
                    return defaultOnTouch(display, x, y);        
                }

                bool onRotary(M5DialDisplay& display, const char * direction) override {
                    return defaultOnRotary(display, direction);
                }

                bool onButton(M5DialDisplay& display, const char * clickType) override {
                    if (strcmp(clickType, BUTTON_SHORT)==0){
                        if(strcmp(this->getHvacMode().c_str(), "off")==0){
                            haApi.turnClimateOn(this->device.getEntityId());
                        } else {
                            haApi.turnClimateOff(this->device.getEntityId());
                        }
                        
                        return true;
                    } 
                    return false;
                }

        };
    }
}