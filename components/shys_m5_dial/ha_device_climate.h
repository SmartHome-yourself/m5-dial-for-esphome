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

                        if (temp_mode["rotary_step_width"].is<float>()) {
                            // Keep step as actual value (0.5), scaling happens in raiseCurrentValue/reduceCurrentValue
                            modeTemp->setRotaryStepWidth(temp_mode["rotary_step_width"].as<float>());
                        } else if (temp_mode["rotary_step_width"].is<int>()) {
                            // Keep step as actual value (1), scaling happens in raiseCurrentValue/reduceCurrentValue
                            modeTemp->setRotaryStepWidth((float)temp_mode["rotary_step_width"].as<int>());
                        }


                        if (temp_mode["min_temperature"].is<int>()) {
                            // Scale min temperature by 10 to support fractional steps
                            modeTemp->setMinValue(temp_mode["min_temperature"].as<int>() * 10);
                        }

                        if (temp_mode["max_temperature"].is<int>()) {
                            // Scale max temperature by 10 to support fractional steps
                            modeTemp->setMaxValue(temp_mode["max_temperature"].as<int>() * 10);
                        }

                        // Parse and apply color configuration
                        if (temp_mode["colors"].is<JsonObject>()) {
                            JsonObject colors = temp_mode["colors"];

                            ESP_LOGI("HA_DEVICE", "Applying color configuration for climate: %s", this->getEntityId().c_str());

                            modeTemp->setColors(
                                colors["screen_bg"].as<std::string>(),
                                colors["arc_base"].as<std::string>(),
                                colors["arc_heating_active"].as<std::string>(),
                                colors["arc_heating_idle"].as<std::string>(),
                                colors["arc_cooling"].as<std::string>(),
                                colors["arc_rest"].as<std::string>(),
                                colors["current_temp_bg"].as<std::string>(),
                                colors["current_temp_outline"].as<std::string>(),
                                colors["current_temp_text"].as<std::string>(),
                                colors["setpoint_bg"].as<std::string>(),
                                colors["setpoint_outline"].as<std::string>(),
                                colors["setpoint_text"].as<std::string>(),
                                colors["current_temp_marker"].as<std::string>(),
                                colors["setpoint_marker"].as<std::string>(),
                                colors["badge_heating_bg"].as<std::string>(),
                                colors["badge_cooling_bg"].as<std::string>(),
                                colors["badge_idle_bg"].as<std::string>(),
                                colors["badge_off_bg"].as<std::string>(),
                                colors["badge_heating_text"].as<std::string>(),
                                colors["badge_cooling_text"].as<std::string>(),
                                colors["badge_idle_text"].as<std::string>(),
                                colors["badge_off_text"].as<std::string>(),
                                colors["bottom_bar_bg"].as<std::string>(),
                                colors["bottom_bar_text"].as<std::string>()
                            );
                        }
                    }

                    if (this->modeConfig["fan_mode"].is<JsonObject>()) {
                        JsonObject fan_mode = this->modeConfig["fan_mode"];

                        modeFan->loadFanModes(this->modeConfig);
                        this->addMode(modeFan);                        

                        if (fan_mode["rotary_step_width"].is<float>()) {
                            modeFan->setRotaryStepWidth(fan_mode["rotary_step_width"].as<float>());
                        } else if (fan_mode["rotary_step_width"].is<int>()) {
                            modeFan->setRotaryStepWidth((float)fan_mode["rotary_step_width"].as<int>());
                        }
                    }
                }

        };

    }
}