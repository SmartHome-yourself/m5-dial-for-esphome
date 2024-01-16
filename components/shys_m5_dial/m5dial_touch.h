#pragma once
#include "M5Dial.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class M5DialTouch {
            const char* state_name[16] = {
                "none", "touch", "touch_end", "touch_begin",
                "___",  "hold",  "hold_end",  "hold_begin",
                "___",  "flick", "flick_end", "flick_begin",
                "___",  "drag",  "drag_end",  "drag_begin"};

            protected:
                std::function<void(uint16_t, uint16_t)> touch_action;

                m5::touch_state_t prev_state;


            public:
                void on_touch(std::function<void(uint16_t, uint16_t)> callback){
                    this->touch_action = callback;
                }

                void handleTouch(){
                    auto t = M5Dial.Touch.getDetail();
                    if (prev_state != t.state) {
                        prev_state = t.state;

                        if(strcmp(this->state_name[t.state], "none") != 0 && 
                           strcmp(this->state_name[t.state], "___") != 0 ){
                            ESP_LOGI("log", "Touch Event: %s ( %i / %i )", this->state_name[t.state], t.x, t.y);
                        }
                        
                        if( strcmp(this->state_name[t.state], "touch") == 0){
                            this->touch_action(t.x, t.y);
                        }
                    }
                }

        };
    }
}