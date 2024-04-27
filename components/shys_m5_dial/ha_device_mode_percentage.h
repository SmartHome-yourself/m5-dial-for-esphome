#pragma once

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModePercentage: public esphome::shys_m5_dial::HaDeviceMode {
            protected:
                std::string label = "Percentage";
                std::string unit = "%";
                unsigned short* icon = nullptr;

                bool barActive = true;
                bool use_custom_value = false;
                std::string custom_value = "";


                void showPercentageMenu(M5DialDisplay& display){
                    LovyanGFX* gfx = display.getGfx();

                    uint16_t height = gfx->height();
                    uint16_t width  = gfx->width();

                    gfx->setTextColor(MAROON);
                    gfx->setTextDatum(middle_center);

                    gfx->startWrite();                      // Secure SPI bus

                    display.clear();

                    float valOnArc = (getMaxValue()==0?240:((float)240 / (this->getMaxValue() - this->getMinValue())) * (getValue() - this->getMinValue())) + 150;

                    if(this->isBarActive()){
                        // Round %-Bar
                        gfx->fillArc(width / 2,
                                    height / 2,
                                    115,
                                    100,
                                    150,
                                    valOnArc,
                                    RED
                                    );

                        gfx->fillArc(width / 2,
                                    height / 2,
                                    115,
                                    100,
                                    valOnArc,
                                    390,
                                    ORANGE
                                    );
                    } else {
                        gfx->fillArc(width / 2,
                                    height / 2,
                                    115,
                                    100,
                                    150,
                                    390,
                                    display.getBackgroundColor()
                                    );
                    }

                    // Percent
                    display.setFontsize(1.7);
                    gfx->drawString(use_custom_value ? custom_value.c_str() : (String(getValue()) + this->unit.c_str()).c_str(),
                                    width / 2,
                                    height / 2 - 70);

                    // Mode
                    display.setFontsize(1);
                    gfx->drawString(this->label.c_str(),
                                    width / 2,
                                    height / 2 - 40);  

                    // Icon
                    if(this->icon != nullptr){
                        display.drawBitmapTransparent(this->icon, width/2-35, height/2-30, 70, 70, 0xFFFF);
                    }

                    // Device Name
                    display.setFontsize(1);
                    gfx->drawString(this->device.getName().c_str(),
                                    width / 2,
                                    height / 2 + 90);
 

                    gfx->endWrite();                      // Release SPI bus
                }

            public:
                HaDeviceModePercentage(HaDevice& device) : HaDeviceMode(device){}

                ~HaDeviceModePercentage() {
                    delete[] icon;
                }

                void setLabel(const std::string& newLabel){
                    this->label = newLabel;
                }

                void setCustomValue(const std::string& newVal){
                    this->custom_value = newVal;
                }

                void useCustomValue(bool activate){
                    this->use_custom_value = activate;
                }

                void setUnit(const std::string& newUnit){
                    this->unit = newUnit;
                }

                void setIcon(const unsigned short* newIcon, size_t size) {
                    delete[] this->icon;
                    this->icon = new unsigned short[size];
                    std::copy(newIcon, newIcon + size, icon);
                }

                void activateBar(bool activate){
                    this->barActive = activate;
                }

                bool isBarActive(){
                    return this->barActive;
                }

                void refreshDisplay(M5DialDisplay& display, bool init) override {
                    ESP_LOGD("DISPLAY", "refresh Display: Percentage-Modus");
                    showPercentageMenu(display);
                }
                
                bool onTouch(M5DialDisplay& display, uint16_t x, uint16_t y) override {
                    return this->defaultOnTouch(display, x, y);        
                }

                bool onRotary(M5DialDisplay& display, const char * direction) override {
                    return this->defaultOnRotary(display, direction);
                }
        };
    }
}