#pragma once
//#include "shys_m5_dial.h"
#include "ha_device.h"
#include "ha_device_mode_menu_test.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class ShysM5Dial;

        class HaDeviceMenu: public esphome::shys_m5_dial::HaDevice {
            protected:
                HaDeviceModeMenuTest*   modeMTest   = new HaDeviceModeMenuTest(*this);
                ShysM5Dial* M5DialObj=NULL;

            public:
                HaDeviceMenu(const std::string& entity_id, const std::string& name, const std::string& modes) : HaDevice(entity_id, name, modes) {}

                void init() override {
                    ESP_LOGD("HA_DEVICE", "Init Menu: %s", this->getEntityId().c_str());

                    this->addMode(modeMTest);
                    /**
                     * @todo change this name "speed mode"
                     * 
                     */
                    if (this->modeConfig.containsKey("speed_mode")) {
                        JsonObject speed_mode = this->modeConfig["speed_mode"];

                        if (speed_mode.containsKey("rotary_step_width")) {
                            modeMTest->setRotaryStepWidth(speed_mode["rotary_step_width"].as<int>());
                        }

                        if (speed_mode.containsKey("changeable_direction")) {
                            modeMTest->setChangeableDirection(speed_mode["changeable_direction"].as<bool>());
                        }
                    }
                }

                /**
                 * @brief initialize the pointer to the main ShysM5Dial object
                 * 
                 * @param m5D 
                */
                inline void setM5DialObj(ShysM5Dial * m5D) {
                    this->M5DialObj=m5D;
                    modeMTest->setM5DialObj(m5D);
                }
                

        };

    }
}