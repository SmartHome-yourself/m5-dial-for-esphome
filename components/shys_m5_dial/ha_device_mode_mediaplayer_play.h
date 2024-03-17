#pragma once

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModeMediaPlayerPlay: public esphome::shys_m5_dial::HaDeviceMode {
            protected:
                std::string player_state = "";

                std::string media_title = "";
                std::string media_artist = "";
                std::string media_album_name = "";

                uint16_t media_duration = 0;
                uint16_t media_position = 0;
                
                unsigned long lastRefresh = 0;

                void sendValueToHomeAssistant(int value) override {
                    haApi.setMediaPlayerVolume(this->device.getEntityId(), value);
                }

                float getMediaPositionPct(){
                    float result = 0;

                    if(strcmp(this->player_state.c_str(), "playing")==0){
                        if(this->media_duration > 0 && this->media_position <= this->media_duration){
                            this->media_position/this->media_duration*100;
                        }
                    }

                    return result;
                }

                void showPlayMenu(M5DialDisplay& display){
                    LovyanGFX* gfx = display.getGfx();
                    uint16_t height = gfx->height();
                    uint16_t width  = gfx->width();

                    gfx->setTextColor(MAROON);
                    gfx->setTextDatum(middle_center);

                    gfx->startWrite();                      // Secure SPI bus

                    gfx->fillRect(0, 0, width, height, YELLOW);

                    // Round Volume Bar
                    gfx->fillArc(width / 2,
                                 height / 2,
                                 115,
                                 100,
                                 150,
                                 getValue()==0?150:(((float)240 / 100) * getValue()) + 150,
                                 RED
                                );

                    gfx->fillArc(width / 2,
                                 height / 2,
                                 115,
                                 100,
                                 getValue()==0?150:(((float)240 / 100) * getValue()) + 150,
                                 390,
                                 ORANGE
                                );

                    // Percent
                    display.setFontsize(2);
                    gfx->drawString((String(getValue()) + "%").c_str(),
                                    width / 2,
                                    height / 2 - 45);
                    
                    // Device Name
                    display.setFontsize(1);
                    gfx->drawString(this->device.getName().c_str(),
                                    width / 2,
                                    height / 2 - 80);
                                    

                    
                    // Media-Artist/Title
                    display.setFontsize(.7);
                    bool displayTitle = ((millis() / 5000) % 2 == 1);
                    if(displayTitle){
                        gfx->drawString(this->media_title.c_str(),
                                        width / 2,
                                        height / 2 + 70);                          
                    } else {
                        gfx->drawString(this->media_artist.c_str(),
                                        width / 2,
                                        height / 2 + 70);
                    }

                    // Position Bar
                    gfx->fillRect(width/2-40, height/2+90, 80, 5, ORANGE);
                    gfx->fillRect(width/2-40, height/2+90, ((float)80 /100* getMediaPositionPct()), 5, RED);



                    if(strcmp(this->player_state.c_str(), "playing") == 0){
                        // Pause Button
                        gfx->fillRect(width/2-20, height/2-20, 15, 40, RED);
                        gfx->fillRect(width/2+5, height/2-20, 15, 40, RED);
                    } else {
                        // Play Button
                        M5Dial.Display.fillTriangle(width/2-13, height/2-20, width/2-13, height/2+20, width/2+23, height/2, RED);
                    }

                    // FWD
                    M5Dial.Display.fillTriangle(width/2+50, height/2-20, width/2+50, height/2+20, width/2+75, height/2, RED);
                    M5Dial.Display.fillTriangle(width/2+65, height/2-20, width/2+65, height/2+20, width/2+95, height/2, RED);

                    // PREV
                    M5Dial.Display.fillTriangle(width/2-50, height/2-20, width/2-50, height/2+20, width/2-75, height/2, RED);
                    M5Dial.Display.fillTriangle(width/2-65, height/2-20, width/2-65, height/2+20, width/2-95, height/2, RED);


                    gfx->endWrite();                      // Release SPI bus
                }

            public:
                HaDeviceModeMediaPlayerPlay(HaDevice& device) : HaDeviceMode(device){}

                void refreshDisplay(M5DialDisplay& display, bool init) override {
                    this->showPlayMenu(display);
                    ESP_LOGD("DISPLAY", "Play-Modus");

                    this->displayRefreshNeeded = false;
                }

                void registerHAListener() override {
                    std::string volAttrName = "volume_level";
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                volAttrName, 
                                [this](const std::string &state) {
                        
                        if(this->isValueModified()){
                            return;
                        }

                        ESP_LOGI("HA_API", "Got Volume value %s for %s", state.c_str(), this->device.getEntityId().c_str());
                        auto val = parse_number<float>(state);
                        ESP_LOGI("HA_API", "Parsed Volume value %f for %s", val.value(), this->device.getEntityId().c_str());

                        if (!val.has_value()) {
                            this->setReceivedValue(0);
                            ESP_LOGD("HA_API", "No Volume value in %s for %s", state.c_str(), this->device.getEntityId().c_str());
                        } else {
                            this->setReceivedValue(val.value()*100);
                            ESP_LOGI("HA_API", "Set Volume value %f for %s", val.value(), this->device.getEntityId().c_str());

                        }
                    });


                    std::string stateAttrName = "";
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                stateAttrName, 
                                [this](const std::string &state) {
                        
                        if(this->isValueModified()){
                            return;
                        }

                        this->player_state = state;

                        this->displayRefreshNeeded = true;
                        ESP_LOGI("HA_API", "Got State %s for %s", state.c_str(), this->device.getEntityId().c_str());
                    });

                    std::string titleAttrName = "media_title";
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                titleAttrName, 
                                [this](const std::string &state) {
                        
                        if(this->isValueModified()){
                            return;
                        }

                        this->media_title = state;

                        this->displayRefreshNeeded = true;
                        ESP_LOGI("HA_API", "Got Title %s for %s", state.c_str(), this->device.getEntityId().c_str());
                    });

                    std::string artistAttrName = "media_artist";
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                artistAttrName, 
                                [this](const std::string &state) {
                        
                        if(this->isValueModified()){
                            return;
                        }

                        this->media_artist = state;

                        this->displayRefreshNeeded = true;
                        ESP_LOGI("HA_API", "Got Artist %s for %s", state.c_str(), this->device.getEntityId().c_str());
                    });

                    std::string albumAttrName = "media_album_name";
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                albumAttrName, 
                                [this](const std::string &state) {
                        
                        if(this->isValueModified()){
                            return;
                        }

                        this->media_album_name = state;

                        this->displayRefreshNeeded = true;
                        ESP_LOGI("HA_API", "Got Album %s for %s", state.c_str(), this->device.getEntityId().c_str());
                    });

                    std::string durationAttrName = "media_duration";
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                durationAttrName, 
                                [this](const std::string &state) {
                        
                        if(this->isValueModified()){
                            return;
                        }

                        auto val = parse_number<int>(state);
                        if (!val.has_value()) {
                            this->media_duration = 0;
                            ESP_LOGD("HA_API", "No Media-Duration in %s for %s", state.c_str(), this->device.getEntityId().c_str());
                        } else {
                            this->media_duration = (int)val.value();
                            ESP_LOGI("HA_API", "Got Media-Duration %i for %s", val.value(), this->device.getEntityId().c_str());
                        }
                    });

                    std::string positionAttrName = "media_position";
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                positionAttrName, 
                                [this](const std::string &state) {
                        
                        if(this->isValueModified()){
                            return;
                        }

                        auto val = parse_number<int>(state);
                        if (!val.has_value()) {
                            this->media_position = 0;
                            ESP_LOGD("HA_API", "No Media-Position value in %s for %s", state.c_str(), this->device.getEntityId().c_str());
                        } else {
                            this->media_position = (int)val.value();
                            this->displayRefreshNeeded = true;
                            ESP_LOGI("HA_API", "Got Media-Position value %i for %s", val.value(), this->device.getEntityId().c_str());
                        }
                    });

                }

                bool onTouch(M5DialDisplay& display, uint16_t x, uint16_t y) override {
                    ESP_LOGI("TOUCH", "Touch %i / %i for %s", x, y, this->device.getEntityId().c_str());

                    LovyanGFX* gfx = display.getGfx();
                    uint16_t height = gfx->height();
                    uint16_t width  = gfx->width();

                    uint16_t minHeight = height/2-30;
                    uint16_t maxHeight = height/2+30;

                    uint16_t minPrevX = width/2-110;
                    uint16_t maxPrevX = width/2-50;

                    uint16_t minPlayX = width/2-30;
                    uint16_t maxPlayX = width/2+30;

                    uint16_t minNextX = width/2+50;
                    uint16_t maxNextX = width/2+110;

                    if(y > minHeight && y < maxHeight){
                        if(x>minPrevX && x<maxPrevX){
                            M5Dial.Speaker.tone(5000, 20);
                            haApi.setPreviousTrackOnMediaPlayer(this->device.getEntityId());
                            return true;

                        } else if(x>minPlayX && x<maxPlayX){
                            M5Dial.Speaker.tone(5000, 20);
                            haApi.playPauseMediaPlayer(this->device.getEntityId());
                            return true;

                        } else if(x>minNextX && x<maxNextX){
                            M5Dial.Speaker.tone(5000, 20);
                            haApi.setNextTrackOnMediaPlayer(this->device.getEntityId());
                            return true;
                        } 
                    }

                    return false;
                }

                bool onRotary(M5DialDisplay& display, const char * direction) override {
                    this->defaultOnRotary(display, direction);
                    return true;
                }

                bool onButton(M5DialDisplay& display, const char * clickType) override {
                    if (strcmp(clickType, BUTTON_SHORT)==0){
                        haApi.playPauseMediaPlayer(this->device.getEntityId());
                        return true;
                    }
                    return false;
                }

                void onLoop() override {
                    if(strcmp(this->player_state.c_str(), "playing")==0){
                        bool timebasedRefresh = (this->lastRefresh + 2000) < esphome::millis();
                        if(timebasedRefresh){
                            this->lastRefresh = esphome::millis();

                            haApi.updateEntity(this->device.getEntityId().c_str());
                        }
                    }
                }

        };
    }
}