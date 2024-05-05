#pragma once
#include "M5Dial.h"
#include "esphome.h"
#include "default_font_16px.h"
#include "screensaver.h"

#define FF_DEFAULT &default_font_16px


/**
 * M5Dial Display
 *--------------------------
 * Display driver: GC9A01
 * Resolution: 240x240
 * Touch driver: FT3267
 */
namespace esphome
{
    namespace shys_m5_dial
    {
        class M5DialDisplay {
            protected:
                uint16_t backgroundColor = YELLOW;

                LovyanGFX* gfx = &M5Dial.Display;

                int timeToScreenOff = 30000;
                unsigned long lastEvent = 0;
                uint16_t lastMode = -1;

                std::string fontName = "default";  //"FreeMono12pt7b";
                float fontFactor = 1;

                Screensaver* screensaver = nullptr;
                bool screensaverRunning = false;

                std::function<void(bool)> display_refresh_action;

            public:
                M5DialDisplay() {
                }

                void on_display_refresh(std::function<void(bool)> callback){
                    ESP_LOGD("DEVICE", "register on_swipe Callback");
                    this->display_refresh_action = callback;
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

                LovyanGFX* getGfx() {
                    return gfx;
                }

                void setFontName(std::string name){
                    this->fontName = name;
                }

                void setFontFactor(float factor){
                    this->fontFactor = factor;
                }

                bool isDisplayOn(){
                    return M5Dial.Display.getBrightness() > 0;
                }

                void setBackgroundColor(uint16_t color){
                    this->backgroundColor = backgroundColor;
                }

                uint16_t getBackgroundColor(){
                    return this->backgroundColor;
                }

                void setScreensaver(Screensaver* saver){
                    this->screensaver = saver;
                }

                bool isScreensaverActive(){
                    return this->screensaver != nullptr;
                }
                
                bool isScreensaverRunning(){
                    return screensaverRunning;
                }
                
                void resetScreensaverRunning(){
                    screensaverRunning = false;
                }

                void validateTimeout(){
                    if (esphome::millis() - lastEvent > timeToScreenOff ) {
                        if(this->isScreensaverActive()){
                            bool forceRefresh = !screensaverRunning;
                            screensaver->show(*this, forceRefresh);
                            
                            screensaverRunning = true;
                        } else {
                            if(M5Dial.Display.getBrightness()>0){
                                M5Dial.Display.setBrightness(0);
                                ESP_LOGI("DISPLAY", "Sleep after %d ms", timeToScreenOff);
                            }
                        } 
                    } else {
                        if(screensaverRunning){
                            this->resetScreensaverRunning();
                            this->display_refresh_action(true);
                        }

                        if ( M5Dial.Display.getBrightness()<=0 ) {
                            M5Dial.Display.setBrightness(100);
                            ESP_LOGI("DISPLAY", "Display on");
                        }
                    }
                }


                void showOffline(){
                    uint16_t height = this->getHeight();
                    uint16_t width  = this->getWidth();

                    gfx->setTextColor(LIGHTGREY);
                    gfx->setTextDatum(middle_center);

                    this->setFontByName(this->fontName);

                    gfx->startWrite();                      // Secure SPI bus
                    this->clear(DARKGREY);
                    
                    this->setFontsize(2);
                    gfx->drawString("OFFLINE",
                                    width / 2,
                                    height / 2);

                    gfx->endWrite();                      // Release SPI bus
                    this->resetScreensaverRunning();
                }

                void showDisconnected(){
                    uint16_t height = this->getHeight();
                    uint16_t width  = this->getWidth();

                    gfx->setTextColor(WHITE);
                    gfx->setTextDatum(middle_center);

                    this->setFontByName(this->fontName);

                    gfx->startWrite();                      // Secure SPI bus
                    this->clear(BLUE);
                    
                    this->setFontsize(1);
                    gfx->drawString("DISCONNECTED",
                                    width / 2,
                                    height / 2);

                    gfx->endWrite();                      // Release SPI bus
                    this->resetScreensaverRunning();
                }

                void showUnknown(){
                    uint16_t height = this->getHeight();
                    uint16_t width  = this->getWidth();

                    gfx->setTextColor(MAROON);
                    gfx->setTextDatum(middle_center);

                    this->setFontByName(this->fontName);

                    gfx->startWrite();                      // Secure SPI bus
                    
                    this->clear(ORANGE);
                    
                    this->setFontsize(2);

                    gfx->drawString("UNKNOWN",
                                    width / 2,
                                    height / 2);

                    gfx->endWrite();                      // Release SPI bus
                    this->resetScreensaverRunning();
                }

                float getDegByCoord(uint16_t x, uint16_t y){
                    float mx = M5Dial.Display.width()/2;
                    float my = M5Dial.Display.height()/2;

                    float angle = atan2(y - my, x - mx) * 180.0 / M_PI;
                    //angle = 360 - fmod((angle + 360.0 - 90), 360.0);
                    angle = fmod((angle + 360.0 - 90), 360.0);
                    return angle;
                }

                float getRadiusFromCoord(float touchX, float touchY) {
                    float dx = touchX - (getWidth() / 2.0f);
                    float dy = touchY - (getHeight() / 2.0f);
                    float radius = sqrt(dx * dx + dy * dy);

                    return radius;
                }

                coord getColorCoord(float radius, float degree){
                    coord result;
                    result.x = radius * sin(degree*M_PI/180) + (gfx->width()/2);
                    result.y = radius * cos(degree*M_PI/180) + (gfx->height()/2);
                    return result;
                }

                void drawColorCircleLine(float degree, float r1, float r2, uint32_t color) {
                    uint16_t step = 1;
                    coord c1 = getColorCoord(r1, degree);
                    coord c2 = getColorCoord(r2, degree-step);
                    coord c3 = getColorCoord(r2, degree+step);

                    M5Dial.Display.fillTriangle(c1.x, c1.y, c2.x, c2.y, c3.x, c3.y, color);

                    c1 = getColorCoord(r1, degree);
                    c2 = getColorCoord(r1, degree-step-step);
                    c3 = getColorCoord(r2, degree-step);
                    M5Dial.Display.fillTriangle(c1.x, c1.y, c2.x, c2.y, c3.x, c3.y, color);
                }

                void setFontsize(float size) {
                    getGfx()->setTextSize(size * this->fontFactor);
                }

                int getRowHeight(float fontSize){
                    return (int)this->fontFactor * fontSize;
                }

                void setFontByName(const std::string& name) {
                    if (FONT_MAP.find(name) != FONT_MAP.end()) {
                        this->setFontName(name);
                    } else {
                        this->setFontName("default");
                        ESP_LOGE("DISPLAY", "Font '%s' not found, using default font: 'default'", name.c_str());
                    }

                    if(strcmp(name.c_str(), "default")==0){
                        getGfx()->setFont(FF_DEFAULT);
                    } else {
                        getGfx()->setFont(FONT_MAP[this->fontName]);
                    }
                }

                void drawBitmap(const uint8_t* bmp, int size, uint8_t x, uint8_t y, uint8_t width, uint8_t height){
                    M5Dial.Display.drawJpg(bmp, size, x, y, width, height, 0, 0);
                }

                void drawBitmapTransparent(const uint16_t* bmp, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint32_t transparentColor){
                    M5Dial.Display.pushImage(x, y, width, height, bmp, transparentColor);
                }

                void clear(uint16_t bgColor){
                    M5Dial.Display.fillRect(0, 0, getWidth(), getHeight(), bgColor);
                }

                void clear(){
                    this->clear(this->backgroundColor);
                }
        };
    }
}