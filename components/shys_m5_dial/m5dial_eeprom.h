#pragma once
#include "EEPROM.h"

#define EEPROM_SIZE 32

namespace esphome
{
    namespace shys_m5_dial
    {
        class M5DialEEPROM {
            protected:

            public:
                /**
                 * Text in EEProm ablegen
                 */
                void writeStringToEEPROM(int addrOffset, const String &strToWrite){
                    EEPROM.begin(EEPROM_SIZE);

                    byte len = strToWrite.length();
                    EEPROM.write(addrOffset, len);
                    for (int i = 0; i < len; i++){
                        EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
                    }
                    EEPROM.commit();
                    EEPROM.end();
                }

                /**
                 * Text aus EEProm auslesen
                 */
                String readStringFromEEPROM(int addrOffset){
                    String retVal = "";

                    EEPROM.begin(EEPROM_SIZE);

                    int newStrLen = EEPROM.read(addrOffset);
                    if(!isnan(newStrLen)){
                        char data[newStrLen + 1];
                        for (int i = 0; i < newStrLen; i++){
                            data[i] = EEPROM.read(addrOffset + 1 + i);
                        }
                        data[newStrLen] = '\0';
                        
                        EEPROM.end();
                        retVal = String(data);
                    }
                    
                    return retVal;
                }

        };
    }
}