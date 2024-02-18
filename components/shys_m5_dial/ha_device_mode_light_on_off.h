#pragma once

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModeLightOnOff: public esphome::shys_m5_dial::HaDeviceMode {
            protected:
                void sendValueToHomeAssistant(int value) override {
                    if(getValue()<=0){
                        haApi.turnLightOff(this->device.getEntityId());
                    } else {
                        haApi.turnLightOn(this->device.getEntityId());
                    }
                }

                void showOnOffMenu(LovyanGFX* gfx){
                    uint16_t currentValue = getValue();

                    uint16_t height = gfx->height();
                    uint16_t width  = gfx->width();

                    gfx->setTextColor(MAROON);
                    gfx->setTextDatum(middle_center);
                    gfx->setFont(&fonts::FreeMono12pt7b);

                    gfx->startWrite();                      // Secure SPI bus

                    gfx->fillRect(0, 0, width, height, currentValue>0?YELLOW:RED);

                    gfx->setTextSize(3);
                    gfx->drawString(currentValue>0?"on":"off",
                                    width / 2,
                                    height / 2 - 30);                        
                    
                    gfx->setTextSize(1);
                    gfx->drawString(this->device.getName().c_str(),
                                    width / 2,
                                    height / 2 + 20);
                    gfx->drawString("On/Off",
                                    width / 2,
                                    height / 2 + 50);  

                    gfx->endWrite();                      // Release SPI bus
                }

            public:
                HaDeviceModeLightOnOff(HaDevice& device) : HaDeviceMode(device){}

                void refreshDisplay(M5DialDisplay& display, bool init) override {
                    this->showOnOffMenu(display.getGfx());
                    ESP_LOGD("DISPLAY", "An/Aus-Modus");
                }

                void registerHAListener() override {
                    std::string attrName = "";
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                attrName, 
                                [this](const std::string &state) {

                        int newState = strcmp("on", state.c_str())==0?1:0;

                        this->setReceivedValue(newState);
                        ESP_LOGI("HA_API", "Got value %s for %s", state.c_str(), this->device.getEntityId().c_str());
                    });
                }

                bool onTouch(M5DialDisplay& display, uint16_t x, uint16_t y) override {
                    haApi.toggleLight(this->device.getEntityId());
                    return true;
                }

                bool onRotary(M5DialDisplay& display, const char * direction) override {
                    if(strcmp(direction, ROTARY_LEFT)==0){
                        haApi.turnLightOff(this->device.getEntityId());
                    } else if(strcmp(direction, ROTARY_RIGHT)==0){
                        haApi.turnLightOn(this->device.getEntityId());
                    }

                    return true;
                }

                bool onButton(M5DialDisplay& display, const char * clickType) override {
                    if (strcmp(clickType, BUTTON_SHORT)==0){
                        haApi.toggleLight(this->device.getEntityId());
                        return true;
                    } 
                    return false;
                }

        };
    }
}