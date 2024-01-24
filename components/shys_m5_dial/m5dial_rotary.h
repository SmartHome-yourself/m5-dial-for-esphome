#pragma once
#include "M5Dial.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class M5DialRotary {
            protected:
                std::function<void(void)> rotary_left_action;
                std::function<void(void)> rotary_right_action;
                std::function<void(void)> short_button_press_action;
                std::function<void(void)> long_button_press_action;


                int longPressMs = 1500;

                long oldPosition = 0;
                bool longPress = false;

            public:
                void on_rotary_right(std::function<void(void)> callback){
                    this->rotary_right_action = callback;
                }

                void on_rotary_left(std::function<void(void)> callback){
                    this->rotary_left_action = callback;
                }

                void on_short_button_press(std::function<void(void)> callback){
                    this->short_button_press_action = callback;
                }

                void on_long_button_press(std::function<void(void)> callback){
                    this->long_button_press_action = callback;
                }

                void setLongPressDuration(int value){
                    longPressMs = value;
                }




                void handleRotary(){
                    long newPosition = M5Dial.Encoder.read();
                    if (newPosition != this->oldPosition) {
                        if(newPosition > this->oldPosition){
                            this->rotary_right_action();
                        } else {
                            this->rotary_left_action();
                        }
                        this->oldPosition = newPosition;
                    }
                }


                bool handleButtonPress(){
                    bool is_event = false;

                    if (M5Dial.BtnA.wasPressed()) {
                        longPress = false;
                        is_event = true;
                    }

                    if (M5Dial.BtnA.pressedFor(longPressMs)) {
                        M5Dial.Speaker.tone(4000, 200);
                        longPress = true;
                        is_event = true;
                    }

                    if (M5Dial.BtnA.wasReleased()) {
                        if(longPress){
                            this->long_button_press_action();
                        } else {
                            this->short_button_press_action();
                        }
                        is_event = true;
                    }

                    return is_event;
                }

        };
    }
}