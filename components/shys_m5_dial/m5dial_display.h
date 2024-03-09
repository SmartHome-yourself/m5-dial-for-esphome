#pragma once
#include "M5Dial.h"
#include "esphome.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class M5DialDisplay {
            protected:
                LovyanGFX* gfx = &M5Dial.Display;

                int timeToScreenOff = 30000;
                unsigned long lastEvent = 0;
                uint16_t lastMode = -1;

            public:
                void setTimeToScreenOff(int value){
                    this->timeToScreenOff = value;
                }
                void resetLastEventTimer(){
                    lastEvent = esphome::millis();
                }

                uint16_t getHeight(){
                    return M5Dial.Display.height();
                }
                uint16_t getWidth(){
                    return M5Dial.Display.width();
                }

                LovyanGFX* getGfx(){
                    return gfx;
                }

                bool isDisplayOn(){
                    return M5Dial.Display.getBrightness() > 0;
                }

                void validateTimeout(){
                    if (esphome::millis() - lastEvent > timeToScreenOff ) {
                        if(M5Dial.Display.getBrightness()>0){
                            M5Dial.Display.setBrightness(0);
                            ESP_LOGI("SLEEP", "Sleep after %d ms", timeToScreenOff);
                        }
                    } else if ( M5Dial.Display.getBrightness()<=0 ) {
                        M5Dial.Display.setBrightness(100);
                        ESP_LOGI("SLEEP", "Display on");
                    }
                }

                void showOffline(){
                    uint16_t height = this->getHeight();
                    uint16_t width  = this->getWidth();

                    gfx->setTextColor(LIGHTGREY);
                    gfx->setTextDatum(middle_center);
                    gfx->setFont(&fonts::FreeMono12pt7b);

                    gfx->startWrite();                      // Secure SPI bus
                    gfx->fillRect(0, 0, width, height, DARKGREY);
                    
                    gfx->setTextSize(2);
                    gfx->drawString("OFFLINE",
                                    width / 2,
                                    height / 2);

                    gfx->endWrite();                      // Release SPI bus
                }

                void showDisconnected(){
                    uint16_t height = this->getHeight();
                    uint16_t width  = this->getWidth();

                    gfx->setTextColor(WHITE);
                    gfx->setTextDatum(middle_center);
                    gfx->setFont(&fonts::FreeMono12pt7b);

                    gfx->startWrite();                      // Secure SPI bus
                    gfx->fillRect(0, 0, width, height, BLUE);
                    
                    gfx->setTextSize(1);
                    gfx->drawString("DISCONNECTED",
                                    width / 2,
                                    height / 2);
<<<<<<< Updated upstream
=======
                    gfx->drawString("from HA",
                                    width / 2,
                                    height / 2+22);
>>>>>>> Stashed changes

                    gfx->endWrite();                      // Release SPI bus
                }

                void showUnknown(){
                    uint16_t height = this->getHeight();
                    uint16_t width  = this->getWidth();

                    gfx->setTextColor(MAROON);
                    gfx->setTextDatum(middle_center);
                    gfx->setFont(&fonts::FreeMono12pt7b);

                    gfx->startWrite();                      // Secure SPI bus
                    gfx->fillRect(0, 0, width, height, ORANGE);
                    
                    gfx->setTextSize(2);
                    gfx->drawString("UNKNOWN",
                                    width / 2,
                                    height / 2);

                    gfx->endWrite();                      // Release SPI bus
                }

                



                float getDegByCoord(uint16_t x, uint16_t y){
                    float mx = M5Dial.Display.width()/2;
                    float my = M5Dial.Display.height()/2;

                    float angle = atan2(y - my, x - mx) * 180.0 / M_PI;
                    angle = 360 - fmod((angle + 360.0 - 90), 360.0);

                    return angle;
                }

        };
    }
}