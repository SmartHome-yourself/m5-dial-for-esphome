#pragma once
#include "M5Dial.h"
#include "globals.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class M5DialTouch {
            protected:
                std::function<void(uint16_t, uint16_t)> touch_action;
                std::function<void(const char*)> swipe_action;
                m5::touch_state_t prev_state;

                struct Coord {
                    int x = -1;
                    int y = -1;

                    void reset(){
                        x=-1;
                        y=-1;
                    }
                };

                bool swipeInProgress = false;
                Coord swipeCoord;

            public:
                void on_touch(std::function<void(uint16_t, uint16_t)> callback){
                    ESP_LOGD("DEVICE", "register on_touch Callback");
                    this->touch_action = callback;
                }
                void on_swipe(std::function<void(const char*)> callback){
                    ESP_LOGD("DEVICE", "register on_swipe Callback");
                    this->swipe_action = callback;
                }

                void handleTouch(){
                    auto t = M5Dial.Touch.getDetail();
                    if (prev_state != t.state) {
                        prev_state = t.state;

                        if(strcmp(TOUCH_STATE_NAME[t.state], TOUCH_STATE_NONE) != 0 && 
                           strcmp(TOUCH_STATE_NAME[t.state], TOUCH_STATE_TMP) != 0 ){
                            ESP_LOGD("TOUCH", "Touch Event: %s ( %i / %i )", TOUCH_STATE_NAME[t.state], t.x, t.y);
                        }
                        
                        if( strcmp(TOUCH_STATE_NAME[t.state], TOUCH_STATE_FLICK_BEGIN) == 0){
                            swipeCoord.x = t.x;
                            swipeCoord.y = t.y;
                        
                        } else if( strcmp(TOUCH_STATE_NAME[t.state], TOUCH_STATE_FLICK_END) == 0){
                            uint16_t h = swipeCoord.x > t.x?swipeCoord.x - t.x:t.x - swipeCoord.x;
                            uint16_t v = swipeCoord.y > t.y?swipeCoord.y - t.y:t.y - swipeCoord.y;
                            
                            const char* swipeDirection;
                            if(v>h && v>M5Dial.Display.height()/4 ){
                                if(swipeCoord.y > t.y){
                                    swipeDirection = TOUCH_SWIPE_UP;
                                } else {
                                    swipeDirection = TOUCH_SWIPE_DOWN;
                                }
                            } else if (h>M5Dial.Display.width()/4) {
                                if(swipeCoord.x < t.x){
                                    swipeDirection = TOUCH_SWIPE_RIGHT;
                                } else {
                                    swipeDirection = TOUCH_SWIPE_LEFT;
                                }
                            } else {
                                swipeDirection = TOUCH_SWIPE_NONE;
                            }

                            if(swipeDirection == TOUCH_SWIPE_NONE){
                                this->touch_action(t.x, t.y);
                                ESP_LOGI("TOUCH", "%s: %i / %i", "Touch: ", t.x, t.y);
                            } else {
                                this->swipe_action(swipeDirection);
                                ESP_LOGI("TOUCH", "Swipe: %s", swipeDirection);
                            }
                            
                        } else if( strcmp(TOUCH_STATE_NAME[t.state], TOUCH_STATE_TOUCH_END) == 0){
                            this->touch_action(t.x, t.y);
                            ESP_LOGI("TOUCH", "%s: %i / %i", "Touch: ", t.x, t.y);
                        }

                    }
                }

        };
    }
}