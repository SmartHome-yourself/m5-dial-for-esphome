#pragma once
#include "ha_device.h"
#include "ha_device_mode_cover_position.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceCover: public esphome::shys_m5_dial::HaDevice {
            protected:
                HaDeviceModeCoverPosition*   modePosition   = new HaDeviceModeCoverPosition(*this);

            public:
                HaDeviceCover(const std::string& entity_id, const std::string& name, const std::string& modes) : HaDevice(entity_id, name, modes) {}

                void init() override {
                    ESP_LOGD("HA_DEVICE", "Init Cover: %s", this->getEntityId().c_str());

                    this->addMode(modePosition);

                    if (this->modeConfig["position_mode"].is<JsonObject>()) {
                        JsonObject position_mode = this->modeConfig["position_mode"];

                        if (position_mode["rotary_step_width"].is<float>()) {
                            modePosition->setRotaryStepWidth(position_mode["rotary_step_width"].as<float>());
                        } else if (position_mode["rotary_step_width"].is<int>()) {
                            modePosition->setRotaryStepWidth((float)position_mode["rotary_step_width"].as<int>());
                        }
                    }
                }

        };

    }
}