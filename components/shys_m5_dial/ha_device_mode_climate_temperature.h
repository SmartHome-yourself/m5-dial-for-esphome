#pragma once
#include "ha_device_mode_percentage.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModeClimateTemperature: public esphome::shys_m5_dial::HaDeviceModePercentage {
            protected:
                std::string hvac_mode = "none";
                float current_temperature = 0.0f;

                std::string getHvacMode(){
                    return this->hvac_mode;
                }

                void setHvacMode(const std::string& newMode){
                    this->hvac_mode = newMode;
                }

                float getCurrentTemperature(){
                    return this->current_temperature;
                }

                void setCurrentTemperature(float temp){
                    this->current_temperature = temp;
                }

                void sendValueToHomeAssistant(int value) override {
                    // Descale the value before sending (215 → 21.5)
                    float actualTemp = value / 10.0;
                    ESP_LOGD("HA_API", "sendValueToHomeAssistant: scaled value=%i, actual=%.1f°", value, actualTemp);
                    ESP_LOGD("HA_API", "getEntityId() %s ", this->device.getEntityId().c_str());
                    haApi.setClimateTemperature(this->device.getEntityId(), actualTemp);
                }

            public:
                HaDeviceModeClimateTemperature(HaDevice& device) : HaDeviceModePercentage(device){
                    this->maxValue = 400;  // Scaled by 10 (40°C * 10 = 400)
                    this->setLabel("Temperature");
                    this->setIcon(FIREPLACE_IMG, 4900);
                    this->setUnit("°");
                }

                void showClimateMenu(M5DialDisplay& display){
                    // Descale setpoint for display (215 → 21.5)
                    float setpointActual = this->getValue() / 10.0;

                    ESP_LOGI("DISPLAY", ">>> showClimateMenu - START");
                    ESP_LOGI("DISPLAY", ">>> SetPoint (target): %.1f° (scaled: %d)", setpointActual, this->getValue());
                    ESP_LOGI("DISPLAY", ">>> Current temp: %.1f°", this->current_temperature);
                    ESP_LOGI("DISPLAY", ">>> HVAC Mode: '%s'", this->hvac_mode.c_str());

                    LovyanGFX* gfx = display.getGfx();

                    uint16_t height = gfx->height();
                    uint16_t width  = gfx->width();

                    gfx->setTextColor(MAROON);
                    gfx->setTextDatum(middle_center);

                    gfx->startWrite();                      // Secure SPI bus

                    display.clear();

                    // Calculate arc positions for setpoint and current temperature
                    // getValue() returns scaled int (e.g., 215 for 21.5°)
                    float setpointOnArc = (getMaxValue()==0?240:((float)240 / (this->getMaxValue() - this->getMinValue())) * (getValue() - this->getMinValue())) + 150;

                    // Calculate current temperature position - scale it to match
                    float currentTempValue = this->current_temperature;
                    float currentTempScaled = currentTempValue * 10.0;  // Scale to match getValue()
                    float currentTempOnArc = (getMaxValue()==0?240:((float)240 / (this->getMaxValue() - this->getMinValue())) * (currentTempScaled - this->getMinValue())) + 150;

                    // Clamp current temp to arc range
                    if (currentTempOnArc < 150) currentTempOnArc = 150;
                    if (currentTempOnArc > 390) currentTempOnArc = 390;
                    if (setpointOnArc < 150) setpointOnArc = 150;
                    if (setpointOnArc > 390) setpointOnArc = 390;

                    ESP_LOGI("DISPLAY", ">>> Arc positions - Current temp: %.1f°, Setpoint: %.1f° (scaled: %d)", currentTempValue, setpointActual, getValue());
                    ESP_LOGI("DISPLAY", ">>> Arc angles - Current temp: %.1f°, Setpoint: %.1f°", currentTempOnArc, setpointOnArc);

                    // Determine if heating is active
                    bool isHeating = (strcmp(this->hvac_mode.c_str(), "off") != 0);

                    ESP_LOGI("DISPLAY", ">>> isHeating: %s", isHeating ? "TRUE (ON)" : "FALSE (OFF)");

                    if(this->isBarActive()){
                        // Determine which temperature is higher to draw arcs correctly
                        float lowerArc = (currentTempOnArc < setpointOnArc) ? currentTempOnArc : setpointOnArc;
                        float higherArc = (currentTempOnArc < setpointOnArc) ? setpointOnArc : currentTempOnArc;

                        // Base arc (from start to current temp position)
                        gfx->fillArc(width / 2, height / 2, 115, 100, 150, lowerArc, DARKGREY);

                        // Gap between current temp and setpoint (highlight the difference)
                        uint16_t gapColor;
                        if (currentTempValue < setpointActual) {  // Compare actual values
                            // Need to heat up
                            gapColor = isHeating ? RED : ORANGE;
                        } else {
                            // Room is warmer than setpoint
                            gapColor = BLUE;
                        }
                        gfx->fillArc(width / 2, height / 2, 115, 100, lowerArc, higherArc, gapColor);

                        // Rest of arc (from setpoint to end)
                        gfx->fillArc(width / 2, height / 2, 115, 100, higherArc, 390, ORANGE);

                        ESP_LOGI("DISPLAY", ">>> Drew arcs - base to %.1f, gap %.1f to %.1f, rest to 390", lowerArc, lowerArc, higherArc);

                        // Draw marker circles on the arc
                        // Calculate circle positions
                        int centerX = width / 2;
                        int centerY = height / 2;
                        int arcRadius = 107;  // Middle of arc (between 100 and 115)

                        // Current temperature marker (small circle)
                        float currentTempRad = currentTempOnArc * PI / 180.0;
                        int currentX = centerX + arcRadius * cos(currentTempRad);
                        int currentY = centerY + arcRadius * sin(currentTempRad);
                        gfx->fillCircle(currentX, currentY, 5, WHITE);  // Small white circle
                        gfx->drawCircle(currentX, currentY, 5, BLACK);  // Black outline

                        ESP_LOGI("DISPLAY", ">>> Drew current temp marker at angle %.1f (x=%d, y=%d)", currentTempOnArc, currentX, currentY);

                        // Setpoint marker (larger circle)
                        float setpointRad = setpointOnArc * PI / 180.0;
                        int setpointX = centerX + arcRadius * cos(setpointRad);
                        int setpointY = centerY + arcRadius * sin(setpointRad);
                        gfx->fillCircle(setpointX, setpointY, 8, YELLOW);  // Larger yellow circle
                        gfx->drawCircle(setpointX, setpointY, 8, BLACK);   // Black outline

                        ESP_LOGI("DISPLAY", ">>> Drew setpoint marker at angle %.1f (x=%d, y=%d)", setpointOnArc, setpointX, setpointY);

                    } else {
                        gfx->fillArc(width / 2,
                                    height / 2,
                                    115,
                                    100,
                                    150,
                                    390,
                                    display.getBackgroundColor()
                                    );
                    }

                    // Icon removed - MODE badge provides status indication

                    // LEFT: Current Temperature (what you HAVE) - with circular background
                    int leftX = width / 2 - 45;   // Closer to center (was -50)
                    int tempY = height / 2 - 35;  // Moved LOWER, away from arc (was -65)
                    int circleRadius = 35;

                    // Current temp background circle
                    gfx->fillCircle(leftX, tempY, circleRadius, DARKGREY);
                    gfx->drawCircle(leftX, tempY, circleRadius, WHITE);
                    gfx->drawCircle(leftX, tempY, circleRadius - 1, WHITE);  // Thicker outline

                    // Current temp text
                    display.setFontsize(1.1);
                    gfx->setTextColor(WHITE);
                    char currentTempBuf[16];
                    snprintf(currentTempBuf, sizeof(currentTempBuf), "%.1f", this->current_temperature);
                    gfx->drawString(currentTempBuf,
                                    leftX,
                                    tempY - 5);  // Slightly above center
                    // Small "°" label below
                    display.setFontsize(0.7);
                    gfx->drawString("NOW",
                                    leftX,
                                    tempY + 15);  // Label below temp
                    ESP_LOGI("DISPLAY", ">>> Drew current temp: %s at x=%d, y=%d", currentTempBuf, leftX, tempY);

                    // RIGHT: SetPoint (what you WANT) - with circular background
                    int rightX = width / 2 + 45;  // Closer to center (was +50)

                    // Setpoint background circle - different color
                    gfx->fillCircle(rightX, tempY, circleRadius, ORANGE);
                    gfx->drawCircle(rightX, tempY, circleRadius, WHITE);
                    gfx->drawCircle(rightX, tempY, circleRadius - 1, WHITE);  // Thicker outline

                    // Setpoint text
                    display.setFontsize(1.1);
                    gfx->setTextColor(WHITE);
                    char setpointBuf[16];
                    snprintf(setpointBuf, sizeof(setpointBuf), "%.1f", setpointActual);
                    gfx->drawString(setpointBuf,
                                    rightX,
                                    tempY - 5);  // Slightly above center
                    // Small label below
                    display.setFontsize(0.7);
                    gfx->drawString("SET",
                                    rightX,
                                    tempY + 15);  // Label below temp
                    ESP_LOGI("DISPLAY", ">>> Drew setpoint: %s at x=%d, y=%d", setpointBuf, rightX, tempY);

                    gfx->setTextColor(MAROON);  // Reset text color

                    // HVAC Mode Status at bottom with background badge
                    display.setFontsize(1.0);
                    String modeLabel;
                    uint16_t modeColor;
                    uint16_t modeBgColor;

                    if (strcmp(this->hvac_mode.c_str(), "off") == 0) {
                        modeLabel = "OFF";
                        modeColor = WHITE;
                        modeBgColor = DARKGREY;
                    } else if (strcmp(this->hvac_mode.c_str(), "heat") == 0) {
                        modeLabel = "HEAT";
                        modeColor = WHITE;
                        modeBgColor = RED;
                    } else if (strcmp(this->hvac_mode.c_str(), "cool") == 0) {
                        modeLabel = "COOL";
                        modeColor = WHITE;
                        modeBgColor = BLUE;
                    } else {
                        modeLabel = this->hvac_mode.c_str();
                        modeColor = BLACK;
                        modeBgColor = ORANGE;
                    }

                    // Draw mode badge in center area (icon removed)
                    int labelWidth = 75;
                    int labelHeight = 26;
                    int modeY = height / 2 + 25;  // Lowered to avoid touching circles (was +10)

                    gfx->fillRoundRect(width / 2 - labelWidth/2, modeY - labelHeight/2,
                                       labelWidth, labelHeight, 5, modeBgColor);

                    gfx->setTextColor(modeColor);
                    gfx->drawString(modeLabel.c_str(),
                                    width / 2,
                                    modeY + 2);  // Slightly down for better centering
                    ESP_LOGI("DISPLAY", ">>> Drew mode: %s at y=%d", modeLabel.c_str(), modeY);

                    gfx->setTextColor(MAROON);  // Reset text color

                    // Device Name at bottom with full-width background bar
                    int nameY = height / 2 + 95;  // Moved up (was +105)
                    int nameWidth = width + 20;   // Wider than screen to create bar effect
                    int nameHeight = 28;          // Taller for more padding (was 22)

                    // Draw full-width background bar (no rounded corners for bar effect)
                    gfx->fillRect(width / 2 - nameWidth/2, nameY - nameHeight/2,
                                  nameWidth, nameHeight, NAVY);

                    display.setFontsize(0.9);
                    gfx->setTextColor(WHITE);
                    gfx->drawString(this->device.getName().c_str(),
                                    width / 2,
                                    nameY + 2);  // Adjusted for taller box
                    ESP_LOGI("DISPLAY", ">>> Drew device name: %s at y=%d", this->device.getName().c_str(), nameY);

                    gfx->endWrite();                      // Release SPI bus

                    this->displayRefreshNeeded = false;

                    ESP_LOGI("DISPLAY", ">>> showClimateMenu - END");
                }

                void refreshDisplay(M5DialDisplay& display, bool init) override {
                    this->showClimateMenu(display);
                    ESP_LOGD("DISPLAY", "Climate-Display-Modus");
                }

                void registerHAListener() override {
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                optional<std::string>(),
                                [this](const std::string &state) {

                        ESP_LOGI("HA_API", ">>> HVAC MODE callback triggered for %s", this->device.getEntityId().c_str());
                        ESP_LOGI("HA_API", ">>> Received state (mode): '%s'", state.c_str());
                        ESP_LOGI("HA_API", ">>> Current stored hvac_mode before update: '%s'", this->hvac_mode.c_str());
                        ESP_LOGI("HA_API", ">>> isValueModified: %s", this->isValueModified() ? "TRUE" : "FALSE");

                        if(this->isValueModified()){
                            ESP_LOGW("HA_API", ">>> Skipping mode update - value is modified");
                            return;
                        }

                        this->setHvacMode(state.c_str());
                        ESP_LOGI("HA_API", ">>> Stored hvac_mode after update: '%s'", this->hvac_mode.c_str());
                        ESP_LOGI("HA_API", "Got Mode %s for %s", state.c_str(), this->device.getEntityId().c_str());

                        this->displayRefreshNeeded = true;
                        ESP_LOGI("HA_API", ">>> Set displayRefreshNeeded = true");
                    });

                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                optional<std::string>("temperature"),
                                [this](const std::string &state) {

                        ESP_LOGI("HA_API", ">>> TEMPERATURE callback triggered for %s", this->device.getEntityId().c_str());
                        ESP_LOGI("HA_API", ">>> Received state (temperature attribute): '%s'", state.c_str());
                        ESP_LOGI("HA_API", ">>> isValueModified: %s", this->isValueModified() ? "TRUE" : "FALSE");

                        if(this->isValueModified()){
                            ESP_LOGW("HA_API", ">>> Skipping temperature update - value is modified");
                            return;
                        }

                        auto val = parse_number<float>(state);

                        if (!val.has_value()) {
                            this->setReceivedValue(0);
                            ESP_LOGD("HA_API", "No Temperature value in %s for %s", state.c_str(), this->device.getEntityId().c_str());
                        } else {
                            // Scale the value by 10 to support fractional temperatures
                            int scaledValue = (int)round(val.value() * 10.0);
                            this->setReceivedValue(scaledValue);
                            ESP_LOGI("HA_API", "Got Temperature value %.1f (stored as %i) for %s",
                                     val.value(), scaledValue, this->device.getEntityId().c_str());

                            this->displayRefreshNeeded = true;
                            ESP_LOGI("HA_API", ">>> Set displayRefreshNeeded = true");
                        }
                    });

                    // Subscribe to current_temperature attribute
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                optional<std::string>("current_temperature"),
                                [this](const std::string &state) {

                        ESP_LOGI("HA_API", ">>> CURRENT_TEMPERATURE callback triggered for %s", this->device.getEntityId().c_str());
                        ESP_LOGI("HA_API", ">>> Received state (current_temperature attribute): '%s'", state.c_str());
                        ESP_LOGI("HA_API", ">>> Current stored current_temperature before update: %.1f", this->current_temperature);

                        auto val = parse_number<float>(state);

                        if (!val.has_value()) {
                            this->setCurrentTemperature(0.0f);
                            ESP_LOGW("HA_API", ">>> Failed to parse current_temperature from '%s'", state.c_str());
                        } else {
                            this->setCurrentTemperature(val.value());
                            ESP_LOGI("HA_API", ">>> Stored current_temperature after update: %.1f", this->current_temperature);
                            ESP_LOGI("HA_API", "Got Current Temperature %.1f for %s", val.value(), this->device.getEntityId().c_str());

                            this->displayRefreshNeeded = true;
                            ESP_LOGI("HA_API", ">>> Set displayRefreshNeeded = true");
                        }
                    });
                }


                bool onTouch(M5DialDisplay& display, uint16_t x, uint16_t y) override {
                    return defaultOnTouch(display, x, y);        
                }

                bool onRotary(M5DialDisplay& display, const char * direction) override {
                    return defaultOnRotary(display, direction);
                }

                bool onButton(M5DialDisplay& display, const char * clickType) override {
                    ESP_LOGI("HA_API", ">>> onButton called: clickType='%s', entity='%s'",
                             clickType, this->device.getEntityId().c_str());

                    if (strcmp(clickType, BUTTON_SHORT)==0){
                        ESP_LOGI("HA_API", ">>> Button SHORT press detected");
                        ESP_LOGI("HA_API", ">>> Current stored hvac_mode: '%s'", this->getHvacMode().c_str());

                        if(strcmp(this->getHvacMode().c_str(), "off")==0){
                            ESP_LOGI("HA_API", ">>> Mode IS 'off', calling turnClimateOn");
                            haApi.turnClimateOn(this->device.getEntityId());
                        } else {
                            ESP_LOGI("HA_API", ">>> Mode is '%s' (NOT 'off'), calling turnClimateOff", this->getHvacMode().c_str());
                            haApi.turnClimateOff(this->device.getEntityId());
                        }

                        this->displayRefreshNeeded = true;
                        ESP_LOGI("HA_API", ">>> Set displayRefreshNeeded = true after button press");

                        return true;
                    }
                    ESP_LOGI("HA_API", ">>> Not a SHORT press, ignoring");
                    return false;
                }

        };
    }
}
