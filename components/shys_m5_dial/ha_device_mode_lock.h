#pragma once
#include "globals.h"
#include "global_image_data.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModeLock: public esphome::shys_m5_dial::HaDeviceMode {
            protected:
                bool openOnButton = false;
                std::string lockState = "unknown";

                long openingTime = 0;


                bool isOpenOnButton(){
                    return this->openOnButton;
                }

                bool isOpen(){
                    return strcmp(lockState.c_str(), "open")==0 || strcmp(lockState.c_str(), "opening")==0;
                }
                
                bool isLocked(){
                    return strcmp(lockState.c_str(), "locked")==0 || strcmp(lockState.c_str(), "locking")==0;
                }

                bool isUnlocked(){
                    return strcmp(lockState.c_str(), "unlocked")==0 || strcmp(lockState.c_str(), "unlocking")==0;
                }

                std::string getLockState(){
                    if(openingTime + 5000 > esphome::millis()){
                        return "Opening";
                    }
                    return this->lockState;
                }

                void sendValueToHomeAssistant(int value) override {
                    switch(value){
                        case 0:
                            this->openLock();
                            break;
                        case 1:
                            haApi.unlockLock(this->device.getEntityId());
                            break;
                        case 2:
                            haApi.lockLock(this->device.getEntityId());
                            break;
                    }
                }

                void showLockStatus(M5DialDisplay& display){
                    LovyanGFX* gfx = display.getGfx();
                    uint16_t height = gfx->height();
                    uint16_t width  = gfx->width();

                    gfx->setTextColor(MAROON);
                    gfx->setTextDatum(middle_center);

                    gfx->startWrite();                      // Secure SPI bus

                    display.clear();

                    display.setFontsize(2);
                    gfx->drawString(this->getLockState().c_str(),
                                    width / 2,
                                    height / 2 - 15);
                    
                    display.setFontsize(1);
                    gfx->drawString(this->device.getName().c_str(),
                                    width / 2,
                                    height / 2 - 80);
                    gfx->drawString("Lock",
                                    width / 2,
                                    height / 2 - 60);  

                    gfx->drawString(String(this->getValue()).c_str(),
                                    width / 2,
                                    height / 2 + 15);

                    display.drawBitmapTransparent(DOOR_OPEN_IMG, width/2-35, height/2+30, 70, 70, 0xFFFF);
                    display.setFontsize(.7);
                    gfx->drawString("Open",
                                    width / 2,
                                    height / 2 +105);  

                    gfx->endWrite();                      // Release SPI bus
                }

                void setReceivedLockState(const std::string& newState){
                    this->lockState = newState;

                    if (this->isLocked()){
                        this->setReceivedValue(2);
                    } else if (this->isUnlocked()){
                        this->setReceivedValue(1);
                    } else if (this->isOpen()){
                        this->setReceivedValue(0);
                    }
                    this->displayRefreshNeeded = true;
                }

                void openLock(){
                    haApi.openLock(this->device.getEntityId());
                    this->openingTime = esphome::millis();
                }

            public:
                HaDeviceModeLock(HaDevice& device) : HaDeviceMode(device){
                    setMaxValue(2);
                }

                void setOpenOnButton(bool openOnBtn){
                    this->openOnButton = openOnBtn;
                }


                void refreshDisplay(M5DialDisplay& display, bool init) override {
                    ESP_LOGD("DISPLAY", "refresh Display: Lock-Modus");
                    this->showLockStatus(display);
                    
                    this->displayRefreshNeeded = false;
                }

                void registerHAListener() override {
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId(),
                                optional<std::string>(), 
                                [this](const esphome::StringRef state) {

                        if(this->isValueModified()){
                            return;
                        }

                        this->setReceivedLockState(state);
                        ESP_LOGI("HA_API", "Got State %s for %s", state.c_str(), this->device.getEntityId().c_str());
                    });
                }

                bool onButton(M5DialDisplay& display, const char * clickType) override {
                    if (strcmp(clickType, BUTTON_SHORT)==0){
                        if(this->isLocked()){
                            if(this->isOpenOnButton()){
                                this->openLock();
                                return true;
                            } else {
                                haApi.unlockLock(this->device.getEntityId());
                                return true;
                            }
                        } else if(this->isUnlocked() || this->isOpen()){
                            haApi.lockLock(this->device.getEntityId());
                            return true;
                        }
                    }

                    this->displayRefreshNeeded = true;

                    return false;
                }

                bool onRotary(M5DialDisplay& display, const char * direction) override {
                    this->defaultOnRotary(display, direction);

                    if(getValue()==0){
                        this->openingTime = esphome::millis();
                    }

                    return true;
                }

                bool onTouch(M5DialDisplay& display, uint16_t x, uint16_t y) override {
                    ESP_LOGI("TOUCH", "Touch %i / %i for %s", x, y, this->device.getEntityId().c_str());

                    LovyanGFX* gfx = display.getGfx();
                    uint16_t height = gfx->height();
                    uint16_t width  = gfx->width();

                    uint16_t minOpenY = height/2+30;
                    uint16_t maxOpenY = height/2+100;
                    uint16_t minOpenX = width/2-40;
                    uint16_t maxOpenX = width/2+40;

                    if(y > minOpenY && y < maxOpenY){
                        if(x>minOpenX && x<maxOpenX){
                            M5Dial.Speaker.tone(5000, 20);
                            this->openLock();
                            return true;
                        } 
                    }

                    return false;
                }

                void onLoop(){
                    if(openingTime>0 && openingTime + 7000 < esphome::millis()){
                        this->displayRefreshNeeded = true;
                        openingTime = 0;
                        setReceivedLockState(this->lockState);
                    }
                }
        };
    }
}
