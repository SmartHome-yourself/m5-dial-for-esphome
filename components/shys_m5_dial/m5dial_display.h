#pragma once
#include "M5Dial.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class M5DialDisplay {
            protected:

            public:
                void refreshDisplay(uint16_t currentValue, HaDevice currentDevice){
                    LovyanGFX* gfx = &M5Dial.Display;

                    uint16_t height = gfx->height();
                    uint16_t width  = gfx->width();

                    gfx->clear();

                    gfx->fillRect(0, 0, width, height, RED);
                    gfx->fillRect(0, height-(height*currentValue/100), width, height, YELLOW);
                    gfx->setTextColor(MAROON);
                    gfx->setTextDatum(middle_center);
                    gfx->setFont(&fonts::FreeMono12pt7b);

                    gfx->setTextSize(3);

                    gfx->drawString(String(currentValue),
                                    width / 2,
                                    height / 2 - 30);

                    gfx->setTextSize(1);

                    gfx->drawString(currentDevice.getName().c_str(),
                                    width / 2,
                                    height / 2 + 20);
                    gfx->drawString(currentDevice.getDeviceType().c_str(),
                                    width / 2,
                                    height / 2 + 50);  
                }

        };
    }
}