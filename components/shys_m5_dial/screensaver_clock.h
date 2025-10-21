#pragma once
#include "screensaver.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class ScreensaverClock: public esphome::shys_m5_dial::Screensaver {
            protected:
                esphome::time::RealTimeClock* localTime;

                uint8_t lastHr  = 255;
                uint8_t lastMin = 255;
                uint8_t lastSec = 255;

                void showClock(M5DialDisplay& display){
                    display.clear(BLACK);

                    for(int i=1;i<=12;i++){
                        int degree = i*30;
                        coord c1 = display.getColorCoord(i%3?117:112, degree);
                        coord c2 = display.getColorCoord(120, degree-(i%3?1:4));
                        coord c3 = display.getColorCoord(120, degree+(i%3?1:4));

                        display.getGfx()->fillTriangle(c1.x, c1.y, c2.x, c2.y, c3.x, c3.y, WHITE);
                    }
                }

                void refreshClock(M5DialDisplay& display){
                    int x = display.getWidth()/2;
                    int y = display.getHeight()/2;

                    uint8_t hour = this->localTime->now().hour;
                    if(lastHr != hour){
                        display.getGfx()->fillCircle(x, y, 110, BLACK);
                        lastHr  = 255;
                        lastMin = 255;
                        lastSec = 255;                        
                    }

                    this->paintHourHand(display);
                    this->paintMinuteHand(display);
                    this->paintSecondHand(display);

                    display.getGfx()->fillCircle(x, y, 5, WHITE);
                }

                void paintHourHand(M5DialDisplay& display){
                    uint8_t hour = this->localTime->now().hour;
                    uint8_t minute = this->localTime->now().minute;

                    if(lastHr != hour || lastMin != minute){
                        int degree = 180 - (lastHr * 360 / 12);
                        degree = degree - (round(lastMin/12) * 360 / 60);
                        paintHand(display, degree, 32, 70, BLACK);
                    }

                    int degree = 180 - (hour * 360 / 12);
                    degree = degree - (round(minute/12) * 360 / 60);
                    paintHand(display, degree, 30, 70, WHITE);

                    lastHr = hour;
                }

                void paintMinuteHand(M5DialDisplay& display){
                    uint8_t minute = this->localTime->now().minute;

                    if(lastMin != minute){
                        int degree = 180 - (lastMin * 360 / 60);
                        paintHand(display, degree, 22, 95, BLACK);
                    }

                    int degree = 180 - (minute * 360 / 60);
                    paintHand(display, degree, 20, 95, WHITE);

                    lastMin = minute;
                }

                void paintSecondHand(M5DialDisplay& display){
                    uint8_t second = this->localTime->now().second;

                    if(lastSec != second){
                        int degree = 180 - (lastSec * 360 / 60);
                        paintHand(display, degree, 2, 105, BLACK);
                    }

                    int degree = 180 - (second * 360 / 60);
                    paintHand(display, degree, 1, 105, RED);

                    lastSec = second;
                }

                void paintHand(M5DialDisplay& display, int degree, int width, int length, uint16_t color){
                        coord c1 = display.getColorCoord(length, degree);
                        coord c2 = display.getColorCoord(4, degree - width);
                        coord c3 = display.getColorCoord(4, degree + width);

                        display.getGfx()->fillTriangle(c1.x, c1.y, c2.x, c2.y, c3.x, c3.y, color);
                }

            public:
                void show(M5DialDisplay& display, bool init) override {
                    if(init){
                        showClock(display);
                    } else {
                        refreshClock(display);
                    }
                }

                void setLocalTime(esphome::time::RealTimeClock* time_comp) {
                    this->localTime = time_comp;
                }

        };

    }
}