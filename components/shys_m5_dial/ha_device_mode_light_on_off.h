#pragma once

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModeLightOnOff: public esphome::shys_m5_dial::HaDeviceMode {
            protected:
                void sendValueToHomeAssistant(HaDevice& device, int value) override {
                    if(getValue()<=0){
                        haApi.turnLightOff(device.getEntityId());
                    } else {
                        haApi.turnLightOn(device.getEntityId());
                    }
                }

                void showBrightnessMenu(LovyanGFX* gfx, HaDevice& currentDevice){
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
                    gfx->drawString(currentDevice.getName().c_str(),
                                    width / 2,
                                    height / 2 + 20);
                    gfx->drawString("On/Off",
                                    width / 2,
                                    height / 2 + 50);  

                    gfx->endWrite();                      // Release SPI bus
                }

            public:
                void refreshDisplay(M5DialDisplay& display, HaDevice& currentDevice, bool init) override {
                    this->showBrightnessMenu(display.getGfx(), currentDevice);
                    ESP_LOGD("DISPLAY", "An/Aus-Modus");
                }

                bool onTouch(M5DialDisplay& display, HaDevice& currentDevice, uint16_t x, uint16_t y) override {
                    haApi.toggleLight(currentDevice.getEntityId());
                    return true;
                }

                bool onRotary(M5DialDisplay& display, HaDevice& currentDevice, const char * direction) override {                    
                    haApi.toggleLight(currentDevice.getEntityId());
                    return true;
                }

                bool onButton(M5DialDisplay& display, HaDevice& currentDevice, const char * clickType) override {
                    if (strcmp(clickType, BUTTON_SHORT)==0){
                        haApi.toggleLight(currentDevice.getEntityId());
                        return true;
                    } 
                    return false;
                }

        };
    }
}