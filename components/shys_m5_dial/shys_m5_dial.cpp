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
            M5.delay(1);
            M5Dial.update();
            esphome::delay(1);
            ShysM5Dial::doLoop();
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

    
        /**
         * @brief Add Menu device - moved from .h to .cpp
         * 
         * @param entity_id 
         * @param name 
         * @param modes 
         */
        void ShysM5Dial::addMenu(const std::string& entity_id, const std::string& name, const std::string& modes){
            HaDeviceMenu* menu = new HaDeviceMenu(entity_id, name, modes);
            addDevice(menu);

            menu->setM5DialObj(this);
        }


        /**
         * @brief initialize the pointer to the main ShysM5Dial object
         * 
         * @param m5D 
         */
        void HaDeviceModeMenuTest::setM5DialObj(ShysM5Dial * m5D) {
            this->M5DialObj=m5D;
            if (this->M5DialObj != NULL)  this->NumberOfDevices=this->M5DialObj->getDeviceCount();
            else this->NumberOfDevices=0;
        }


        /**
         * @brief Draw only inner circle, arrow and new text
         * 
         * @param display 
         * @param currentValue 
         */
                              
        void HaDeviceModeMenuTest::refreshInnerMenu(M5DialDisplay& display, uint16_t currentValue) {
            LovyanGFX* gfx = display.getGfx();
            uint16_t height = gfx->height();
            uint16_t width  = gfx->width();
            int size=width/6-2;                             //** size of button
            int degr = ((12-currentValue)*30+180)%360;      //** degress to selected button

            char text[40];
            snprintf(text, sizeof(text), "refreshInnerMenu( ,%d) = %d °", currentValue, degr);
            ESP_LOGI("DISPLAY", text);

            gfx->setTextColor(BLACK);
            gfx->setTextDatum(middle_center);

            gfx->startWrite();                      // Secure SPI bus

            // void fillCircle      ( int32_t x, int32_t y                      , int32_t r, const T& color) { setColor(color); fillCircle   (x, y      , r); }
            gfx->fillCircle(width/2, width/2, width/2-6*size/4+1, 0xde74);    // 0xfeb0); //

            // draw arrow to selected item
            coord c1= getDegreeCoord(gfx,width/2-6*size/4,degr);
            coord c2= getDegreeCoord(gfx,width/2-8*size/4, degr-10);
            coord c3= getDegreeCoord(gfx,width/2-8*size/4, degr+10);
            // fillTriangle    ( int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, const T& color)  { setColor(color); fillTriangle(x0, y0, x1, y1, x2, y2); }
            gfx->fillTriangle(c1.x, c1.y, c2.x, c2.y, c3.x, c3.y, 0xc960);

            display.setFontsize(1);
            gfx->drawString((String(currentValue)).c_str(),
                            width / 2,
                            height / 2 - 30);
    #if true
            if (currentValue-1 < NumberOfDevices) {
                gfx->drawString(M5DialObj->getDeviceNameAt(currentValue-1).c_str(),
                                width / 2,
                                height / 2);
            }
    #endif 

            display.setFontsize(0.5);
            gfx->setTextDatum(middle_center);
            gfx->drawString(this->device.getName().c_str(),
                            width / 2,
                            height / 2 + 50);

            // void fillArc         ( int32_t x, int32_t y, int32_t r0, int32_t r1, float angle0, float angle1, const T& color) { setColor(color); fillEllipseArc( x, y, r0, r1, r0, r1, angle0, angle1); }

            gfx->endWrite();                      // Release SPI bus
        }

        /**
         * @brief Process onButton event - if possible, jump to selected device
         * 
         * @param display 
         * @param clickType 
         * @return true 
         * @return false 
         */
        bool HaDeviceModeMenuTest::onButton(M5DialDisplay& display, const char * clickType)  {
            if (strcmp(clickType, BUTTON_SHORT)==0){
                
                if (this->value -1  < M5DialObj->getDeviceCount()) M5DialObj->setCurrentDevice(this->value-1);
                return true;
            } else if (strcmp(clickType, BUTTON_LONG)==0){
                return true;
            } 
            return false;
        }


    }

}