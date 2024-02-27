#pragma once
#include "ha_device.h"
#include "ha_device_mode_switch_on_off.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceSwitch: public esphome::shys_m5_dial::HaDevice {
            protected:
                HaDeviceModeSwitchOnOff*   modeOnOff   = new HaDeviceModeSwitchOnOff(*this);

            public:
                HaDeviceSwitch(const std::string& entity_id, const std::string& name, const std::string& modes) : HaDevice(entity_id, name, modes) {}

                void init() override {
                    ESP_LOGD("HA_DEVICE", "Init Switch: %s", this->getEntityId().c_str());

                    this->addMode(modeOnOff);
                }

        };

    }
}