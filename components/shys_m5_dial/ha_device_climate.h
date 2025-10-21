#pragma once
#include "ha_device.h"
#include "ha_device_mode_climate_temperature.h"
#include "ha_device_mode_climate_fan.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceClimate: public esphome::shys_m5_dial::HaDevice {
            protected:
                HaDeviceModeClimateTemperature*             modeTemp           = new HaDeviceModeClimateTemperature(*this);
                HaDeviceModeClimateFan*                     modeFan            = new HaDeviceModeClimateFan(*this);

            public:
                HaDeviceClimate(const std::string& entity_id, const std::string& name, const std::string& modes) : HaDevice(entity_id, name, modes) {}

                void init() override {
                    ESP_LOGD("HA_DEVICE", "Init Climate: %s", this->getEntityId().c_str());

                    this->addMode(modeTemp);

                    if (this->modeConfig["temp_mode"].is<JsonObject>()) {
                        JsonObject temp_mode = this->modeConfig["temp_mode"];

                        if (temp_mode["rotary_step_width"].is<int>()) {
                            modeTemp->setRotaryStepWidth(temp_mode["rotary_step_width"].as<int>());
                        }


                        if (temp_mode["min_temperature"].is<int>()) {
                            modeTemp->setMinValue(temp_mode["min_temperature"].as<int>());
                        }

                        if (temp_mode["max_temperature"].is<int>()) {
                            modeTemp->setMaxValue(temp_mode["max_temperature"].as<int>());
                        }                        
                    }

                    if (this->modeConfig["fan_mode"].is<JsonObject>()) {
                        JsonObject fan_mode = this->modeConfig["fan_mode"];

                        modeFan->loadFanModes(this->modeConfig);
                        this->addMode(modeFan);                        

                        if (fan_mode["rotary_step_width"].is<int>()) {
                            modeFan->setRotaryStepWidth(fan_mode["rotary_step_width"].as<int>());
                        }
                    }
                }

        };

    }
}