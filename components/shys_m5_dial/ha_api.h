#pragma once
namespace esphome
{
    namespace shys_m5_dial
    {
        class HaApi {
            public:
                void turnLightOn(String entity, int brightness){
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;
                    
                    resp.service = "light.turn_on";
                    
                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);
                    
                    resp_kv.key = "brightness_pct";
                    resp_kv.value = String(brightness).c_str();
                    resp.data.push_back(resp_kv);
                    
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
                }

                void toggleLight(String entity){
                    esphome::api::HomeassistantServiceResponse resp;
                    esphome::api::HomeassistantServiceMap resp_kv;

                    resp.service = "light.toggle";

                    resp_kv.key = "entity_id";
                    resp_kv.value = entity.c_str();
                    resp.data.push_back(resp_kv);

                    esphome::api::global_api_server->send_homeassistant_service_call(resp);
                }
        };

    }
}