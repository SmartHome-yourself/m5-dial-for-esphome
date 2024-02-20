#pragma once
#include "ha_device_mode_percentage.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModeCoverPosition: public esphome::shys_m5_dial::HaDeviceModePercentage {
            protected:
                void sendValueToHomeAssistant(int value) override {
                    haApi.setCoverPosition(this->device.getEntityId(), value);
                }

            public:
                HaDeviceModeCoverPosition(HaDevice& device) : HaDeviceModePercentage(device){}

                void registerHAListener() {
                    std::string attrName = "current_position";
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                attrName, 
                                [this](const std::string &state) {
                        if(this->isValueModified()){
                            return;
                        }
                        auto val = parse_number<int>(state);
                        if (!val.has_value()) {
                            this->setReceivedValue(0);
                            ESP_LOGD("HA_API", "No Position value in %s for %s", state.c_str(), this->device.getEntityId().c_str());
                        } else {
                            this->setReceivedValue(val.value());
                            ESP_LOGI("HA_API", "Got Position value %i for %s", val.value(), this->device.getEntityId().c_str());
                        }
                    });
                };

                bool onButton(M5DialDisplay& display, const char * clickType) override {
                    if (strcmp(clickType, BUTTON_SHORT)==0){
                        if(this->getValue() > 50){
                            this->setValue(0);
                        } else {
                            this->setValue(100);
                        }
                        
                        return true;
                    } 
                    return false;
                }
        };
    }
}