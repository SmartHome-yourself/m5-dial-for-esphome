#pragma once
#include "ha_device.h"
#include "ha_device_mode_light_on_off.h"
#include "ha_device_mode_light_brightness.h"
#include "ha_device_mode_light_color.h"
#include "ha_device_mode_light_tunable_white.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceLight: public esphome::shys_m5_dial::HaDevice {
            protected:
                HaDeviceModeLightOnOff*             modeOnOff           = new HaDeviceModeLightOnOff(*this);
                HaDeviceModeLightBrightness*        modeBrightness      = new HaDeviceModeLightBrightness(*this);
                HaDeviceModeLightColor*             modeColor           = new HaDeviceModeLightColor(*this);
                HaDeviceModeLightTunableWhite*      modeTunableWhite    = new HaDeviceModeLightTunableWhite(*this);

                bool brightnessModeActive = false;
                bool whiteModeActive = false;

                bool onTouch(M5DialDisplay& display, uint16_t x, uint16_t y) override {
                    if((getCurrentMode() == modeColor) && getRadiusFromCoord(display,x,y)<70){
                        changeToWhiteMode();
                        return true;
                    }

                    ESP_LOGD("DEVICE", "HaDevice.onTouch: %i/%i", x, y);
                    return getCurrentMode()->onTouch(display, x, y);
                }

            public:
                HaDeviceLight(const std::string& entity_id, const std::string& name, const std::string& modes) : HaDevice(entity_id, name, modes) {}

                void init() override {
                    ESP_LOGD("HA_DEVICE", "Init Light: %s", this->getEntityId().c_str());

                    JsonObject dimm_mode;
                    if (this->modeConfig["dimm_mode"].is<JsonObject>()) {
                        dimm_mode = this->modeConfig["dimm_mode"];
                        if (dimm_mode["enable"].is<bool>() && dimm_mode["enable"].as<bool>()) {
                            brightnessModeActive = true;
                        }
                    }

                    if(brightnessModeActive){
                        float stepWidth = dimm_mode["rotary_step_width"].is<float>() ?
                                          dimm_mode["rotary_step_width"].as<float>() :
                                          (float)dimm_mode["rotary_step_width"].as<int>();
                        ESP_LOGD("HA_DEVICE", "Dimm-Mode enabled (steps: %.2f)", stepWidth);

                        this->addMode(modeBrightness);

                        if (dimm_mode["rotary_step_width"].is<float>()) {
                            modeBrightness->setRotaryStepWidth(dimm_mode["rotary_step_width"].as<float>());
                        } else if (dimm_mode["rotary_step_width"].is<int>()) {
                            modeBrightness->setRotaryStepWidth((float)dimm_mode["rotary_step_width"].as<int>());
                        }

                        if (dimm_mode["min_brightness"].is<int>()) {
                            modeBrightness->setMinValue(dimm_mode["min_brightness"].as<int>());
                        }

                        if (dimm_mode["max_brightness"].is<int>()) {
                            modeBrightness->setMaxValue(dimm_mode["max_brightness"].as<int>());
                        }

                    } else {
                        this->addMode(modeOnOff);
                    }


                    if (this->modeConfig["rgb_mode"].is<JsonObject>()) {
                        JsonObject rgb_mode = this->modeConfig["rgb_mode"];

                        if (rgb_mode["enable"].is<bool>() && rgb_mode["enable"].as<bool>()) {
                            float stepWidth = rgb_mode["rotary_step_width"].is<float>() ?
                                              rgb_mode["rotary_step_width"].as<float>() :
                                              (float)rgb_mode["rotary_step_width"].as<int>();
                            ESP_LOGD("HA_DEVICE", "Color-Mode enabled (steps: %.2f)", stepWidth);

                            this->addMode(modeColor);

                            if (rgb_mode["rotary_step_width"].is<float>()) {
                                modeColor->setRotaryStepWidth(rgb_mode["rotary_step_width"].as<float>());
                            } else if (rgb_mode["rotary_step_width"].is<int>()) {
                                modeColor->setRotaryStepWidth((float)rgb_mode["rotary_step_width"].as<int>());
                            }
                        }
                    }


                    if (this->modeConfig["white_mode"].is<JsonObject>()) {
                        JsonObject white_mode = this->modeConfig["white_mode"];

                        if (white_mode["enable"].is<bool>() && white_mode["enable"].as<bool>()) {
                            whiteModeActive = true;
                            float stepWidth = white_mode["rotary_step_width"].is<float>() ?
                                              white_mode["rotary_step_width"].as<float>() :
                                              (float)white_mode["rotary_step_width"].as<int>();
                            ESP_LOGD("HA_DEVICE", "White-Mode enabled (steps: %.2f)", stepWidth);

                            this->addMode(modeTunableWhite);

                            if (white_mode["rotary_step_width"].is<float>()) {
                                modeTunableWhite->setRotaryStepWidth(white_mode["rotary_step_width"].as<float>());
                            } else if (white_mode["rotary_step_width"].is<int>()) {
                                modeTunableWhite->setRotaryStepWidth((float)white_mode["rotary_step_width"].as<int>());
                            }
                            if (white_mode["min_kelvin"].is<int>()) {
                                modeTunableWhite->setMinValue(white_mode["min_kelvin"].as<int>());
                            }
                            if (white_mode["max_kelvin"].is<int>()) {
                                modeTunableWhite->setMaxValue(white_mode["max_kelvin"].as<int>());
                            }
                        }
                    }

                }

                void changeToWhiteMode(){
                    if(whiteModeActive){
                        setCurrentModeIndex(getModeIndex(modeTunableWhite));
                        haApi.turnLightOnWhite(getEntityId(), modeTunableWhite->getValue());

                    } else {
                        if(brightnessModeActive){
                            setCurrentModeIndex(getModeIndex(modeBrightness));
                            haApi.turnLightOn(getEntityId(), modeBrightness->getValue(), M5Dial.Display.color888(255,255,255));

                        } else {
                            setCurrentModeIndex(getModeIndex(modeOnOff));
                            haApi.turnLightOn(getEntityId(), -1, M5Dial.Display.color888(255,255,255));
                        }
                    }
                }


        };

    }
}