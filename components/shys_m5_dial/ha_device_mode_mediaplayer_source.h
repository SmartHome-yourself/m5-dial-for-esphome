#pragma once

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModeMediaPlayerSource: public esphome::shys_m5_dial::HaDeviceMode {
            protected:
                struct MediaContent {
                    std::string name;
                    std::string content_id;
                    std::string content_type;
                };

                std::vector<MediaContent*> mediaContents = {};

                void sendValueToHomeAssistant(int value) override {
                    // not choose Playlist on change value.
                    // selection activated by button pressed
                }

                void showSourceSelection(M5DialDisplay& display){
                    LovyanGFX* gfx = display.getGfx();
                    uint16_t height = gfx->height();
                    uint16_t width  = gfx->width();

                    gfx->setTextColor(MAROON);
                    gfx->setTextDatum(middle_center);

                    gfx->startWrite();                      // Secure SPI bus

                    display.clear();

                    // Device Name
                    display.setFontsize(1);
                    gfx->drawString(this->device.getName().c_str(),
                                    width / 2,
                                    height / 2 - 80);

                    if(this->getValue()>0){
                        display.setFontsize(.7);
                        gfx->drawString(this->mediaContents[this->getValue()-1]->name.c_str(),
                                        width / 2,
                                        height / 2-40); 
                    }

                    display.setFontsize(1.5);
                    gfx->drawString(this->mediaContents[this->getValue()]->name.c_str(),
                                    width / 2,
                                    height / 2);

                    if(this->getValue()<this->getMaxValue()){
                        display.setFontsize(.7);
                        gfx->drawString(this->mediaContents[this->getValue()+1]->name.c_str(),
                                        width / 2,
                                        height / 2+40); 
                    }

                    gfx->endWrite();                      // Release SPI bus
                }

            public:
                HaDeviceModeMediaPlayerSource(HaDevice& device) : HaDeviceMode(device){
                    this->setMaxValue(0);
                }

                void refreshDisplay(M5DialDisplay& display, bool init) override {
                    this->showSourceSelection(display);
                    ESP_LOGD("DISPLAY", "Source-Modus");

                    this->displayRefreshNeeded = false;
                }

                void registerHAListener() override {
                }

                bool onRotary(M5DialDisplay& display, const char * direction) override {
                    this->defaultOnRotary(display, direction);
                    return true;
                }

                bool onButton(M5DialDisplay& display, const char * clickType) override {
                    if (strcmp(clickType, BUTTON_SHORT)==0){
                        const std::string content_id   = this->mediaContents[this->getValue()]->content_id.c_str();
                        const std::string content_type = this->mediaContents[this->getValue()]->content_type.c_str();

                        haApi.playMediaOnMediaPlayer(this->device.getEntityId(), content_id, content_type);
                        
                        delay(300);
                        haApi.updateEntity(this->device.getEntityId().c_str());

                        return true;
                    }

                    return false;
                }

               /**
                * 
                */
                void loadMediaContents(JsonObject modeConfig){
                    this->mediaContents = {};

                    if (modeConfig["source_mode"].is<JsonObject>()) {
                        JsonObject sourceModeConfig = modeConfig["source_mode"];

                        if (sourceModeConfig["sources"].is<JsonArray>()) {
                            JsonArray sources = sourceModeConfig["sources"].as<JsonArray>();

                            for(JsonObject source : sources){
                                MediaContent* content = new MediaContent;
                                content->name = source["name"].as<std::string>();
                                content->content_id = source["content_id"].as<std::string>();
                                content->content_type = source["content_type"].as<std::string>();

                                this->mediaContents.push_back(content);
                                ESP_LOGD("MEDIAPLAYER", "Add Source: %s on %s", content->name.c_str(), this->device.getEntityId().c_str());
                            }
                            this->setMaxValue(this->mediaContents.size()-1);
                        }
                    }
                }

        };
    }
}