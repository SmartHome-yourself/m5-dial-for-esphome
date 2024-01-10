#include "esphome/core/log.h"
#include "shys_m5_dial.h"

namespace esphome
{
    namespace shys_m5_dial
    {

        static const char *TAG = "shys_m5_dial";

        /**
         * @brief SETUP
         *
         * Initialisierung
         */
        void ShysM5Dial::setup()
        {
            ShysM5Dial::initDevice();
            ESP_LOGI("log", "%s", "M5 is initialized");
        }

        /**
         * @brief LOOP
         *
         * Standard Loop
         */
        void ShysM5Dial::loop()
        {
            M5Dial.update();
            ShysM5Dial::validateTouchTimeout();

            ShysM5Dial::refreshData();

            ShysM5Dial::handleRotary();
            ShysM5Dial::handleButtonPress();
            ShysM5Dial::handleTouch();

            ShysM5Dial::refreshDisplay(&M5Dial.Display, false);
            esphome::delay(1);
        }

        /**
         * @brief dump_config
         *
         * Ausgabe der aktuellen Konfiguration im Log nach Initialisierung
         */
        void ShysM5Dial::dump_config()
        {
            ESP_LOGCONFIG(TAG, "-----------------------------------");
            ESP_LOGCONFIG(TAG, "Shys M5 Dial");
            ESP_LOGCONFIG(TAG, "-----------------------------------");
        }

    }
}