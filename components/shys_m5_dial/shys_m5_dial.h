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
      int sendValueDelay = 1200;
      int receiveValueDelay = 3000;
      int rotaryStepWidth = 10;

      // -------------------------------

      HaDevice devices[20];
      int deviceAnzahl = 0;
      int currentDevice = 0;
      uint16_t currentValue = 0;

      int lastDisplayDevice = -1;
      int lastDisplayValue = -1;

      unsigned long lastEvent = 0;
      unsigned long lastRotaryEvent = 0;
      
      bool currentValueModified = false;

      bool enableRFID = true;
      bool enableEncoder = true;

      HaApi haApi;

      M5DialDisplay m5DialDisplay;
      M5DialRfid m5DialRfid;
      M5DialRotary m5DialRotary;
      M5DialTouch m5DialTouch;
      
      void refreshDisplay(bool forceRefresh){
        if(forceRefresh || currentValue != lastDisplayValue || currentDevice != lastDisplayDevice){
            lastDisplayDevice = currentDevice;
            lastDisplayValue = currentValue;

            m5DialDisplay.refreshDisplay(currentValue, devices[currentDevice]);
        }
      }

    public:
      void dump_config() override;
      void setup() override;
      void loop() override;

      ShysM5Dial() : Component() {}

     /**
      * 
      */
      void initDevice(){
        using std::placeholders::_1;
        using std::placeholders::_2;

        auto cfg = M5.config();
        M5Dial.begin(cfg, enableEncoder, enableRFID);

        m5DialRotary.on_rotary_left(std::bind(&esphome::shys_m5_dial::ShysM5Dial::turnRotaryLeft, this));
        m5DialRotary.on_rotary_right(std::bind(&esphome::shys_m5_dial::ShysM5Dial::turnRotaryRight, this));
        m5DialRotary.on_short_button_press(std::bind(&esphome::shys_m5_dial::ShysM5Dial::shortButtonPress, this));
        m5DialRotary.on_long_button_press(std::bind(&esphome::shys_m5_dial::ShysM5Dial::longButtonPress, this));

        m5DialTouch.on_touch(std::bind(&esphome::shys_m5_dial::ShysM5Dial::touchInput, this, _1, _2));
      }


      void doLoop(){
        validateTouchTimeout();
        refreshData();

        m5DialRotary.handleRotary();

        if (this->m5DialRotary.handleButtonPress()){
          lastEvent = esphome::millis();
        }
    
        m5DialTouch.handleTouch();

        refreshDisplay(false);
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


      void refreshData(){
        if ( esphome::millis() - lastRotaryEvent > sendValueDelay ) {
          if(devices[currentDevice].getPercentageValue() != currentValue ){
            if(currentValueModified){
              haApi.turnLightOn(devices[currentDevice].getEntityId(), currentValue);
              currentValueModified = false;
              devices[currentDevice].setPercentageValue(currentValue);
              lastRotaryEvent = esphome::millis();
            } else {
              if ( esphome::millis() - lastRotaryEvent > receiveValueDelay ) {
                currentValue = devices[currentDevice].getPercentageValue();
              }
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
        lastEvent = esphome::millis();
        lastRotaryEvent = esphome::millis();

        ESP_LOGI("log", "%s", "Rotary left");
      }

      void turnRotaryRight(){
        M5Dial.Speaker.tone(5000, 20);
        if(currentValue <= 100-rotaryStepWidth){
          currentValue = currentValue + rotaryStepWidth;
        } else {
          currentValue = 100;
        }          
        currentValueModified = true;
        lastEvent = esphome::millis();
        lastRotaryEvent = esphome::millis();

        ESP_LOGI("log", "%s", "Rotary right");
      }



      void shortButtonPress(){
        M5Dial.Speaker.tone(4000, 20);
        haApi.toggleLight(devices[currentDevice].getEntityId());
        ESP_LOGI("log", "%s", "Short press");
      }

      void longButtonPress(){
        lastEvent = esphome::millis();
        changeToNextDevice();
        ESP_LOGI("log", "%s", "Long press");
      }

      void touchInput(uint16_t x, uint16_t y){
        ESP_LOGI("touchInput", "%s: %i / %i", "Touch: ", x, y);
      }



      void changeToNextDevice(){
        if(currentDevice >= deviceAnzahl-1){
          currentDevice = 0;
        } else {
          currentDevice++;
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