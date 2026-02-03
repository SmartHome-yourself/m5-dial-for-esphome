#pragma once

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModeLightBrightness: public esphome::shys_m5_dial::HaDeviceModePercentage {
            protected:
                void sendValueToHomeAssistant(int value) override {
                    haApi.turnLightOn(this->device.getEntityId(), value);
                }

            public:
                HaDeviceModeLightBrightness(HaDevice& device) : HaDeviceModePercentage(device){
                    this->setLabel("Brightness");
                    this->setIcon(LIGHT_ON_IMG, 4900);
                }

                void refreshDisplay(M5DialDisplay& display, bool init) override {
                    ESP_LOGD("DISPLAY", "refresh Display: Helligkeits-Modus");
                    this->showPercentageMenu(display);
                }

                void registerHAListener() override {
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                optional<std::string>("brightness"), 
                                std::function<void(const std::string&)>([this](const std::string &state) {
                        if(this->isValueModified()){
                            return;
                        }
                        auto val = parse_number<int>(state);
                        if (!val.has_value()) {
                            this->setReceivedValue(0);
                            ESP_LOGD("HA_API", "No Brightness value in %s for %s", state.c_str(), this->device.getEntityId().c_str());
                        } else {
                            this->setReceivedValue(round((float)val.value()*100/255));
                            ESP_LOGI("HA_API", "Got Brightness value %i for %s", val.value(), this->device.getEntityId().c_str());
                        }
                    }));
                }

                bool onTouch(M5DialDisplay& display, uint16_t x, uint16_t y) override {
                    return this->defaultOnTouch(display, x, y);
                }

                bool onRotary(M5DialDisplay& display, const char * direction) override {
                    return this->defaultOnRotary(display, direction);
                }

                bool onButton(M5DialDisplay& display, const char * clickType) override {
                    if (strcmp(clickType, BUTTON_SHORT)==0){
                        haApi.toggleLight(this->device.getEntityId());
                        return true;
                    } 
                    return false;
                }

        };
    }
}
