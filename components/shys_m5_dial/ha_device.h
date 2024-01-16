#pragma once
namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDevice {
            protected:
                int index;
                String entity;
                String name;
                
                int lastValue = 0;


                bool startsWith(const char *pre, const char *str){
                    return strncmp(pre, str, strlen(pre)) == 0;
                }

            public:
                String getName(){
                    return this->name;
                }

                String getEntityId(){
                    return this->entity;
                }

                String getAttribute(){
                    if(this->isLight()){
                    return "brightness";
                    } else if(this->isCover()){
                    return "current_position";
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

                uint16_t getPercentageValue() {
                    if(this->isLight()){
                    return round((float)getLastKnownValue() * 100 / 255);
                    }
                    return getLastKnownValue();
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

                void init(String entity_id, String name){
                    this->entity = entity_id;
                    this->name   = name;
                    this->lastValue = 50;

                    std::string n = this->entity.c_str();
                    std::string a = this->getAttribute().c_str();

                    api::global_api_server->subscribe_home_assistant_state(
                                n,
                                a, 
                                [this](const std::string &state) {

                        auto val = parse_number<int>(state);
                        if (!val.has_value()) {
                        // ESP_LOGW("ERR", "Can't convert '%s' to number!", state.c_str());
                        //return;
                        val = 0;
                        } else {
                        ESP_LOGD("OK", " Got value %i for %s", *val, this->entity.c_str());
                        }

                        this->lastValue = *val;
                    });
                }

        };
    }
}