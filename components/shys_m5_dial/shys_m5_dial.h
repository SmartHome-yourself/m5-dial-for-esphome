#pragma once
#include "esphome.h"
#include "esp_log.h"

#include "globals.h"
#include "ha_api.h"
#include "ha_device.h"
#include "ha_device_light.h"
#include "ha_device_climate.h"
#include "ha_device_cover.h"
#include "ha_device_switch.h"
#include "ha_device_fan.h"
#include "ha_device_mediaplayer.h"
#include "ha_device_lock.h"

#include "M5Dial.h"

#define MAX_DEVICE_COUNT 50

namespace esphome
{
  namespace shys_m5_dial
  {
    class ShysM5Dial : public Component, public esphome::api::CustomAPIDevice
    {
    protected:
      int timeToScreenOff = 30000;
      int longPressMs = 1200;
      int rotaryStepWidth = 10;
      uint16_t displayRefeshPause = 700;

      int apiSendDelay = 1000; // Verzögerung nach Wert-Änderung (um nicht jeden Wert beim drehen des Rades zu senden)
      int apiSendLock = 3000;  // Wartezeit zwischen einzelnden API-Aufrufen

      // -------------------------------

      HaDevice* devices[MAX_DEVICE_COUNT];
      int deviceAnzahl = 0;
      // device numbers aka indices should be of unsigned type

      int currentDevice = 0;

      int lastDisplayDevice = -1;
      float lastDisplayValue = -1;
      int lastModeIndex = -1;
      
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
      M5DialEEPROM* m5DialEEPROM = new M5DialEEPROM();

      bool startsWith(const char *pre, const char *str){
          return strncmp(pre, str, strlen(pre)) == 0;
      }

      int getCurrentValue(){
        return devices[currentDevice]->getValue();
      }

      bool isDisplayRefreshNeeded(){
        if (getCurrentValue() != lastDisplayValue || 
              currentDevice != lastDisplayDevice ||
              devices[currentDevice]->getCurrentModeIndex() != lastModeIndex || 
              devices[currentDevice]->isDisplayRefreshNeeded()){
          return esphome::millis() - lastDisplayRefresh > displayRefeshPause;
        }
        return false;
      }

      void refreshDisplay(bool forceRefresh){
        if(forceRefresh || isDisplayRefreshNeeded()){
            devices[currentDevice]->refreshDisplay(*m5DialDisplay, lastDisplayDevice != currentDevice);

            lastDisplayDevice  = currentDevice;
            lastModeIndex      = devices[currentDevice]->getCurrentModeIndex();
            lastDisplayValue   = getCurrentValue();
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
          if(this->deviceAnzahl >= MAX_DEVICE_COUNT-1){
            ESP_LOGE("DEVICE", "EXCEED DEVICE COUNT MAXIMUM: %s can not be added!", device->getName().c_str());
            return;
          }

          ESP_LOGD("DEVICE", "New Device: %s", device->getName().c_str());

          devices[deviceAnzahl] = device;

          devices[deviceAnzahl]->setApiSendDelay(this->apiSendDelay);
          devices[deviceAnzahl]->setApiSendLock(this->apiSendLock);
          devices[deviceAnzahl]->setRotaryStepWidth(this->rotaryStepWidth);

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

      void setApiSendDelay(int delayInMs){
          ESP_LOGI("DEVICE", "setApiSendDelay %i", delayInMs);
          this->apiSendDelay = delayInMs;
      }
      
      void setApiSendLock(int delayInMs){
          ESP_LOGI("DEVICE", "setApiSendLock %i", delayInMs);
          this->apiSendLock = delayInMs;
      }

      void setRotaryStepWidth(int value){
        ESP_LOGI("DEVICE", "setRotaryStepWidth %i", value);
        this->rotaryStepWidth = value;
      }

      void setFontName(std::string value){
        ESP_LOGI("DEVICE", "setFontName %s", value);
        m5DialDisplay->setFontName(value);
      }

      void setFontFactor(int value){
        ESP_LOGI("DEVICE", "setFontFactor %i", value);
        m5DialDisplay->setFontFactor(value);
      }


     /**
      * 
      */
      void addLight(const std::string& entity_id, const std::string& name, const std::string& modes){
        HaDeviceLight* light = new HaDeviceLight(entity_id, name, modes);
        addDevice(light);
      }


     /**
      * 
      */
      void addClimate(const std::string& entity_id, const std::string& name, const std::string& modes){
        HaDeviceClimate* climate = new HaDeviceClimate(entity_id, name, modes);
        addDevice(climate);
      }


     /**
      * 
      */
      void addCover(const std::string& entity_id, const std::string& name, const std::string& modes){
        HaDeviceCover* climate = new HaDeviceCover(entity_id, name, modes);
        addDevice(climate);
      }


     /**
      * 
      */
      void addSwitch(const std::string& entity_id, const std::string& name, const std::string& modes){
        HaDeviceSwitch* switchDevice = new HaDeviceSwitch(entity_id, name, modes);
        addDevice(switchDevice);
      }


     /**
      * 
      */
      void addFan(const std::string& entity_id, const std::string& name, const std::string& modes){
        HaDeviceFan* fan = new HaDeviceFan(entity_id, name, modes);
        addDevice(fan);
      }


     /**
      * 
      */
      void addMediaPlayer(const std::string& entity_id, const std::string& name, const std::string& modes){
        HaDeviceMediaPlayer* mediaPlayer = new HaDeviceMediaPlayer(entity_id, name, modes);
        addDevice(mediaPlayer);
      }


      void addLock(const std::string& entity_id, const std::string& name, const std::string& modes){
        HaDeviceLock* lock = new HaDeviceLock(entity_id, name, modes);
        addDevice(lock);
      }


      void addMenu(const std::string& entity_id, const std::string& name, const std::string& modes);

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

        this->registerServices();
      }

      /**
       * @brief Get the Device Name At index
       *        returns last device if index is to large
       * 
       * @param index 
       * @return std::string 
       */
      const std::string getDeviceNameAt(uint16_t index){
        if(index >= deviceAnzahl){
          index= deviceAnzahl-1;
        } 
       
        return devices[index]->getName();
      }

      /**
       * @brief return pointer to the Devicet object at index
       * 
       * @param index 
       * @return HaDevice* 
       */
      HaDevice* getDevice(uint16_t index){
        if(index >= deviceAnzahl){
          index= deviceAnzahl-1;
        } 
       
        return devices[index];
      }

      /**
       * @brief Get the number of used devices
       * 
       * @return int 
       */
      inline int getDeviceCount(){
        return deviceAnzahl;
      }


      /**
       * @brief Set the currentDevice to new value and refresh
       * 
       * @param newDev 
       */
      void setCurrentDevice(uint16_t newDev){
        if(newDev >= deviceAnzahl-1){
          currentDevice = 0;
        } else {
          currentDevice=newDev;
        }
        refreshDisplay(true);
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

          devices[currentDevice]->onLoop();

          this->refreshDisplay(false);
          lastLoop = 1;

        } else if(network::is_connected()){
          if(lastLoop != 2){
            ESP_LOGD("HA_API", "API is not connected");
            // in if hinein verschoben = don't refresh if already in this state
            m5DialDisplay->showDisconnected();

          }
          esphome::delay(10);
          lastLoop = 2;

        } else {
          if(lastLoop != 3){
            ESP_LOGD("wifi", "Network is not connected");
            // in if hinein verschoben = don't refresh if already in this state
            m5DialDisplay->showOffline();

          }
          esphome::delay(10);
          lastLoop = 3;          
        }
      }


     /**
      * 
      */
      void registerServices(){
        register_service(&ShysM5Dial::selectDevice, "selectDevice", {"entity_id"});
      }

     /**
      * 
      */
      void selectDevice(std::string entityId){
        bool found = false;

        for(int i=0; i<deviceAnzahl; i++){
          HaDevice *device = devices[i];
          if( strcmp(device->getEntityId().c_str(), entityId.c_str()) == 0 ){
            this->currentDevice = i;
            this->m5DialDisplay->resetLastEventTimer();

            found = true;
            ESP_LOGI("SERVICE", "Entity %s selected", entityId.c_str());
            return;
          } 
        }
        ESP_LOGW("SERVICE", "Entity-ID %s not found", entityId.c_str());
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
          devices[currentDevice]->onButton(*m5DialDisplay, BUTTON_SHORT);
        }
      }

     /**
      * 
      */
      void longButtonPress(){
        if(m5DialDisplay->isDisplayOn()){
          m5DialDisplay->resetLastEventTimer();
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