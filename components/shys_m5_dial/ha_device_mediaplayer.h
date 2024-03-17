#pragma once
#include "ha_device.h"
#include "ha_device_mode_mediaplayer_play.h"
#include "ha_device_mode_mediaplayer_source.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceMediaPlayer: public esphome::shys_m5_dial::HaDevice {
            protected:
                HaDeviceModeMediaPlayerPlay*   modePlay   = new HaDeviceModeMediaPlayerPlay(*this);
                HaDeviceModeMediaPlayerSource* modeSource = new HaDeviceModeMediaPlayerSource(*this);

            public:
                HaDeviceMediaPlayer(const std::string& entity_id, const std::string& name, const std::string& modes) : HaDevice(entity_id, name, modes) {}

                void init() override {
                    ESP_LOGD("HA_DEVICE", "Init Media-Player: %s", this->getEntityId().c_str());

                    this->addMode(modePlay);

                    modeSource->loadMediaContents(this->modeConfig);
                    this->addMode(modeSource);

                    if (this->modeConfig.containsKey("play_mode")) {
                        JsonObject play_mode = this->modeConfig["play_mode"];
                        if (play_mode.containsKey("rotary_step_width")) {
                            modePlay->setRotaryStepWidth(play_mode["rotary_step_width"].as<int>());
                        }
                    }
                    
                    if (this->modeConfig.containsKey("source_mode")) {
                        JsonObject source_mode = this->modeConfig["source_mode"];
                        if (source_mode.containsKey("rotary_step_width")) {
                            modeSource->setRotaryStepWidth(source_mode["rotary_step_width"].as<int>());
                        }
                    }
                }

        };

    }
}