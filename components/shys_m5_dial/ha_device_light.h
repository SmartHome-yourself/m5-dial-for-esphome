#pragma once
#include "ha_device.h"
#include "ha_device_mode_light_on_off.h"
#include "ha_device_mode_light_brightness.h"
#include "ha_device_mode_light_color.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceLight: public esphome::shys_m5_dial::HaDevice {
            protected:
                HaDeviceModeLightOnOff*       modeOnOff      = new HaDeviceModeLightOnOff();
                HaDeviceModeLightBrightness*  modeBrightness = new HaDeviceModeLightBrightness();
                HaDeviceModeLightColor*       modeColor      = new HaDeviceModeLightColor();

                bool dimm_enabled = true;
                bool rgb_enabled = true;
// TODO:                bool tunable_white = true;


                void addOnOffListener(){
                    std::string attrName = "";
                    api::global_api_server->subscribe_home_assistant_state(
                                this->entity.c_str(),
                                attrName, 
                                [this](const std::string &state) {

                        int newState = strcmp("on", state.c_str())?1:0;

                        modeOnOff->setReceivedValue(newState);
                        ESP_LOGI("HA_API", "Got value %s for %s", state.c_str(), this->entity.c_str());
                    });
                }

                void addBrightnessListener(){
                    std::string attrName = "brightness";
                    api::global_api_server->subscribe_home_assistant_state(
                                this->entity.c_str(),
                                attrName, 
                                [this](const std::string &state) {

                        auto val = parse_number<int>(state);
                        if (!val.has_value()) {
                            modeBrightness->setReceivedValue(0);
                            ESP_LOGD("HA_API", "No Brightness value in %s for %s", state.c_str(), this->entity.c_str());
                        } else {
                            modeBrightness->setReceivedValue(round((float)val.value()*100/255));
                            ESP_LOGI("HA_API", "Got Brightness value %i for %s", val.value(), this->entity.c_str());
                        }
                    });
                }

                void addColorListener(){
                    std::string attrName = "hs_color";
                    api::global_api_server->subscribe_home_assistant_state(
                                this->entity.c_str(),
                                attrName, 
                                [this](const std::string &state) {

                            std::string colorString = "";          
                            std::string::size_type pos = state.find(',');
                            
                            if (pos != std::string::npos) {
                                colorString = state.substr(1, pos-1);
                            }
                            ESP_LOGD("HA_API", "HS_Color value %s for %s", colorString.c_str(), this->entity.c_str());

                            auto val = parse_number<float>(colorString.c_str());
                            if (!val.has_value()) {
                                modeColor->setReceivedValue(0);
                                ESP_LOGD("HA_API", "No Color value in %s for %s", colorString.c_str(), this->entity.c_str());
                            } else {
                                modeColor->setReceivedValue(val.value());
                                ESP_LOGI("HA_API", "Got Color value %f for %s", val.value(), this->entity.c_str());
                            }
                    });
                }

            public:
                void init(String entity_id, String name) override {
                    this->initDefault(entity_id, name);

                    if(dimm_enabled){
                        this->addMode(modeBrightness);
                        this->addBrightnessListener();
                    } else {
                        this->addMode(modeOnOff);
                        this->addOnOffListener();
                    }
                    if(rgb_enabled){
                        this->addMode(modeColor);
                        this->addColorListener();
                    }
                }

        };

    }
}