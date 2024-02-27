#pragma once
#include "ha_device.h"
#include "ha_device_mode_fan_speed.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceFan: public esphome::shys_m5_dial::HaDevice {
            protected:
                HaDeviceModeFanSpeed*   modeSpeed   = new HaDeviceModeFanSpeed(*this);

            public:
                HaDeviceFan(const std::string& entity_id, const std::string& name, const std::string& modes) : HaDevice(entity_id, name, modes) {}

                void init() override {
                    ESP_LOGD("HA_DEVICE", "Init Fan: %s", this->getEntityId().c_str());

                    this->addMode(modeSpeed);

                    if (this->modeConfig.containsKey("speed_mode")) {
                        JsonObject speed_mode = this->modeConfig["speed_mode"];

                        if (speed_mode.containsKey("rotary_step_width")) {
                            modeSpeed->setRotaryStepWidth(speed_mode["rotary_step_width"].as<int>());
                        }

                        if (speed_mode.containsKey("changeable_direction")) {
                            modeSpeed->setChangeableDirection(speed_mode["changeable_direction"].as<bool>());
                        }
                    }
                }

        };

    }
}