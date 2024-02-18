#pragma once

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModePercentage: public esphome::shys_m5_dial::HaDeviceMode {
            protected:
                void showPercentageMenu(LovyanGFX* gfx, uint16_t currentValue){
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
                    gfx->drawString(this->device.getName().c_str(),
                                    width / 2,
                                    height / 2 + 20);
                    gfx->drawString("Brightness",
                                    width / 2,
                                    height / 2 + 50);  

                    gfx->endWrite();                      // Release SPI bus
                }

            public:
                HaDeviceModePercentage(HaDevice& device) : HaDeviceMode(device){}

                void refreshDisplay(M5DialDisplay& display, bool init) override {
                    ESP_LOGD("DISPLAY", "refresh Display: Percentage-Modus");
                    showPercentageMenu(display.getGfx(), round((float)getValue()));
                }

                void registerHAListener() override {
                }
                

        };
    }
}