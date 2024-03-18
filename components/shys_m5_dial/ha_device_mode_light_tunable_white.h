#pragma once
#include <math.h>

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModeLightTunableWhite: public esphome::shys_m5_dial::HaDeviceMode {
            protected:
                void sendValueToHomeAssistant(int value) override {
                    haApi.turnLightOnWhite(this->device.getEntityId(), value);
                }

                uint32_t colorTemperatureToRGB(int kelvin){
                    int temp = kelvin / 100;
                    int red, green, blue;

                    if( temp <= 66 ){ 
                        red = 255; 
                        green = temp;
                        green = 99.4708025861 * log(green) - 161.1195681661;
                        
                        if( temp <= 19){
                            blue = 0;
                        } else {
                            blue = temp-10;
                            blue = 138.5177312231 * log(blue) - 305.0447927307;
                        }
                    } else {
                        red = temp - 60;
                        red = 329.698727446 * pow(red, -0.1332047592);
                        
                        green = temp - 60;
                        green = 288.1221695283 * pow(green, -0.0755148492 );

                        blue = 255;
                    }

                    return M5Dial.Display.color888(clamp(red,   0, 255), clamp(green, 0, 255), clamp(blue,  0, 255));
                }


                int clamp(int x, int min, int max ) {
                    if(x<min){ return min; }
                    if(x>max){ return max; }
                    return x;
                }


                void showTunableWhiteMenu(M5DialDisplay& display, uint16_t currentValue){
                    LovyanGFX* gfx = display.getGfx();

                    uint16_t height = gfx->height();
                    uint16_t width  = gfx->width();

                    uint16_t ypos = getDisplayPositionY(currentValue);

                    gfx->setTextColor(MAROON);
                    gfx->setTextDatum(middle_center);

                    gfx->startWrite();                      // Secure SPI bus

                    gfx->fillRect(0, 0, width, height, colorTemperatureToRGB(currentValue) );

                    gfx->drawLine(0, ypos, width, ypos, RED );

                    display.setFontsize(3);
                    gfx->drawString((String(currentValue) + "K").c_str(),
                                    width / 2,
                                    height / 2 - 30);

                    display.setFontsize(1);
                    gfx->drawString(this->device.getName().c_str(),
                                    width / 2,
                                    height / 2 + 20);
                    gfx->drawString("White",
                                    width / 2,
                                    height / 2 + 50);  

                    gfx->endWrite();                      // Release SPI bus
                }

            public:
                HaDeviceModeLightTunableWhite(HaDevice& device) : HaDeviceMode(device){
                    this->value = 2000;
                    this->minValue = 2000;
                    this->maxValue = 6500;
                    this->endlessRotaryValue = false;
                }

                void refreshDisplay(M5DialDisplay& display, bool init) override {
                    ESP_LOGD("DISPLAY", "refresh Display: Tunable White-Modus");
                    this->showTunableWhiteMenu(display, getValue());
                }

                void registerHAListener() override {
                    std::string attrName = "color_temp_kelvin";
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                attrName, 
                                [this](const std::string &state) {

                        if(this->isValueModified()){
                            return;
                        }

                        auto val = parse_number<int>(state);
                        if (!val.has_value()) {
                            this->setReceivedValue(0);
                            ESP_LOGD("HA_API", "No Kelvin value in %s for %s", state.c_str(), this->device.getEntityId().c_str());
                        } else {
                            this->setReceivedValue(round((float)val.value()));
                            ESP_LOGI("HA_API", "Got Kelvin value %i for %s", val.value(), this->device.getEntityId().c_str());
                        }
                    });
                }
                
                bool onTouch(M5DialDisplay& display, uint16_t x, uint16_t y) override {
                    return defaultOnTouch(display, x, y);        
                }

                bool onRotary(M5DialDisplay& display, const char * direction) override {
                    return defaultOnRotary(display, direction);
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