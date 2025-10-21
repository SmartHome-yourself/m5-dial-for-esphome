#pragma once
#include "ha_device_mode_percentage.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModeNumberValue: public esphome::shys_m5_dial::HaDeviceModePercentage {
            protected:
                void sendValueToHomeAssistant(int value) override {
                    haApi.setNumberValue(this->device.getEntityId(), value);
                }

            public:
                HaDeviceModeNumberValue(HaDevice& device) : HaDeviceModePercentage(device){
                    this->setLabel("Value");
                    this->setUnit("");
                    this->setIcon(COUNTER_IMG, 4900);
                }

                void registerHAListener() {
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                optional<std::string>(), 
                                [this](const std::string &state) {
                        if(this->isValueModified()){
                            return;
                        }
                        auto val = parse_number<float>(state);
                        if (!val.has_value()) {
                            this->setReceivedValue(0);
                            ESP_LOGD("HA_API", "No value in %s for %s", state.c_str(), this->device.getEntityId().c_str());
                        } else {
                            this->setReceivedValue(val.value());
                            ESP_LOGI("HA_API", "Got value %f for %s", val.value(), this->device.getEntityId().c_str());
                        }
                    });

                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                optional<std::string>("min"), 
                                [this](const std::string &state) {
                        if(this->isValueModified()){
                            return;
                        }
                        auto val = parse_number<float>(state);
                        if (!val.has_value()) {
                            this->setMinValue(0);
                            ESP_LOGD("HA_API", "No min-value in %s for %s", state.c_str(), this->device.getEntityId().c_str());
                        } else {
                            this->setMinValue(val.value());
                            ESP_LOGI("HA_API", "Got min-value %f for %s", val.value(), this->device.getEntityId().c_str());
                        }
                    });
                    
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                optional<std::string>("max"), 
                                [this](const std::string &state) {
                        if(this->isValueModified()){
                            return;
                        }
                        auto val = parse_number<float>(state);
                        if (!val.has_value()) {
                            this->setMaxValue(0);
                            ESP_LOGD("HA_API", "No max-value in %s for %s", state.c_str(), this->device.getEntityId().c_str());
                        } else {
                            this->setMaxValue(val.value());
                            ESP_LOGI("HA_API", "Got max-value %f for %s", val.value(), this->device.getEntityId().c_str());
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