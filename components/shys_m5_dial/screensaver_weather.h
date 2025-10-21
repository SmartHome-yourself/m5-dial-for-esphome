#pragma once
#include "screensaver.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class ScreensaverWeather: public esphome::shys_m5_dial::Screensaver {
            protected:
                void showWeather(M5DialDisplay& display){
                    display.clear(BLACK);

                }

                void refreshWeather(M5DialDisplay& display){
                    int center_x = display.getWidth()/2;
                    int center_y = display.getHeight()/2;
                }


            public:
                void show(M5DialDisplay& display, bool init) override {
                    if(init){
                        showWeather(display);
                    } else {
                        refreshWeather(display);
                    }
                }

        };

    }
}