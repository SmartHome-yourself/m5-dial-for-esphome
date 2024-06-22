#pragma once
#include "globals.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class ShysM5Dial;

        class HaDeviceModeMenuTest: public esphome::shys_m5_dial::HaDeviceMode {
            /**
             * @brief class to draw and use a circular menu, similar to a clock
             * 
             */

            protected:
                /**
                 * @todo remove not needed members:  changeableDirection, direction, stateIsOn
                 * 
                 */
                bool changeableDirection = false;
                const char* direction = FAN_DIRECTION_FORWARD;

                bool stateIsOn = false;

                ShysM5Dial* M5DialObj=NULL;
                uint16_t NumberOfDevices=0;

                typedef struct coord {
                    int32_t x, y;
                } COORD;

                /**
                 * @todo remove all this directional stuff 
                 * 
                 */
                void changeDirection(){
                    if(changeableDirection){
                        if (strcmp(direction, FAN_DIRECTION_FORWARD) == 0){
                            this->publishDirection(FAN_DIRECTION_REVERSE);
                        } else {
                            this->publishDirection(FAN_DIRECTION_FORWARD);
                        }
                    }
                    
                }

                void sendValueToHomeAssistant(int value) override {
                    // do nothing
                    // haApi.setFanSpeed(this->device.getEntityId(), value);
                }

                coord getDegreeCoord(LovyanGFX* gfx, float radius, float degree){
                    coord result;
                    result.x = (int32_t) (radius * sin(degree*M_PI/180) + (gfx->width()/2));
                    result.y = (int32_t) (radius * cos(degree*M_PI/180) + (gfx->height()/2));
                    return result;
                }

                /**
                 * @brief Draw only inner circle, arrow and new text
                 * 
                 * @param display 
                 * @param currentValue 
                 */
                void refreshInnerMenu(M5DialDisplay& display, uint16_t currentValue);
                
                /**
                 * @brief Draws the menu with all elements and details
                 *        if used for each redraw then there is to much flicker - to slow
                 *        is it possible to redraw only part? maybe with "once" flag or additional parameter?
                 *  
                 * @param display 
                 * @param currentValue 
                 */
                void showMenu(M5DialDisplay& display, uint16_t currentValue){
                    LovyanGFX* gfx = display.getGfx();
                    uint16_t height = gfx->height();
                    uint16_t width  = gfx->width();
                    int size=width/6-2;

                    gfx->setTextColor(BLACK);
                    gfx->setTextDatum(middle_center);

                    gfx->startWrite();                      // Secure SPI bus

                    gfx->fillRect(0, 0, width, height, 0xde74);

                    display.setFontsize(1.8);
                    gfx->drawString((String(currentValue)).c_str(),
                                    width / 2,
                                    height / 2 - 30);

                    display.setFontsize(0.5);
                    gfx->setTextDatum(middle_center);
                    gfx->drawString(this->device.getName().c_str(),
                                    width / 2,
                                    height / 2 + 50);


                #if (0)
                    const char*  texte[] = {"Versuch", "Menuliste", "Test", "M5Dial", "Ardufox"};
                    for (int i=0; i < 5; i++) {
                        gfx->drawString(texte[i],
                                        width / 2,
                                        i * 20 +20);
                    }
                #endif

                    display.setFontsize(1.0);
                    gfx->setTextDatum(middle_center);

                    char text[40];
                    
                    snprintf(text, sizeof(text), "showMenu( ,%d)", currentValue);
                    ESP_LOGI("DISPLAY", text);

                    snprintf(text, sizeof(text), "  size=%d width=%d", size, width);
                    ESP_LOGI("DISPLAY", text);
                    gfx->setTextColor(WHITE);

                    // draw items in clock like layout
                    for (int clock=1; clock<13; clock++) {
                        int degr = ((12-clock)*30+180)%360;
                        coord c= getDegreeCoord(gfx,width/2-size/2-6,degr);
                        
                        //snprintf(text, sizeof(text), "  clock=%2d -> %3d° (%3d/%3d)", clock, degr,  c.x, c.y);
                        //ESP_LOGI("DISPLAY", text);

                        /*
                        LGFX_INLINE_T void drawRoundRect   ( int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, const T& color) { setColor(color); drawRoundRect(x, y, w, h, r); }
                        LGFX_INLINE_T void fillRoundRect   ( int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, const T& color) { setColor(color); fillRoundRect(x, y, w, h, r); }
                        */
                        gfx->fillRoundRect(c.x-size/2, c.y-size/2,size,size, size/4,  MAROON); // 0xffcf); // 0xffff);  // weiß
                        gfx->drawRoundRect(c.x-size/2, c.y-size/2,size,size, size/4,  (uint8_t) 0x6310);

                        snprintf(text, sizeof(text), "%d", clock);
                        gfx->drawString(text, c.x, c.y);  

                    }
                    gfx->endWrite();                      // Release SPI bus

                    this->refreshInnerMenu(display, currentValue);
                }

            public:
                HaDeviceModeMenuTest(HaDevice& device) : HaDeviceMode(device){
                    
                    this->minValue = 1;
                    this->maxValue = 12;
                    this->rotaryStepWidth = 1;
                    this->endlessRotaryValue=true;
                    this->changeableDirection = false;
                }

                void setM5DialObj(ShysM5Dial * m5D);

                void setState(const std::string& newState){
                    this->stateIsOn = true; // (strcmp(newState.c_str(), "on") == 0);
                }

                void setChangeableDirection(bool isChangeable){
                    //this->changeableDirection = isChangeable;
                }

                /**
                 * @todo remove all this directional stuff 
                 * 
                 */
                void publishDirection(const std::string& newDirection){
                    setDirection(newDirection);
                    //  do nothing
                    // haApi.setFanDirection(this->device.getEntityId(), this->direction);
                }

                void setDirection(const std::string& newDirection){
                    if (strcmp(newDirection.c_str(), FAN_DIRECTION_FORWARD) == 0){
                        this->direction = FAN_DIRECTION_FORWARD;
                    } else if (strcmp(newDirection.c_str(), FAN_DIRECTION_REVERSE) == 0){
                        this->direction = FAN_DIRECTION_REVERSE;
                    }

                    displayRefreshNeeded = true;
                    
                    ESP_LOGD("DEVICE", "set direction: %s", this->direction);
                }

                void refreshDisplay(M5DialDisplay& display, bool init) override {
                    if (init)   this->showMenu(display, getValue());
                    else        this->refreshInnerMenu(display, getValue());
                    
                    this->displayRefreshNeeded = false;
                }

                void registerHAListener() override {
                    std::string attrNameState = "";
                }

                bool onTouch(M5DialDisplay& display, uint16_t x, uint16_t y) override {
                    /**
                     * @todo write code, to handle "click" near button
                     * 
                     */
                    return this->defaultOnTouch(display, x, y);
                }


                bool onRotary(M5DialDisplay& display, const char * direction) override {
                    if (strcmp(direction, ROTARY_LEFT)==0){
                        this->reduceCurrentValue();
                    } else if (strcmp(direction, ROTARY_RIGHT)==0){
                        this->raiseCurrentValue();
                    }

                    this->displayRefreshNeeded = true;

                    return true;
                }

                bool onButton(M5DialDisplay& display, const char * clickType) override; 


                void setMinValue(int val){
                    this->minValue = 1;
                }

                void setMaxValue(int val){
                    this->maxValue = 12;
                }

                void setRotaryStepWidth(int stepWidth){
                    this->rotaryStepWidth = 1;
                }
        };
    }
}