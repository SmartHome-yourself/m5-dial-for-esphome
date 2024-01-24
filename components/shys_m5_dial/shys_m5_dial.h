#pragma once
#include "esphome.h"
#include "esp_log.h"

#include "M5Dial.h"
#include "globals.h"


namespace esphome
{
  namespace shys_m5_dial
  {

    class ShysM5Dial : public text_sensor::TextSensor, public Component
    {
    protected:
      int timeToScreenOff = 30000;
      int longPressMs = 1200;
      int sendValueDelay = 1200;
      int receiveValueDelay = 3000;
      int rotaryStepWidth = 10;
      uint16_t displayRefeshPause = 700;

      // -------------------------------

      HaDevice devices[10];
      int deviceAnzahl = 0;
      int currentDevice = 0;
      uint16_t currentValue = 0;

      int lastDisplayDevice = -1;
      float lastDisplayValue = -1;
      
      unsigned long lastRotaryEvent = 0;
      unsigned long lastReceiveEvent = 0;
      unsigned long lastDisplayRefresh = 0;
      
      bool currentValueModified = false;

      bool enableRFID = true;
      bool enableEncoder = true;

      HaApi haApi;

      M5DialDisplay m5DialDisplay;
      M5DialRfid m5DialRfid;
      M5DialRotary m5DialRotary;
      M5DialTouch m5DialTouch;
            
      bool isDisplayRefreshNeeded(){
        if (devices[currentDevice].getCurrentMode() == DEVICE_MODE_BRIGHTNESS && (currentValue != lastDisplayValue || currentDevice != lastDisplayDevice)){
          return esphome::millis() - lastDisplayRefresh > displayRefeshPause;
        } else if(devices[currentDevice].getCurrentMode() == DEVICE_MODE_COLOR && (devices[currentDevice].getColor() != lastDisplayValue || currentDevice != lastDisplayDevice)){
          return esphome::millis() - lastDisplayRefresh > displayRefeshPause;;
        }

        return false;
      }


      void refreshDisplay(bool forceRefresh){
        if(forceRefresh || isDisplayRefreshNeeded()){
            lastDisplayDevice = currentDevice;
            
            if (devices[currentDevice].getCurrentMode() == DEVICE_MODE_BRIGHTNESS){
              lastDisplayValue = currentValue;
            } else if(devices[currentDevice].getCurrentMode() == DEVICE_MODE_COLOR){
              lastDisplayValue = devices[currentDevice].getColor();
            }

            m5DialDisplay.refreshDisplay(currentValue, devices[currentDevice]);
        }
      }

      void setCurrenValue(uint16_t value){
        currentValue = value;
        currentValueModified = true;
      }

      void updateBrightnessValue(){
        if(devices[currentDevice].getPercentageValue() != currentValue ){
          if(currentValueModified){
            haApi.turnLightOn(devices[currentDevice].getEntityId(), currentValue);
            currentValueModified = false;
            devices[currentDevice].setPercentageValue(currentValue);
            lastReceiveEvent = esphome::millis();
          } else {
            if ( esphome::millis() - lastReceiveEvent > receiveValueDelay ) {
              currentValue = devices[currentDevice].getPercentageValue();
              lastReceiveEvent = esphome::millis();
            }
          }
        }
      }

      void updateColorValue(){
        if(devices[currentDevice].getColor() != currentValue ){
          if(currentValueModified){
            haApi.turnLightOn(devices[currentDevice].getEntityId(), devices[currentDevice].getPercentageValue(), currentValue);
            currentValueModified = false;
            devices[currentDevice].setColor(currentValue);
            lastReceiveEvent = esphome::millis();
          } else {
            if ( esphome::millis() - lastReceiveEvent > receiveValueDelay ) {
              currentValue = devices[currentDevice].getColor();
              lastReceiveEvent = esphome::millis();
            }
          }
        }
      }

      void refreshData(){
        if ( esphome::millis() - lastRotaryEvent > sendValueDelay ) {
          if (devices[currentDevice].getCurrentMode() == DEVICE_MODE_BRIGHTNESS){
            updateBrightnessValue();
          } else if(devices[currentDevice].getCurrentMode() == DEVICE_MODE_COLOR){
            updateColorValue();
          }
        }
      }


    public:
      void dump_config() override;
      void setup() override;
      void loop() override;

      ShysM5Dial() : Component() {}

      void setScreenOffTime(int value){
        this->timeToScreenOff = value;
        m5DialDisplay.setTimeToScreenOff(value);
      }

      void setLongPressDuration(int value){
        this->longPressMs = value;
        m5DialRotary.setLongPressDuration(value);
      }

      void setSendValueDelay(int value){
        this->sendValueDelay = value;
      }

      void setReceiveValueDelay(int value){
        this->receiveValueDelay = value;
      }

      void setRotaryStepWidth(int value){
        this->rotaryStepWidth = value;
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

        m5DialRotary.on_rotary_left(std::bind(&esphome::shys_m5_dial::ShysM5Dial::turnRotaryLeft, this));
        m5DialRotary.on_rotary_right(std::bind(&esphome::shys_m5_dial::ShysM5Dial::turnRotaryRight, this));
        m5DialRotary.on_short_button_press(std::bind(&esphome::shys_m5_dial::ShysM5Dial::shortButtonPress, this));
        m5DialRotary.on_long_button_press(std::bind(&esphome::shys_m5_dial::ShysM5Dial::longButtonPress, this));

        m5DialTouch.on_touch(std::bind(&esphome::shys_m5_dial::ShysM5Dial::touchInput, this, _1, _2));
        m5DialTouch.on_swipe(std::bind(&esphome::shys_m5_dial::ShysM5Dial::touchSwipe, this, _1));

        m5DialDisplay.on_publish_state(std::bind(&esphome::shys_m5_dial::ShysM5Dial::publish_state, this, _1));
      }


      void doLoop(){
        if(api::global_api_server->is_connected()){
          refreshData();

          m5DialRotary.handleRotary();

          if (m5DialRotary.handleButtonPress()){
            m5DialDisplay.resetLastEventTimer();
          }

          m5DialTouch.handleTouch();

          m5DialDisplay.validateTimeout();

          refreshDisplay(false);
        } else if(network::is_connected()){
          ESP_LOGD("HA_API", "API is not connected");
          m5DialDisplay.showDisconnected();
          esphome::delay(10);
        } else {
          ESP_LOGD("wifi", "Network is not connected");
          m5DialDisplay.showOffline();
          esphome::delay(10);
        }
      }



      void turnRotaryLeft(){
        M5Dial.Speaker.tone(5000, 20);


        if (devices[currentDevice].getCurrentMode() == DEVICE_MODE_BRIGHTNESS){
          if(devices[currentDevice].isDimmEnabled()){
            reduceCurrentValue();
          } else {
            setCurrenValue(0);
          }
        } else if(devices[currentDevice].getCurrentMode() == DEVICE_MODE_COLOR){
          raiseCurrentValue();
        }

        refreshDisplay(true);

        m5DialDisplay.resetLastEventTimer();
        lastRotaryEvent = esphome::millis();
        ESP_LOGI("DEVICE", "%s", "Rotary left");
      }

      void turnRotaryRight(){
        M5Dial.Speaker.tone(5000, 20);

        if (devices[currentDevice].getCurrentMode() == DEVICE_MODE_BRIGHTNESS){
          if(devices[currentDevice].isDimmEnabled()){
            raiseCurrentValue();
          } else {
            setCurrenValue(100);
          }
        } else if(devices[currentDevice].getCurrentMode() == DEVICE_MODE_COLOR){
          reduceCurrentValue();
        }

        refreshDisplay(true);

        m5DialDisplay.resetLastEventTimer();
        lastRotaryEvent = esphome::millis();
        ESP_LOGI("DEVICE", "%s", "Rotary right");
      }



      void shortButtonPress(){
        M5Dial.Speaker.tone(4000, 20);
        ESP_LOGI("DEVICE", "%s", "Short press");

        if (devices[currentDevice].getCurrentMode() == DEVICE_MODE_BRIGHTNESS){
          haApi.toggleLight(devices[currentDevice].getEntityId());
        } else if(devices[currentDevice].getCurrentMode() == DEVICE_MODE_COLOR){
          haApi.toggleLight(devices[currentDevice].getEntityId());
        }
        
        m5DialDisplay.resetLastEventTimer();
      }

      void longButtonPress(){
        m5DialDisplay.resetLastEventTimer();

        changeToNextDevice();

        ESP_LOGI("DEVICE", "%s", "Long press");
      }


      void touchInput(uint16_t x, uint16_t y){
        m5DialDisplay.resetLastEventTimer();

        if (devices[currentDevice].getCurrentMode() == DEVICE_MODE_BRIGHTNESS){
          if(m5DialDisplay.isDisplayOn()){
            if(devices[currentDevice].isDimmEnabled()){
              updateCurrentValueByTouch(y);
            } else {
              haApi.toggleLight(devices[currentDevice].getEntityId());
            }
            lastRotaryEvent = esphome::millis();
          }
        } else if(devices[currentDevice].getCurrentMode() == DEVICE_MODE_COLOR){
          if(m5DialDisplay.isDisplayOn()){
            updateColorValueByTouch(x, y);
            lastRotaryEvent = esphome::millis();    
          }
        }

        ESP_LOGI("TOUCH", "%s: %i / %i", "Touch: ", x, y);
      }

      void changeToNextMode(){
        devices[currentDevice].nextMode();
        if(devices[currentDevice].getCurrentMode() == DEVICE_MODE_COLOR){
          currentValue = devices[currentDevice].getColor();
        } else {
          currentValue = devices[currentDevice].getLastKnownValue();
        }
        refreshDisplay(true);
      }

      void touchSwipe(const char* direction){
        m5DialDisplay.resetLastEventTimer();

        if(m5DialDisplay.isDisplayOn()){
          if( strcmp(direction, TOUCH_SWIPE_UP) == 0){
            ESP_LOGI("TOUCH", "%s", "Swipe up");
            //devices[currentDevice].previousMode();
          
          } else if( strcmp(direction, TOUCH_SWIPE_DOWN) == 0){
            ESP_LOGI("TOUCH", "%s", "Swipe down");
            changeToNextMode();
            ESP_LOGI("DEVICE", "%s %i", "New Mode: ", devices[currentDevice].getCurrentMode());
          
          } else if( strcmp(direction, TOUCH_SWIPE_LEFT) == 0){
            changeToNextDevice();
            ESP_LOGI("TOUCH", "%s", "Swipe left");
          
          } else if( strcmp(direction, TOUCH_SWIPE_RIGHT) == 0){
            changeToPreviousDevice();
            ESP_LOGI("TOUCH", "%s", "Swipe right");
          }
        }
      }


      void updateCurrentValueByTouch(uint16_t newVal){
        if(newVal > m5DialDisplay.getHeight()*.95){
          newVal = m5DialDisplay.getHeight();
        } else if (newVal < m5DialDisplay.getHeight()*.05){
          newVal = 0;
        }
        uint16_t result = 100 - ((newVal * (100/rotaryStepWidth)) / m5DialDisplay.getHeight()) * rotaryStepWidth;
        setCurrenValue(result);
        ESP_LOGD("TOUCH", "Aktuellen Wert auf %i gesetzt", result);
      }

      void updateColorValueByTouch(uint16_t x, uint16_t y){
        uint16_t degree = m5DialDisplay.getDegByCoord(x, y);
        //devices[currentDevice].setColor(degree);
        currentValue = degree;
        currentValueModified = true;
        ESP_LOGD("TOUCH", "Neuen Farbwert auf %d gesetzt", degree);
      }


      void raiseCurrentValue(){
        if(currentValue <= devices[currentDevice].getMaxRotary() - rotaryStepWidth){
          setCurrenValue(currentValue + rotaryStepWidth);
        } else {
          setCurrenValue(devices[currentDevice].getMaxRotary());
        }          
      }

      void reduceCurrentValue(){
        if(currentValue >= rotaryStepWidth){
          setCurrenValue(currentValue - rotaryStepWidth);
        } else {
          setCurrenValue(0);
        }
      }


/*
      void raiseColorValue(){
        if(devices[currentDevice].getColor() <= devices[currentDevice].getMaxRotary() - rotaryStepWidth){
          devices[currentDevice].setColor(devices[currentDevice].getColor() + rotaryStepWidth);
        } else {
          devices[currentDevice].setColor(0);
        }
        currentValueModified = true;
      }

      void reduceColorValue(){
        if(devices[currentDevice].getColor() >= rotaryStepWidth){
          devices[currentDevice].setColor(devices[currentDevice].getColor() - rotaryStepWidth);
        } else {
          devices[currentDevice].setColor(devices[currentDevice].getMaxRotary());
        }
        currentValueModified = true;
      }      
*/

      void changeToNextDevice(){
        if(currentDevice >= deviceAnzahl-1){
          currentDevice = 0;
        } else {
          currentDevice++;
        }
      }

      void changeToPreviousDevice(){
        if(currentDevice >= 1){
          currentDevice--;
        } else {
          currentDevice = deviceAnzahl-1;
        }
      }

     /**
      * 
      */
      void addDevice(String entity_id, String name, bool rgb_enabled, bool dimm_enabled)
      {
        devices[deviceAnzahl].init(entity_id, name, rgb_enabled, dimm_enabled);
        deviceAnzahl++;
      }

    };
  }
}