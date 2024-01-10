#pragma once
#include "esphome.h"
#include "esp_log.h"

#include "M5Dial.h"

namespace esphome
{
  namespace shys_m5_dial
  {

    class ShysM5Dial : public text_sensor::TextSensor, public Component
    {
    protected:
      int timeToScreenOff = 30000;
      int longPressMs = 1500;
      int sendValueDelay = 700;
      int rotaryStepWidth = 10;


      struct HaDevice
      {
          int index;
          String entity;
          String name;
          
          int lastValue = 0;

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


          bool startsWith(const char *pre, const char *str){
              return strncmp(pre, str, strlen(pre)) == 0;
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

      HaDevice devices[20];
      int deviceAnzahl = 0;
      int currentDevice = 0;
      uint16_t currentValue = 0;

      long oldPosition = 0;

      int lastDisplayDevice = -1;
      int lastDisplayValue = -1;

      unsigned long lastEvent = 0;
      unsigned long lastRotaryEvent = 0;
      
      bool currentValueModified = false;

      bool enableRFID = true;
      bool enableEncoder = true;

    public:
      void dump_config() override;
      void setup() override;
      void loop() override;

      ShysM5Dial() : Component() {}


     /**
      * 
      */
      void initDevice(){
        auto cfg = M5.config();
        M5Dial.begin(cfg, enableEncoder, enableRFID);
      }


      void changeToNextDevice(){
        if(currentDevice >= deviceAnzahl-1){
          currentDevice = 0;
        } else {
          currentDevice++;
        }
      }


      void handleTouch(){

      }


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
      
      void refreshData(){
        if ( esphome::millis() - lastRotaryEvent > sendValueDelay ) {
          if(devices[currentDevice].getPercentageValue() != currentValue ){
            if(currentValueModified){
              turnLightOn(devices[currentDevice].entity, currentValue);

              currentValueModified = false;
              devices[currentDevice].setPercentageValue(currentValue);
            } else {
              currentValue = devices[currentDevice].getPercentageValue();
            }
          }
        }
      }

      void turnRotaryLeft(){
        M5Dial.Speaker.tone(5000, 20);
        if(currentValue >= rotaryStepWidth){
          currentValue = currentValue - rotaryStepWidth;
        } else {
          currentValue = 0;
        }
        currentValueModified = true;
      }

      void turnRotaryRight(){
        M5Dial.Speaker.tone(5000, 20);
        if(currentValue <= 100-rotaryStepWidth){
          currentValue = currentValue + rotaryStepWidth;
        } else {
          currentValue = 100;
        }          
        currentValueModified = true;
      }

      void handleRotary(){
        long newPosition = M5Dial.Encoder.read();
        if (newPosition != oldPosition) {
            lastRotaryEvent = esphome::millis();
            lastEvent = esphome::millis();

            if(newPosition > oldPosition){
              turnRotaryRight();
            } else {
              turnRotaryLeft();
            }
            oldPosition = newPosition;
        }
      }

      void shortButtonPress(){
        M5Dial.Speaker.tone(4000, 20);
        toggleLight(devices[currentDevice].entity);
      }

      void longButtonPress(){
        changeToNextDevice();
      }

      bool longPress = false;
      void handleButtonPress(){
        if (M5Dial.BtnA.wasPressed()) {
          lastEvent = esphome::millis();
          longPress = false;
        }

        if (M5Dial.BtnA.pressedFor(longPressMs)) {
          M5Dial.Speaker.tone(4000, 200);
          longPress = true;
        }

        if (M5Dial.BtnA.wasReleased()) {
          if(longPress){
            this->longButtonPress();
          } else {
            this->shortButtonPress();
          }
        }
      }

      void validateTouchTimeout()
      {
        if (esphome::millis() - lastEvent > timeToScreenOff ) {
          if(M5Dial.Display.getBrightness()>0){
            M5Dial.Display.setBrightness(0);
            ESP_LOGI("sleep", "Sleep after %d ms", timeToScreenOff);
            publish_state("Display off");
          }
        } else if ( M5Dial.Display.getBrightness()<=0 ) {
          M5Dial.Display.setBrightness(100);
          ESP_LOGI("sleep", "Display on");
          publish_state("Display on");
        }
      }


      void refreshDisplay(LovyanGFX* gfx, bool forceRefresh){
        uint16_t height = gfx->height();
        uint16_t width  = gfx->width();

        if(forceRefresh || currentValue != lastDisplayValue || currentDevice != lastDisplayDevice){
          lastDisplayDevice = currentDevice;
          lastDisplayValue = currentValue;

          gfx->clear();

          gfx->fillRect(0, 0, width, height, RED);
          gfx->fillRect(0, height-(height*currentValue/100), width, height, YELLOW);
          gfx->setTextColor(MAROON);
          gfx->setTextDatum(middle_center);
          gfx->setFont(&fonts::FreeMono12pt7b);

          gfx->setTextSize(3);

          gfx->drawString(String(currentValue),
                          width / 2,
                          height / 2 - 30);

          gfx->setTextSize(1);

          gfx->drawString(devices[currentDevice].name.c_str(),
                          width / 2,
                          height / 2 + 20);
          gfx->drawString(devices[currentDevice].getDeviceType().c_str(),
                          width / 2,
                          height / 2 + 50);  
        }
      }


     /**
      * 
      */
      void addDevice(String entity_id, String name)
      {
        devices[deviceAnzahl].init(entity_id, name);

        deviceAnzahl++;
      }


    };
  }
}