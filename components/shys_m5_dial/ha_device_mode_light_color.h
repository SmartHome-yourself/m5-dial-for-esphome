#pragma once
#include "m5dial_display.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModeLightColor: public esphome::shys_m5_dial::HaDeviceMode {
            protected:
                void sendValueToHomeAssistant(int value) override {
                    haApi.turnLightOn(this->device.getEntityId(), -1, value);
                }

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


                coord getColorCoord(LovyanGFX* gfx, float radius, float degree){
                    coord result;
                    result.x = radius * sin(degree*M_PI/180) + (gfx->width()/2);
                    result.y = radius * cos(degree*M_PI/180) + (gfx->height()/2);
                    return result;
                }

                void drawColorCircleLine(LovyanGFX* gfx, float degree, float r1, float r2, uint32_t color) {
                    uint16_t step = 1;
                    coord c1 = getColorCoord(gfx, r1, degree);
                    coord c2 = getColorCoord(gfx, r2, degree-step);
                    coord c3 = getColorCoord(gfx, r2, degree+step);

                    M5Dial.Display.fillTriangle(c1.x, c1.y, c2.x, c2.y, c3.x, c3.y, color);

                    c1 = getColorCoord(gfx, r1, degree);
                    c2 = getColorCoord(gfx, r1, degree-step-step);
                    c3 = getColorCoord(gfx, r2, degree-step);
                    M5Dial.Display.fillTriangle(c1.x, c1.y, c2.x, c2.y, c3.x, c3.y, color);
                }

                void refreshColorMenu(M5DialDisplay& display){
                    LovyanGFX* gfx = display.getGfx();

                    int currentValue = getValue();
                    uint32_t complementary_color = getComplementaryByDegree(currentValue);

                    int height = gfx->height();
                    int width  = gfx->width();

                    gfx->setTextColor(complementary_color);
                    gfx->setTextDatum(middle_center);

                    gfx->startWrite();                    // Secure SPI bus
                    gfx->fillCircle(width/2, height/2, 70, getColorByDegree(currentValue));

                    display.setFontsize(1);
                    gfx->drawString(String(currentValue),
                                    width / 2,
                                    height / 2 - 20);

                    display.setFontsize(1);
                    gfx->drawString(this->device.getName().c_str(),
                                    width / 2,
                                    height / 2 + 20);
                    gfx->drawString("Color",
                                    width / 2,
                                    height / 2 + 50);  

                    drawColorCircleLine(gfx, currentValue, 40, 69, complementary_color);
                    gfx->endWrite();                      // Release SPI bus
                }

                void showColorMenu(M5DialDisplay& display){
                    LovyanGFX* gfx = display.getGfx();

                    int currentValue = getValue();
                    uint32_t complementary_color = getComplementaryByDegree(currentValue);

                    int height = gfx->height();
                    int width  = gfx->width();

                    gfx->startWrite();                      // Secure SPI bus

                    gfx->fillRect(0, 0, width, height, BLACK);

                    for (int i=0; i<360; i++){
                        drawColorCircleLine(gfx, i, 70.0, 130.0, getColorByDegree(i));
                    }

                    gfx->endWrite();                      // Release SPI bus

                    refreshColorMenu(display);
                }


            public:
                HaDeviceModeLightColor(HaDevice& device) : HaDeviceMode(device){
                    this->maxValue = 360;
                    this->endlessRotaryValue = true;
                }

                void refreshDisplay(M5DialDisplay& display, bool init) override {
                    ESP_LOGD("DISPLAY", "refresh Display: Farbwahl-Modus");
                    if(init){
                        showColorMenu(display);
                    } else {
                        refreshColorMenu(display);
                    }
                }

                void registerHAListener() override {
                    std::string attrName = "hs_color";
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                attrName, 
                                [this](const std::string &state) {
                                
                        if(this->isValueModified()){
                            return;
                        }

                        std::string colorString = "";          
                        std::string::size_type pos = state.find(',');
                        
                        if (pos != std::string::npos) {
                            colorString = state.substr(1, pos-1);
                        }
                        ESP_LOGD("HA_API", "HS_Color value %s for %s", colorString.c_str(), this->device.getEntityId().c_str());

                        auto val = parse_number<float>(colorString.c_str());
                        if (!val.has_value()) {
                            this->setReceivedValue(0);
                            ESP_LOGD("HA_API", "No Color value in %s for %s", colorString.c_str(), this->device.getEntityId().c_str());
                        } else {
                            this->setReceivedValue(val.value());
                            ESP_LOGI("HA_API", "Got Color value %f for %s", val.value(), this->device.getEntityId().c_str());
                        }
                    });
                }
                
                bool onTouch(M5DialDisplay& display, uint16_t x, uint16_t y) override {
                    uint16_t degree = display.getDegByCoord(x, y);
                    setValue(degree);
                    ESP_LOGD("TOUCH", "Neuen Farbwert auf %d gesetzt", degree);
                    
                    return true;
                }

                bool onRotary(M5DialDisplay& display, const char * direction) override {
                    return this->defaultOnRotary(display, direction);
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