#pragma once
#ifndef GLOBALS_H 
#define GLOBALS_H

    static const char *TOUCH_STATE_NONE = "none";
    static const char *TOUCH_STATE_TOUCH = "touch";
    static const char *TOUCH_STATE_TOUCH_END = "touch_end";
    static const char *TOUCH_STATE_TOUCH_BEGIN = "touch_begin";

    static const char *TOUCH_STATE_HOLD = "hold";
    static const char *TOUCH_STATE_HOLD_END = "hold_end";
    static const char *TOUCH_STATE_HOLD_BEGIN = "hold_begin";

    static const char *TOUCH_STATE_FLICK = "flick";
    static const char *TOUCH_STATE_FLICK_END = "flick_end";
    static const char *TOUCH_STATE_FLICK_BEGIN = "flick_begin";

    static const char *TOUCH_STATE_DRAG = "drag";
    static const char *TOUCH_STATE_DRAG_END = "drag_end";
    static const char *TOUCH_STATE_DRAG_BEGIN = "drag_begin";

    static const char *TOUCH_STATE_TMP = "___";


    static const char *TOUCH_STATE_NAME[16] = {
        TOUCH_STATE_NONE, TOUCH_STATE_TOUCH, TOUCH_STATE_TOUCH_END, TOUCH_STATE_TOUCH_BEGIN,
        TOUCH_STATE_TMP, TOUCH_STATE_HOLD, TOUCH_STATE_HOLD_END, TOUCH_STATE_HOLD_BEGIN,
        TOUCH_STATE_TMP, TOUCH_STATE_FLICK, TOUCH_STATE_FLICK_END, TOUCH_STATE_FLICK_BEGIN,
        TOUCH_STATE_TMP, TOUCH_STATE_DRAG, TOUCH_STATE_DRAG_END, TOUCH_STATE_DRAG_BEGIN };

    static const char *TOUCH_SWIPE_NONE = "none";
    static const char *TOUCH_SWIPE_UP = "up";
    static const char *TOUCH_SWIPE_DOWN = "down";
    static const char *TOUCH_SWIPE_LEFT = "left";
    static const char *TOUCH_SWIPE_RIGHT = "right";

    static const char *ROTARY_LEFT = "left";
    static const char *ROTARY_RIGHT = "right";

    static const char *BUTTON_SHORT = "short";
    static const char *BUTTON_LONG = "long";    

    static const char *FAN_DIRECTION_FORWARD = "forward";
    static const char *FAN_DIRECTION_REVERSE = "reverse";

#endif

#ifndef M_PI
    #define M_PI 3.1415926535
#endif
