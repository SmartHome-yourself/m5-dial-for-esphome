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

    static const uint16_t DEVICE_MODE_BRIGHTNESS = 0;
    static const uint16_t DEVICE_MODE_COLOR = 1;
    static const uint16_t DEVICE_MODE_POSITION = 2;
    static const uint16_t DEVICE_MODE_POWER = 3;

    static std::vector<uint16_t> LIGHT_MODES = {DEVICE_MODE_BRIGHTNESS, DEVICE_MODE_COLOR};
    static std::vector<uint16_t> COVER_MODES = {DEVICE_MODE_POSITION};
    static std::vector<uint16_t> FAN_MODES = {DEVICE_MODE_POWER};

    static constexpr size_t lightModeListSize = sizeof(LIGHT_MODES) / sizeof(LIGHT_MODES[0]);
    static constexpr size_t coverModeListSize = sizeof(COVER_MODES) / sizeof(COVER_MODES[0]);
    static constexpr size_t fanModeListSize = sizeof(FAN_MODES) / sizeof(FAN_MODES[0]);

    static const char *TOUCH_SWIPE_NONE = "none";
    static const char *TOUCH_SWIPE_UP = "up";
    static const char *TOUCH_SWIPE_DOWN = "down";
    static const char *TOUCH_SWIPE_LEFT = "left";
    static const char *TOUCH_SWIPE_RIGHT = "right";

#endif

#ifndef M_PI
    #define M_PI 3.1415926535
#endif
