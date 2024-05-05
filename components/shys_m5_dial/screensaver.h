#pragma once
#include "m5dial_display.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class M5DialDisplay;
        class Screensaver {
            protected:

            public:
                virtual void show(M5DialDisplay& display, bool init) = 0;
        };

    }
}