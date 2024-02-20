#pragma once
#include "ha_device.h"
#include "ha_device_mode_climate_temperature.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceClimate: public esphome::shys_m5_dial::HaDevice {
            protected:
                HaDeviceModeClimateTemperature*             modeTemp           = new HaDeviceModeClimateTemperature(*this);

            public:
                HaDeviceClimate(const std::string& entity_id, const std::string& name, const std::string& modes) : HaDevice(entity_id, name, modes) {}

                void init() override {
                    ESP_LOGD("HA_DEVICE", "Init Climate: %s", this->getEntityId().c_str());

                    if (this->modeConfig.containsKey("temp_mode")) {
                        JsonObject temp_mode = this->modeConfig["temp_mode"];

                        if (temp_mode.containsKey("enable") && temp_mode["enable"].as<bool>()) {
                            ESP_LOGD("HA_DEVICE", "Temperature-Mode enabled (steps: %i)", temp_mode["rotary_step_width"].as<int>());
                            this->addMode(modeTemp);

                            if (temp_mode.containsKey("rotary_step_width")) {
                                modeTemp->setRotaryStepWidth(temp_mode["rotary_step_width"].as<int>());
                            }
                        }
                    }
                }

        };

    }
}