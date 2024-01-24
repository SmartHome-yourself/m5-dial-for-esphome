#pragma once

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaApi {
            protected:
                std::string colorValue;
                esphome::api::HomeassistantServiceResponse resp;
                esphome::api::HomeassistantServiceMap resp_kv;
                
            public:
                void turnLightOn(String entity, int brightness){
                    turnLightOn(entity, brightness, -1);
                }

                void turnLightOn(String entity, int brightness, int color){
                    ESP_LOGD("HA_API", "light on values: %s (%i) Color: %i", entity.c_str(), brightness, color);

                    resp.service = "light.turn_on";
                    
                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);
                    
                    resp_kv.key = "brightness_pct";
                    resp_kv.value = String(brightness).c_str();
                    resp.data.push_back(resp_kv);
                    
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

                void turnLightOff(String entity){
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "light.turn_off";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);

                    ESP_LOGI("HA_API", "turn light off: %s", entity.c_str());
                }

                void toggleLight(String entity){
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "light.toggle";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                    
                    ESP_LOGI("HA_API", "toggle light: %s", entity.c_str());
                }
        };

    }
}