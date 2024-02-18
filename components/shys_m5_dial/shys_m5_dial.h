#pragma once
#include "esphome.h"
#include "esp_log.h"

#include "globals.h"
#include "ha_api.h"
#include "ha_device.h"
#include "ha_device_light.h"
#include "M5Dial.h"

namespace esphome
{
  namespace shys_m5_dial
  {

    class ShysM5Dial : public Component
    {
    protected:
      int timeToScreenOff = 30000;
      int longPressMs = 1200;
      int sendValueDelay = 1200;
      int receiveValueDelay = 3000;
      int rotaryStepWidth = 10;
      uint16_t displayRefeshPause = 700;

      // -------------------------------

      HaDevice* devices[10];
      int deviceAnzahl = 0;

      int currentDevice = 0;

      int lastDisplayDevice = -1;
      float lastDisplayValue = -1;
      
      unsigned long lastRotaryEvent = 0;
      unsigned long lastReceiveEvent = 0;
      unsigned long lastDisplayRefresh = 0;
      
      int lastLoop = 0;

      bool enableRFID = true;
      bool enableEncoder = true;

      M5DialDisplay* m5DialDisplay = new M5DialDisplay();
      M5DialRfid* m5DialRfid = new M5DialRfid();
      M5DialRotary* m5DialRotary = new M5DialRotary();
      M5DialTouch* m5DialTouch = new M5DialTouch();

      bool startsWith(const char *pre, const char *str){
          return strncmp(pre, str, strlen(pre)) == 0;
      }

      int getCurrentValue(){
        return devices[currentDevice]->getValue();
      }

      bool isDisplayRefreshNeeded(){
        if (getCurrentValue() != lastDisplayValue || currentDevice != lastDisplayDevice){
          return esphome::millis() - lastDisplayRefresh > displayRefeshPause;
        }
        return false;
      }

      void refreshDisplay(bool forceRefresh){
        if(forceRefresh || isDisplayRefreshNeeded()){
            devices[currentDevice]->refreshDisplay(*m5DialDisplay, lastDisplayDevice != currentDevice);

            lastDisplayDevice = currentDevice;
            lastDisplayValue = getCurrentValue();
        }
      }


      void nextDevice(){
        if(currentDevice >= deviceAnzahl-1){
          currentDevice = 0;
        } else {
          currentDevice++;
        }
      }

      void previousDevice(){
        if(currentDevice >= 1){
          currentDevice--;
        } else {
          currentDevice = deviceAnzahl-1;
        }
      }

      
     /**
      * 
      */
      void addDevice(HaDevice* device){
        if (device != nullptr) {
          ESP_LOGD("DEVICE", "New Device: %s", device->getName().c_str());

          devices[deviceAnzahl] = device;
          devices[deviceAnzahl]->init();
          deviceAnzahl++;
          ESP_LOGD("DEVICE", "Device added");
        }
      }


    public:
      void dump_config() override;
      void setup() override;
      void loop() override;

      ShysM5Dial() : Component() {}

      void setScreenOffTime(int value){
        ESP_LOGI("DEVICE", "setScreenOffTime %i", value);
        this->timeToScreenOff = value;
        m5DialDisplay->setTimeToScreenOff(value);
      }

      void setLongPressDuration(int value){
        ESP_LOGI("DEVICE", "setLongPressDuration %i", value);
        this->longPressMs = value;
        m5DialRotary->setLongPressDuration(value);
      }

      void setSendValueDelay(int value){
        ESP_LOGI("DEVICE", "setSendValueDelay %i", value);
        this->sendValueDelay = value;
      }

      void setReceiveValueDelay(int value){
        ESP_LOGI("DEVICE", "setReceiveValueDelay %i", value);
        this->receiveValueDelay = value;
      }

      void setRotaryStepWidth(int value){
        ESP_LOGI("DEVICE", "setRotaryStepWidth %i", value);
        this->rotaryStepWidth = value;
      }


     /**
      * 
      */
      void addLight(const std::string& entity_id, const std::string& name, const std::string& modes){
        HaDeviceLight* light = new HaDeviceLight(entity_id, name, modes);

        //ESP_LOGD("DEVICE", "mode_config: %s", modes.c_str());
        addDevice(light);
      }



     /**
      * 
      */
      void initDevice(){
        using std::placeholders::_1;
        using std::placeholders::_2;

        ESP_LOGI("DEVICE", "Initialisierung...");

        auto cfg = M5.config();
        M5Dial.begin(cfg, enableEncoder, enableRFID);

        ESP_LOGI("DEVICE", "Register Callbacks...");
        m5DialRotary->on_rotary_left(std::bind(&esphome::shys_m5_dial::ShysM5Dial::turnRotaryLeft, this));
        m5DialRotary->on_rotary_right(std::bind(&esphome::shys_m5_dial::ShysM5Dial::turnRotaryRight, this));
        m5DialRotary->on_short_button_press(std::bind(&esphome::shys_m5_dial::ShysM5Dial::shortButtonPress, this));
        m5DialRotary->on_long_button_press(std::bind(&esphome::shys_m5_dial::ShysM5Dial::longButtonPress, this));

        m5DialTouch->on_touch(std::bind(&esphome::shys_m5_dial::ShysM5Dial::touchInput, this, _1, _2));
        m5DialTouch->on_swipe(std::bind(&esphome::shys_m5_dial::ShysM5Dial::touchSwipe, this, _1));
      }




     /**
      * 
      */
      void doLoop(){
        if(api::global_api_server->is_connected()){
          m5DialRotary->handleRotary();

          if (m5DialRotary->handleButtonPress()){
            m5DialDisplay->resetLastEventTimer();
          }

          m5DialTouch->handleTouch();
          m5DialDisplay->validateTimeout();

          devices[currentDevice]->updateHomeAssistantValue();

          this->refreshDisplay(false);
          lastLoop = 1;

        } else if(network::is_connected()){
          if(lastLoop != 2){
            ESP_LOGD("HA_API", "API is not connected");
          }
          m5DialDisplay->showDisconnected();
          esphome::delay(10);
          lastLoop = 2;

        } else {
          if(lastLoop != 3){
            ESP_LOGD("wifi", "Network is not connected");
          }
          m5DialDisplay->showOffline();
          esphome::delay(10);
          lastLoop = 3;          
        }
      }



     /**
      * 
      */
      void turnRotaryLeft(){
        m5DialDisplay->resetLastEventTimer();
        M5Dial.Speaker.tone(5000, 20);

        if(m5DialDisplay->isDisplayOn()){
          devices[currentDevice]->onRotary(*m5DialDisplay, ROTARY_LEFT);
        }

        lastRotaryEvent = esphome::millis();
      }

     /**
      * 
      */
      void turnRotaryRight(){
        m5DialDisplay->resetLastEventTimer();
        M5Dial.Speaker.tone(5000, 20);

        if(m5DialDisplay->isDisplayOn()){
          devices[currentDevice]->onRotary(*m5DialDisplay, ROTARY_RIGHT);
        }

        lastRotaryEvent = esphome::millis();
      }

     /**
      * 
      */
      void shortButtonPress(){
        m5DialDisplay->resetLastEventTimer();
        M5Dial.Speaker.tone(4000, 20);
        if(m5DialDisplay->isDisplayOn()){
        }
      }

     /**
      * 
      */
      void longButtonPress(){
        if(m5DialDisplay->isDisplayOn()){
          m5DialDisplay->resetLastEventTimer();
          devices[currentDevice]->nextMode();
        }
      }

     /**
      * 
      */
      void touchInput(uint16_t x, uint16_t y){
        m5DialDisplay->resetLastEventTimer();
        if(m5DialDisplay->isDisplayOn()){
          devices[currentDevice]->onTouch(*m5DialDisplay, x, y);
        }
      }

     /**
      * 
      */
      void touchSwipe(const char* direction){
        ESP_LOGD("TOUCH", "touchSwipe direction: %s", direction);
        m5DialDisplay->resetLastEventTimer();
        if(m5DialDisplay->isDisplayOn()){
          if(! devices[currentDevice]->onSwipe(*m5DialDisplay, direction) ){

            if(strcmp(direction, TOUCH_SWIPE_LEFT)==0){
              this->previousDevice();
            } else if(strcmp(direction, TOUCH_SWIPE_RIGHT)==0){
              this->nextDevice();
            } else if(strcmp(direction, TOUCH_SWIPE_UP)==0){
              devices[currentDevice]->previousMode();
            } else if(strcmp(direction, TOUCH_SWIPE_DOWN)==0){
              devices[currentDevice]->nextMode();
            } 

          }
        }
      }

    };
  }
}