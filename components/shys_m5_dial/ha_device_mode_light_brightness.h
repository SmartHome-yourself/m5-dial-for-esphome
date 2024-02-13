#pragma once

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModeLightBrightness: public esphome::shys_m5_dial::HaDeviceMode {
            protected:
                void sendValueToHomeAssistant(HaDevice& device, int value) override {
                    haApi.turnLightOn(device.getEntityId(), value);
                }

                void showBrightnessMenu(LovyanGFX* gfx, uint16_t currentValue, HaDevice& currentDevice){
                    uint16_t height = gfx->height();
                    uint16_t width  = gfx->width();

                    gfx->setTextColor(MAROON);
                    gfx->setTextDatum(middle_center);
                    gfx->setFont(&fonts::FreeMono12pt7b);

                    gfx->startWrite();                      // Secure SPI bus

                    gfx->fillRect(0, 0, width, height-(height*currentValue/100), RED);
                    gfx->fillRect(0, height-(height*currentValue/100), width, height, YELLOW);

                    gfx->setTextSize(3);
                    gfx->drawString((String(currentValue) + "%").c_str(),
                                    width / 2,
                                    height / 2 - 30);
                    
                    gfx->setTextSize(1);
                    gfx->drawString(currentDevice.getName().c_str(),
                                    width / 2,
                                    height / 2 + 20);
                    gfx->drawString("Brightness",
                                    width / 2,
                                    height / 2 + 50);  

                    gfx->endWrite();                      // Release SPI bus
                }

            public:
                void refreshDisplay(M5DialDisplay& display, HaDevice& currentDevice, bool init) override {
                    ESP_LOGD("DISPLAY", "refresh Display: Helligkeits-Modus");
                    this->showBrightnessMenu(display.getGfx(), getValue(), currentDevice);
                }

                bool onTouch(M5DialDisplay& display, HaDevice& currentDevice, uint16_t x, uint16_t y) override {
                    if(y > display.getHeight() * .97){
                        y = display.getHeight();
                    } else if (y < display.getHeight() * .03){
                        y = 0;
                    }
                    uint16_t result = 100 - ((y * (100/this->rotaryStepWidth)) / display.getHeight()) * this->rotaryStepWidth;
                    this->setValue(result);
                    ESP_LOGD("TOUCH", "Aktuellen Wert auf %i gesetzt", result);
                    
                    return true;                    
                }

                bool onRotary(M5DialDisplay& display, HaDevice& currentDevice, const char * direction) override {
                    if (strcmp(direction, ROTARY_LEFT)==0){
                        this->reduceCurrentValue();
                    } else if (strcmp(direction, ROTARY_RIGHT)==0){
                        this->raiseCurrentValue();
                    }

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