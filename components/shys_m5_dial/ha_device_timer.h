#pragma once
#include "ha_device.h"
#include "ha_device_mode_number_value.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceTimer: public esphome::shys_m5_dial::HaDevice {
            protected:
                HaDeviceModeTimerHandling*   modeTimer   = new HaDeviceModeTimerHandling(*this);


            public:
                HaDeviceTimer(const std::string& entity_id, const std::string& name, const std::string& modes) : HaDevice(entity_id, name, modes) {}

                void init() override {
                    ESP_LOGD("HA_DEVICE", "Init Timer: %s", this->getEntityId().c_str());

                    this->addMode(modeTimer);

                    if (this->modeConfig["timer_mode"].is<JsonObject>()) {
                        JsonObject value = this->modeConfig["timer_mode"];
                        if (value["rotary_step_width"].is<int>()) {
                            modeTimer->setRotaryStepWidth(value["rotary_step_width"].as<int>());
                        }                     
                    }
                }

                void setTimeComponent(esphome::time::RealTimeClock* time_comp) {
                    this->modeTimer->setLocalTime(time_comp);
                }
                
        };

    }
}