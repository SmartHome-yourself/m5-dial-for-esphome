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
                    if (this->modeConfig.containsKey("dimm_mode")) {
                        dimm_mode = this->modeConfig["dimm_mode"];
                        if (dimm_mode.containsKey("enable") && dimm_mode["enable"].as<bool>()) {
                            brightnessModeActive = true;
                        }
                    }

                    if(brightnessModeActive){
                        ESP_LOGD("HA_DEVICE", "Dimm-Mode enabled (steps: %i)", dimm_mode["rotary_step_width"].as<int>());
                        
                        this->addMode(modeBrightness);

                        if (dimm_mode.containsKey("rotary_step_width")) {
                            modeBrightness->setRotaryStepWidth(dimm_mode["rotary_step_width"].as<int>());
                        }

                        if (dimm_mode.containsKey("min_brightness")) {
                            modeBrightness->setMinValue(dimm_mode["min_brightness"].as<int>());
                        }

                        if (dimm_mode.containsKey("max_brightness")) {
                            modeBrightness->setMaxValue(dimm_mode["max_brightness"].as<int>());
                        }

                    } else {
                        this->addMode(modeOnOff);
                    }


                    if (this->modeConfig.containsKey("rgb_mode")) {
                        JsonObject rgb_mode = this->modeConfig["rgb_mode"];

                        if (rgb_mode.containsKey("enable") && rgb_mode["enable"].as<bool>()) {
                            ESP_LOGD("HA_DEVICE", "Color-Mode enabled (steps: %i)", rgb_mode["rotary_step_width"].as<int>());

                            this->addMode(modeColor);

                            if (rgb_mode.containsKey("rotary_step_width")) {
                                modeColor->setRotaryStepWidth(rgb_mode["rotary_step_width"].as<int>());
                            }
                        }
                    }


                    if (this->modeConfig.containsKey("white_mode")) {
                        JsonObject white_mode = this->modeConfig["white_mode"];

                        if (white_mode.containsKey("enable") && white_mode["enable"].as<bool>()) {
                            whiteModeActive = true;
                            ESP_LOGD("HA_DEVICE", "White-Mode enabled (steps: %i)", white_mode["rotary_step_width"].as<int>());

                            this->addMode(modeTunableWhite);                            

                            if (white_mode.containsKey("rotary_step_width")) {
                                modeTunableWhite->setRotaryStepWidth(white_mode["rotary_step_width"].as<int>());
                            }
                            if (white_mode.containsKey("min_kelvin")) {
                                modeTunableWhite->setMinValue(white_mode["min_kelvin"].as<int>());
                            }
                            if (white_mode.containsKey("max_kelvin")) {
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