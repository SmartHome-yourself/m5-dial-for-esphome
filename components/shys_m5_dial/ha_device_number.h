#pragma once
#include "ha_device.h"
#include "ha_device_mode_number_value.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceNumber: public esphome::shys_m5_dial::HaDevice {
            protected:
                HaDeviceModeNumberValue*   modeValue   = new HaDeviceModeNumberValue(*this);

            public:
                HaDeviceNumber(const std::string& entity_id, const std::string& name, const std::string& modes) : HaDevice(entity_id, name, modes) {}

                void init() override {
                    ESP_LOGD("HA_DEVICE", "Init Number: %s", this->getEntityId().c_str());

                    this->addMode(modeValue);

                    if (this->modeConfig["value_mode"].is<JsonObject>()) {
                        JsonObject value = this->modeConfig["value_mode"];

                        if (value["rotary_step_width"].is<int>()) {
                            modeValue->setRotaryStepWidth(value["rotary_step_width"].as<int>());
                        }
                        if (value["unit"].is<std::string>()) {
                            modeValue->setUnit(value["unit"]);
                        }                        
                    }
                }

        };

    }
}