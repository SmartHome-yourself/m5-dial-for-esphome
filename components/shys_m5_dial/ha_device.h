#pragma once
#include "globals.h"
#include "M5Dial.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDevice {
            protected:
                int index;
                String entity;
                String name;
                uint16_t currentMode;
                uint16_t maxRotary = 100;
                
                int lastValue = 0;
                float color = 0;
                bool rgb_enabled = false;
                bool dimm_enabled = false;

                bool startsWith(const char *pre, const char *str){
                    return strncmp(pre, str, strlen(pre)) == 0;
                }

                const uint16_t getDefaultMode(){
                    ESP_LOGD("DEVICE", "getDefaultMode");
                    if (this->isLight()){
                        return DEVICE_MODE_BRIGHTNESS;
                    } else if(this->isCover()){
                        return DEVICE_MODE_POSITION;
                    } else if(this->isFan()){
                        return DEVICE_MODE_POWER;
                    } 
                    
                    return DEVICE_MODE_POWER;
                }

                std::vector<uint16_t> getModeList(){
                    if (this->isLight()){
                        return LIGHT_MODES;
                    } else if (this->isCover()) {
                        return COVER_MODES;
                    } else if (this->isFan()) {
                        return FAN_MODES;
                    }

                    return {};
                }

                const uint16_t getNextMode() {
                    ESP_LOGD("DEVICE", "getNextMode");

                    std::vector<uint16_t> modeList = getModeList();
                    
                    size_t count = modeList.size();
                    //int count = *(&modeList + 1) - modeList;
                    ESP_LOGD("DEVICE", "Mode Count: %i", count);

                    for (int i=0; i < count; i++){
                        if(currentMode == modeList[i]){
                            if(i < count-1){
                                return modeList[i+1];
                            } else {
                                return modeList[0];
                            }
                        }
                    }
                    
                    return getDefaultMode();
                }

                const uint16_t getPreviousMode(const uint16_t modeList[]){
                    int count = *(&modeList + 1) - modeList;
                    for (int i=0; i < count; i++){
                        if(currentMode == modeList[i]){
                            if(i > 0){
                                return modeList[i-1];
                            } else {
                                return modeList[count-1];
                            }
                        }
                    }
                }

            public:
                String getName(){
                    return this->name;
                }

                String getEntityId(){
                    return this->entity;
                }

                bool isDimmEnabled(){
                    return this->dimm_enabled;
                }

                uint16_t getMaxRotary(){
                    if (getCurrentMode() == DEVICE_MODE_COLOR){
                        return 360;
                    }
                    
                    return 100;
                }

                String getApiAttribute(){
                    if(this->isLight()){
                        return "brightness";
                    } else if(this->isCover()){
                        return "current_position";
                    } 
                    
                    return "";
                }

                String getColorApiAttribute(){
                    if(this->isLight()){
                        return "hs_color"; // Bspl: 235.294, 100
                    } 
                    
                    return "";
                }         

                String getDeviceType(){
                    if(this->isLight()){
                        return "Light";
                    } else if(this->isCover()){
                        return "Cover";
                    } else if(this->isFan()){
                        return "Fan";
                    } else {
                        return "unknown";
                    }
                }


                bool isLight(){
                    return this->startsWith("light.", this->entity.c_str());
                }
                bool isCover(){
                    return this->startsWith("cover.", this->entity.c_str());
                }
                bool isFan(){
                    return this->startsWith("fan.", this->entity.c_str());
                }

                uint16_t getCurrentMode(){
                    return currentMode;
                }

                void nextMode(){
                    ESP_LOGD("DEVICE", "nextMode");

                    currentMode = this->getNextMode();
                    if(this->isLight() 
                       && currentMode == DEVICE_MODE_COLOR 
                       && !rgb_enabled){
                        nextMode();
                    }
                }

                uint16_t getPercentageValue() {
                    if(this->isLight()){
                        return round((float)this->getLastKnownValue() * 100 / 255);
                    }
                    return this->getLastKnownValue();
                }

                void setPercentageValue(uint16_t val){
                    if(this->isLight()){
                        this->lastValue = round((float)val * 255 / 100);
                    } else {
                        this->lastValue = val;
                    }
                }

                int getLastKnownValue() {
                    return this->lastValue;
                }

                float getColor(){
                    return this->color;
                }
                
                void setColor(float c){
                    this->color = c;
                }

                void init(String entity_id, String name, bool rgb_enabled, bool dimm_enabled){
                    this->entity       = entity_id;
                    this->name         = name;
                    this->lastValue    = 50;
                    this->rgb_enabled  = rgb_enabled;
                    this->dimm_enabled = dimm_enabled;

                    ESP_LOGI("DEVICE", "New Device: %s (%s) RGB: %s", name, entity_id, rgb_enabled?"on":"off");

                    std::string n = this->entity.c_str();
                    std::string a = this->getApiAttribute().c_str();

                    api::global_api_server->subscribe_home_assistant_state(
                                n,
                                a, 
                                [this](const std::string &state) {
                        M5Dial.Speaker.tone(3000, 100);

                        auto val = parse_number<int>(state);
                        if (!val.has_value()) {
                            val = 0;
                        } else {
                            ESP_LOGI("HA_API", "Got value %i for %s", *val, this->entity.c_str());
                        }

                        this->lastValue = *val;
                    });

                    if(this->isLight() && rgb_enabled){

                        std::string n = this->entity.c_str();
                        std::string a = this->getColorApiAttribute().c_str();

                        api::global_api_server->subscribe_home_assistant_state(
                                    n,
                                    a, 
                                    [this](const std::string &state) {
                            M5Dial.Speaker.tone(2000, 100);

                            std::string colorString = "";          
                            std::string::size_type pos = state.find(',');
                            if (pos != std::string::npos)
                            {
                                colorString = state.substr(1, pos-1);
                            }
                            ESP_LOGD("HA_API", "HS_Color value %s for %s", colorString.c_str(), this->entity.c_str());

                            auto val = parse_number<float>(colorString.c_str());
                            if (!val.has_value()) {
                                this->color = 0;
                                ESP_LOGD("HA_API", "No Color value in %s for %s", colorString, this->entity.c_str());
                            } else {
                                this->color = val.value();
                                ESP_LOGI("HA_API", "Got Color value %f for %s", this->color, this->entity.c_str());
                            }

                        });
                    }

                    this->currentMode = this->getDefaultMode();
                }


        };


    }
}