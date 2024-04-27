#pragma once
#include "esphome.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaApi {
            protected:
                std::string colorValue;
                esphome::api::HomeassistantServiceResponse resp;
                esphome::api::HomeassistantServiceMap resp_kv;
                
                bool startsWith(const char *pre, const char *str){
                    return strncmp(pre, str, strlen(pre)) == 0;
                }                
            public:

                void updateEntity(const std::string& entity){
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "homeassistant.update_entity";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);

                    ESP_LOGD("HA_API", "update Entity: %s", entity.c_str());
                }

// ---------------------------------
//              LIGHT
// ---------------------------------
                void turnLightOn(const std::string& entity){
                    turnLightOn(entity, -1, -1);
                }

                void turnLightOn(const std::string& entity, int brightness){
                    turnLightOn(entity, brightness, -1);
                }

                void turnLightOn(const std::string& entity, int brightness, int color){
                    ESP_LOGD("HA_API", "light on values: %s (%i) Color: %i", entity.c_str(), brightness, color);

                    resp.service = "light.turn_on";
                    
                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);
                    
                    if(brightness >= 0){
                        resp_kv.key = "brightness_pct";
                        resp_kv.value = String(brightness).c_str();
                        resp.data.push_back(resp_kv);
                    }

                    if(color >= 0){
                        colorValue = str_sprintf("{{(%d,100)|list}}", color);
                        ESP_LOGD("HA_API", "light on: %s (%i) HS-Color: %s", entity.c_str(), brightness, colorValue.c_str());

                        resp_kv.key = "hs_color";
                        resp_kv.value = colorValue.c_str();
                        resp.data_template.push_back(resp_kv);                    
                        
                        ESP_LOGI("HA_API", "turn light on: %s (%i) HS-Color: %s", entity.c_str(), brightness, colorValue.c_str());
                    } else {
                        ESP_LOGI("HA_API", "turn light on: %s (%i)", entity.c_str(), brightness);
                    }

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                }


                void turnLightOnWhite(const std::string& entity, int kelvin){
                    ESP_LOGD("HA_API", "light on values: %s Kelvin: %i", entity.c_str(),  kelvin);

                    resp.service = "light.turn_on";
                    
                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    if(kelvin >= 0){
                        resp_kv.key = "kelvin";
                        resp_kv.value = String(kelvin).c_str();
                        resp.data_template.push_back(resp_kv);                    
                        
                        ESP_LOGI("HA_API", "turn light on: %s  Kelvin: %s", entity.c_str(), colorValue.c_str());
                    } else {
                        ESP_LOGI("HA_API", "turn light on: %s", entity.c_str());
                    }

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                }

                void turnLightOff(const std::string& entity){
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "light.turn_off";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);

                    ESP_LOGI("HA_API", "turn light off: %s", entity.c_str());
                }

                void toggleLight(const std::string& entity){
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "light.toggle";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                    
                    ESP_LOGI("HA_API", "toggle light: %s", entity.c_str());
                }

// ---------------------------------
//          CLIMATE
// ---------------------------------

                void turnClimateOn(const std::string& entity) {
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "climate.turn_on";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                    
                    ESP_LOGI("HA_API", "turn on climate: %s", entity.c_str());
                }

                void turnClimateOff(const std::string& entity) {
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "climate.turn_off";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                    
                    ESP_LOGI("HA_API", "turn off climate: %s", entity.c_str());
                }

                void setClimateTemperature(const std::string& entity, int temperature) {
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "climate.set_temperature";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);
                    
                    if(temperature >= 0){
                        resp_kv.key = "temperature";
                        resp_kv.value = String(temperature).c_str();
                        resp.data.push_back(resp_kv);
                    }

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                    
                    ESP_LOGI("HA_API", "set temperature: %i for %s", temperature, entity.c_str());
                }



// ---------------------------------
//              COVER
// ---------------------------------
                void setCoverPosition(const std::string& entity, int position) {
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "cover.set_cover_position";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);
                    
                    if(position >= 0){
                        resp_kv.key = "position";
                        resp_kv.value = String(position).c_str();
                        resp.data.push_back(resp_kv);
                    }

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                    
                    ESP_LOGI("HA_API", "cover set position %i for %s", position, entity.c_str());
                }




// ---------------------------------
//             SWITCH
// ---------------------------------
                void turnSwitchOn(const std::string& entity){
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "switch.turn_on";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);

                    ESP_LOGI("HA_API", "switch turn on: %s", entity.c_str());
                }

                void turnSwitchOff(const std::string& entity){
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "switch.turn_off";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);

                    ESP_LOGI("HA_API", "switch turn off: %s", entity.c_str());
                }

                void toggleSwitch(const std::string& entity){
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "switch.toggle";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                    
                    ESP_LOGI("HA_API", "switch toggle: %s", entity.c_str());
                }




// ---------------------------------
//              FAN
// ---------------------------------
                void turnFanOn(const std::string& entity){
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "fan.turn_on";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);

                    ESP_LOGI("HA_API", "fan turn on: %s", entity.c_str());
                }

                void turnFanOff(const std::string& entity){
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "fan.turn_off";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);

                    ESP_LOGI("HA_API", "fan turn off: %s", entity.c_str());
                }

                void toggleFan(const std::string& entity){
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "fan.toggle";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);

                    ESP_LOGI("HA_API", "fan toggle: %s", entity.c_str());
                }

                void setFanDirection(const std::string& entity, const char* direction){
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "fan.set_direction";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    resp_kv.key = "direction";
                    resp_kv.value = direction;
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                    
                    ESP_LOGI("HA_API", "fan direction: %s for %s", direction, entity.c_str());
                }

                void setFanSpeed(const std::string& entity, int speed){
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "fan.set_percentage";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    resp_kv.key = "percentage";
                    resp_kv.value = String(speed).c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                    
                    ESP_LOGI("HA_API", "fan speed: %s for %s", String(speed).c_str(), entity.c_str());
                }



// ---------------------------------
//           MEDIA PLAYER
// ---------------------------------
                void setMediaPlayerVolume(const std::string& entity, int volume) {
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    if(volume < 0 || volume > 100){
                        return;
                    }

                    resp.service = "media_player.volume_set";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);
                    
                    resp_kv.key = "volume_level";
                    resp_kv.value = String((float)volume/100).c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                    
                    ESP_LOGI("HA_API", "media player set volume %i for %s", volume, entity.c_str());
                }

                void stopMediaPlayer(const std::string& entity){
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "media_player.media_stop";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);

                    ESP_LOGI("HA_API", "media player stop: %s", entity.c_str());
                }

                void setNextTrackOnMediaPlayer(const std::string& entity){
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "media_player.media_next_track";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);

                    ESP_LOGI("HA_API", "media player next track: %s", entity.c_str());
                }

                void setPreviousTrackOnMediaPlayer(const std::string& entity){
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "media_player.media_previous_track";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);

                    ESP_LOGI("HA_API", "media player previous track: %s", entity.c_str());
                }

                void playPauseMediaPlayer(const std::string& entity){
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "media_player.media_play_pause";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);

                    ESP_LOGI("HA_API", "play/pause media player: %s", entity.c_str());
                }

                void playMediaOnMediaPlayer(const std::string& entity, const std::string& content_id, const std::string& content_type){
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "media_player.play_media";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    
                    resp_kv.key = "media_content_id";
                    resp_kv.value = content_id.c_str();
                    resp.data.push_back(resp_kv);
                    
                    resp_kv.key = "media_content_type";
                    resp_kv.value = content_type.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);

                    ESP_LOGI("HA_API", "play media %s on player: %s", content_id.c_str(), entity.c_str());
                }

                void refreshMediaPlayer(const std::string& entity){
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "homeassistant.update_entity";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);

                    ESP_LOGI("HA_API", "refresh media player: %s", entity.c_str());
                }




// ---------------------------------
//              LOCK
// ---------------------------------
                void openLock(const std::string& entity) {
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "lock.open";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);
             
                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                    ESP_LOGI("HA_API", "open Lock %s", entity.c_str());
                }

                void lockLock(const std::string& entity) {
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "lock.lock";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                    ESP_LOGI("HA_API", "lock Lock %s", entity.c_str());
                }

                void unlockLock(const std::string& entity) {
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "lock.unlock";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);
                    

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                    ESP_LOGI("HA_API", "unlock Lock %s", entity.c_str());
                }




// ---------------------------------
//              NUMBER
// ---------------------------------
                void setNumberValue(const std::string& entity, int value) {
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = this->startsWith("input_number.",entity.c_str())?"input_number.set_value":"number.set_value";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);
                    
                    resp_kv.key = "value";
                    resp_kv.value = String(value).c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                    ESP_LOGI("HA_API", "set Number Value %i for %s", value, entity.c_str());
                }



// ---------------------------------
//              TIMER
// ---------------------------------
                void timerStart(const std::string& entity) {
                    timerStart(entity, 0);
                }

                void timerStart(const std::string& entity, int durationInSeconds) {
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "timer.start";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    resp_kv.key = "duration";
                    resp_kv.value = String(durationInSeconds).c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                    ESP_LOGI("HA_API", "start Timer %s", entity.c_str());
                }

                void timerPause(const std::string& entity) {
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "timer.pause";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                    ESP_LOGI("HA_API", "pause Timer %s", entity.c_str());
                }

                void timerCancle(const std::string& entity) {
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "timer.cancle";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                    ESP_LOGI("HA_API", "cancle Timer %s", entity.c_str());
                }

                void timerFinish(const std::string& entity) {
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "timer.finish";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                    ESP_LOGI("HA_API", "finish Timer %s", entity.c_str());
                }

                void timerChange(const std::string& entity, int durationInSeconds) {
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "timer.change";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    resp_kv.key = "duration";
                    resp_kv.value = String(durationInSeconds).c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                    ESP_LOGI("HA_API", "change Timer %s about %i seconds", entity.c_str(), durationInSeconds);
                }








        };

    }
}