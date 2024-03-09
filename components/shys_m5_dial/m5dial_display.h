#pragma once
#include "M5Dial.h"


#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

namespace esphome
{
    namespace shys_m5_dial
    {
        class M5DialDisplay {
            protected:
                LovyanGFX* gfx = &M5Dial.Display;

                int timeToScreenOff = 30000;
                unsigned long lastEvent = 0;
                std::function<void(const std::string)> publish_state_action;

                typedef struct rgb {
                    float r, g, b;
                } RGB;

                typedef struct hsl {
                    float h, s, l;
                } HSL;

                typedef struct coord {
                    float x, y;
                } COORD;

                uint32_t getColorByDegree(float degree){
                    return hslToRgb(degree/360.0, 1, .5);
                }
                
                uint32_t getComplementaryByDegree(float degree){
                    uint32_t complementary = degree;
                    if(complementary >= 180){
                        complementary = complementary - 180;
                    } else {
                        complementary = complementary + 180;
                    }
                    return getColorByDegree(complementary);
                }

                /*
                * Converts an HUE to r, g or b.
                * returns float in the set [0, 1].
                */
                float hue2rgb(float p, float q, float t) {
                    if (t < 0) 
                        t += 1;
                    if (t > 1) 
                        t -= 1;
                    if (t < 1./6) 
                        return p + (q - p) * 6 * t;
                    if (t < 1./2) 
                        return q;
                    if (t < 2./3)   
                        return p + (q - p) * (2./3 - t) * 6;
                        
                    return p;
                }

                /*
                * Convert HSL to RGB.
                * http://en.wikipedia.org/wiki/HSL_color_space.
                * 
                * h, s, l [float 0 - 1]
                * 
                * returns color888
                */
                uint32_t hslToRgb(float h, float s, float l) {
                    RGB result;
                    
                    if(0 == s) {
                        result.r = result.g = result.b = l; // achromatic
                    }
                    else {
                        float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
                        float p = 2 * l - q;
                        result.r = hue2rgb(p, q, h + 1./3) * 255;
                        result.g = hue2rgb(p, q, h) * 255;
                        result.b = hue2rgb(p, q, h - 1./3) * 255;
                    }

                    return M5Dial.Display.color888(result.r, result.g, result.b);
                }


                void showBrightnessMenu(uint16_t currentValue, HaDevice currentDevice){
                    uint16_t height = this->getHeight();
                    uint16_t width  = this->getWidth();

                    gfx->setTextColor(MAROON);
                    gfx->setTextDatum(middle_center);
                    gfx->setFont(&fonts::FreeMono12pt7b);

                    gfx->startWrite();                      // Secure SPI bus

                    if(currentDevice.isDimmEnabled()){
                        gfx->fillRect(0, 0, width, height-(height*currentValue/100), RED);
                        gfx->fillRect(0, height-(height*currentValue/100), width, height, YELLOW);

                        gfx->setTextSize(3);
                        gfx->drawString(String(currentValue),
                                        width / 2,
                                        height / 2 - 30);
                    } else {
                        gfx->fillRect(0, 0, width, height, currentValue>0?YELLOW:RED);

                        gfx->setTextSize(3);
                        gfx->drawString(currentValue>0?"on":"off",
                                        width / 2,
                                        height / 2 - 30);                        
                    }
                    
                    gfx->setTextSize(1);
                    gfx->drawString(currentDevice.getName().c_str(),
                                    width / 2,
                                    height / 2 + 20);
                    gfx->drawString(currentDevice.getDeviceType().c_str(),
                                    width / 2,
                                    height / 2 + 50);  

                    gfx->endWrite();                      // Release SPI bus
                }


                void drawColorCircleLine(float degree, float r1, float r2, uint32_t color) {
                    float step=0.6;
                    
                    // void fillTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, const T &color)
                    coord c1 = getColorCoord(r1, degree);
                    coord c2 = getColorCoord(r2, degree-step);
                    coord c3 = getColorCoord(r2, degree+step);
                    M5Dial.Display.fillTriangle(c1.x, c1.y, c2.x, c2.y, c3.x, c3.y, color);
                    
                    c1 = getColorCoord(r1, degree);
                    c2 = getColorCoord(r1, degree-step-step);
                    c3 = getColorCoord(r2, degree-step);
                    M5Dial.Display.fillTriangle(c1.x, c1.y, c2.x, c2.y, c3.x, c3.y, color);                    
                }

                void refreshColorMenu(uint16_t currentValue, HaDevice currentDevice){
                    uint32_t complementary_color = getComplementaryByDegree(currentValue);

                    int height = this->getHeight();
                    int width  = this->getWidth();

                    gfx->setTextColor(complementary_color);
                    gfx->setTextDatum(middle_center);
                    gfx->setFont(&fonts::FreeMono12pt7b);

                    gfx->startWrite();                    // Secure SPI bus
                    gfx->fillCircle(width/2, height/2, 70, getColorByDegree(currentValue));

                    gfx->setTextSize(1);
                    gfx->drawString(String(currentValue),
                                    width / 2,
                                    height / 2 - 20);

                    gfx->setTextSize(1);
                    gfx->drawString(currentDevice.getName().c_str(),
                                    width / 2,
                                    height / 2 + 20);
                    gfx->drawString(currentDevice.getDeviceType().c_str(),
                                    width / 2,
                                    height / 2 + 50);  

                    drawColorCircleLine(currentDevice.getColor(), 40, 69, complementary_color);
                    gfx->endWrite();                      // Release SPI bus
                }

                void showColorMenu(uint16_t currentValue, HaDevice currentDevice){
                    uint32_t complementary_color = getComplementaryByDegree(currentValue);

                    int height = this->getHeight();
                    int width  = this->getWidth();

                    gfx->startWrite();                      // Secure SPI bus

                    gfx->fillRect(0, 0, width, height, BLACK);

                    for (int i=0; i<360; i++){
                        drawColorCircleLine(i, 70.0, 130.0, getColorByDegree(i));
                    }

                    gfx->endWrite();                      // Release SPI bus

                    refreshColorMenu(currentValue, currentDevice);
                }

                coord getColorCoord(float radius, float degree){
                    coord result;
                    result.x = radius * sin(degree*M_PI/180) + (this->getWidth()/2);
                    result.y = radius * cos(degree*M_PI/180) + (this->getHeight()/2);
                    return result;
                }


                void showUnknown(){
                    uint16_t height = this->getHeight();
                    uint16_t width  = this->getWidth();

                    gfx->setTextColor(MAROON);
                    gfx->setTextDatum(middle_center);
                    gfx->setFont(&fonts::FreeMono12pt7b);

//                    gfx->clear();
                    gfx->startWrite();                      // Secure SPI bus
                    gfx->fillRect(0, 0, width, height, ORANGE);
                    
                    gfx->setTextSize(2);
                    gfx->drawString("UNKNOWN",
                                    width / 2,
                                    height / 2);

                    gfx->endWrite();                      // Release SPI bus
                }



            public:
                void on_publish_state(std::function<void(const std::string)> callback){
                    this->publish_state_action = callback;
                }

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

                uint16_t lastMode = -1;
                void refreshDisplay(uint16_t currentValue, HaDevice currentDevice){
                    if(currentDevice.getCurrentMode() == DEVICE_MODE_BRIGHTNESS){
                        ESP_LOGD("DISPLAY", "Helligkeits-Modus");
                        showBrightnessMenu(currentValue, currentDevice);
                        lastMode = DEVICE_MODE_BRIGHTNESS;
                    } else if(currentDevice.getCurrentMode() == DEVICE_MODE_COLOR){
                        ESP_LOGD("DISPLAY", "Farbwahl-Modus");
                        if(lastMode == DEVICE_MODE_COLOR){
                            refreshColorMenu(currentValue, currentDevice);
                        } else {
                            showColorMenu(currentValue, currentDevice);
                        }
                        lastMode = DEVICE_MODE_COLOR;
                    } else {
                        ESP_LOGD("DISPLAY", "Modus unbekannt");
                        showUnknown();
                    }
                }

                bool isDisplayOn(){
                    return M5Dial.Display.getBrightness() > 0;
                }


                void validateTimeout()
                {
                    if (esphome::millis() - lastEvent > timeToScreenOff ) {
                        if(M5Dial.Display.getBrightness()>0){
                            M5Dial.Display.setBrightness(0);
                            ESP_LOGI("SLEEP", "Sleep after %d ms", timeToScreenOff);
                            this->publish_state_action("Display off");
                        }
                    } else if ( M5Dial.Display.getBrightness()<=0 ) {
                        M5Dial.Display.setBrightness(100);
                        ESP_LOGI("SLEEP", "Display on");
                        this->publish_state_action("Display on");
                    }
                }

                float getDegByCoord(uint16_t x, uint16_t y){
                    float mx = M5Dial.Display.width()/2;
                    float my = M5Dial.Display.height()/2;

                    float angle = atan2(y - my, x - mx) * 180.0 / M_PI;
                    angle = 360 - fmod((angle + 360.0 - 90), 360.0);

                    return angle;
                }

                void showOffline(){
                    uint16_t height = this->getHeight();
                    uint16_t width  = this->getWidth();

                    gfx->setTextColor(LIGHTGREY);
                    gfx->setTextDatum(middle_center);
                    gfx->setFont(&fonts::FreeMono12pt7b);

//                    gfx->clear();
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

//                    gfx->clear();
                    gfx->startWrite();                      // Secure SPI bus
                    gfx->fillRect(0, 0, width, height, BLUE);
                    
                    gfx->setTextSize(1);
                    gfx->drawString("DISCONNECTED",
                                    width / 2,
                                    height / 2);

                    gfx->endWrite();                      // Release SPI bus
                }
                
        };
    }
}
