#pragma once
#include "ha_device_mode_percentage.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModeClimateTemperature: public esphome::shys_m5_dial::HaDeviceModePercentage {
            protected:
                std::string hvac_mode = "none";
                std::string hvac_action = "off";  // NEW: Actual heating status
                float current_temperature = 0.0f;

                // Color configuration with defaults matching original hardcoded values
                uint16_t color_screen_bg = BLACK;
                uint16_t color_arc_base = DARKGREY;
                uint16_t color_arc_heating_active = RED;
                uint16_t color_arc_heating_idle = ORANGE;
                uint16_t color_arc_cooling = BLUE;
                uint16_t color_arc_rest = ORANGE;
                uint16_t color_current_temp_bg = DARKGREY;
                uint16_t color_current_temp_outline = WHITE;
                uint16_t color_current_temp_text = WHITE;
                uint16_t color_setpoint_bg = ORANGE;
                uint16_t color_setpoint_outline = WHITE;
                uint16_t color_setpoint_text = WHITE;
                uint16_t color_current_temp_marker = WHITE;
                uint16_t color_setpoint_marker = YELLOW;
                uint16_t color_badge_heating_bg = RED;
                uint16_t color_badge_cooling_bg = BLUE;
                uint16_t color_badge_idle_bg = ORANGE;
                uint16_t color_badge_off_bg = DARKGREY;
                uint16_t color_badge_heating_text = WHITE;
                uint16_t color_badge_cooling_text = WHITE;
                uint16_t color_badge_idle_text = WHITE;
                uint16_t color_badge_off_text = WHITE;
                uint16_t color_bottom_bar_bg = NAVY;
                uint16_t color_bottom_bar_text = WHITE;

                std::string getHvacMode(){
                    return this->hvac_mode;
                }

                void setHvacMode(const std::string& newMode){
                    this->hvac_mode = newMode;
                }

                std::string getHvacAction(){
                    return this->hvac_action;
                }

                void setHvacAction(const std::string& newAction){
                    this->hvac_action = newAction;
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
                    // Icon removed - not displayed in new layout
                    this->setUnit("°");
                }

                // Helper function to parse hex color string (e.g., "0xFF0000") to uint16_t RGB565
                uint16_t parseHexColor(const std::string& hexStr) {
                    if (hexStr.empty()) return 0;

                    // Remove "0x" prefix if present
                    std::string hex = hexStr;
                    if (hex.substr(0, 2) == "0x" || hex.substr(0, 2) == "0X") {
                        hex = hex.substr(2);
                    }

                    // Parse as 24-bit RGB
                    uint32_t rgb24 = std::strtoul(hex.c_str(), nullptr, 16);

                    // Convert RGB888 to RGB565
                    uint8_t r = (rgb24 >> 16) & 0xFF;
                    uint8_t g = (rgb24 >> 8) & 0xFF;
                    uint8_t b = rgb24 & 0xFF;

                    uint16_t rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);

                    ESP_LOGD("CLIMATE_COLOR", "Parsed color '%s' -> RGB888(r=%d,g=%d,b=%d) -> RGB565(0x%04X)",
                             hexStr.c_str(), r, g, b, rgb565);

                    return rgb565;
                }

                // Color setter methods
                void setColors(const std::string& screen_bg, const std::string& arc_base,
                              const std::string& arc_heating_active, const std::string& arc_heating_idle,
                              const std::string& arc_cooling, const std::string& arc_rest,
                              const std::string& current_temp_bg, const std::string& current_temp_outline,
                              const std::string& current_temp_text, const std::string& setpoint_bg,
                              const std::string& setpoint_outline, const std::string& setpoint_text,
                              const std::string& current_temp_marker, const std::string& setpoint_marker,
                              const std::string& badge_heating_bg, const std::string& badge_cooling_bg,
                              const std::string& badge_idle_bg, const std::string& badge_off_bg,
                              const std::string& badge_heating_text, const std::string& badge_cooling_text,
                              const std::string& badge_idle_text, const std::string& badge_off_text,
                              const std::string& bottom_bar_bg, const std::string& bottom_bar_text) {

                    if (!screen_bg.empty()) color_screen_bg = parseHexColor(screen_bg);
                    if (!arc_base.empty()) color_arc_base = parseHexColor(arc_base);
                    if (!arc_heating_active.empty()) color_arc_heating_active = parseHexColor(arc_heating_active);
                    if (!arc_heating_idle.empty()) color_arc_heating_idle = parseHexColor(arc_heating_idle);
                    if (!arc_cooling.empty()) color_arc_cooling = parseHexColor(arc_cooling);
                    if (!arc_rest.empty()) color_arc_rest = parseHexColor(arc_rest);
                    if (!current_temp_bg.empty()) color_current_temp_bg = parseHexColor(current_temp_bg);
                    if (!current_temp_outline.empty()) color_current_temp_outline = parseHexColor(current_temp_outline);
                    if (!current_temp_text.empty()) color_current_temp_text = parseHexColor(current_temp_text);
                    if (!setpoint_bg.empty()) color_setpoint_bg = parseHexColor(setpoint_bg);
                    if (!setpoint_outline.empty()) color_setpoint_outline = parseHexColor(setpoint_outline);
                    if (!setpoint_text.empty()) color_setpoint_text = parseHexColor(setpoint_text);
                    if (!current_temp_marker.empty()) color_current_temp_marker = parseHexColor(current_temp_marker);
                    if (!setpoint_marker.empty()) color_setpoint_marker = parseHexColor(setpoint_marker);
                    if (!badge_heating_bg.empty()) color_badge_heating_bg = parseHexColor(badge_heating_bg);
                    if (!badge_cooling_bg.empty()) color_badge_cooling_bg = parseHexColor(badge_cooling_bg);
                    if (!badge_idle_bg.empty()) color_badge_idle_bg = parseHexColor(badge_idle_bg);
                    if (!badge_off_bg.empty()) color_badge_off_bg = parseHexColor(badge_off_bg);
                    if (!badge_heating_text.empty()) color_badge_heating_text = parseHexColor(badge_heating_text);
                    if (!badge_cooling_text.empty()) color_badge_cooling_text = parseHexColor(badge_cooling_text);
                    if (!badge_idle_text.empty()) color_badge_idle_text = parseHexColor(badge_idle_text);
                    if (!badge_off_text.empty()) color_badge_off_text = parseHexColor(badge_off_text);
                    if (!bottom_bar_bg.empty()) color_bottom_bar_bg = parseHexColor(bottom_bar_bg);
                    if (!bottom_bar_text.empty()) color_bottom_bar_text = parseHexColor(bottom_bar_text);

                    ESP_LOGI("CLIMATE_COLOR", "Climate colors configured for entity %s", this->device.getEntityId().c_str());
                }

                void showClimateMenu(M5DialDisplay& display){
                    // Descale setpoint for display (215 → 21.5)
                    float setpointActual = this->getValue() / 10.0;

                    ESP_LOGI("DISPLAY", ">>> showClimateMenu - START");
                    ESP_LOGI("DISPLAY", ">>> SetPoint (target): %.1f° (scaled: %d)", setpointActual, this->getValue());
                    ESP_LOGI("DISPLAY", ">>> Current temp: %.1f°", this->current_temperature);
                    ESP_LOGI("DISPLAY", ">>> HVAC Mode: '%s'", this->hvac_mode.c_str());
                    ESP_LOGI("DISPLAY", ">>> HVAC Action: '%s'", this->hvac_action.c_str());

                    LovyanGFX* gfx = display.getGfx();

                    uint16_t height = gfx->height();
                    uint16_t width  = gfx->width();

                    gfx->setTextColor(MAROON);
                    gfx->setTextDatum(middle_center);

                    gfx->startWrite();                      // Secure SPI bus

                    display.clear(this->color_screen_bg);

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
                        gfx->fillArc(width / 2, height / 2, 115, 100, 150, lowerArc, this->color_arc_base);

                        // Gap between current temp and setpoint (highlight the difference)
                        uint16_t gapColor;
                        if (currentTempValue < setpointActual) {  // Compare actual values
                            // Need to heat up
                            gapColor = isHeating ? this->color_arc_heating_active : this->color_arc_heating_idle;
                        } else {
                            // Room is warmer than setpoint
                            gapColor = this->color_arc_cooling;
                        }
                        gfx->fillArc(width / 2, height / 2, 115, 100, lowerArc, higherArc, gapColor);

                        // Rest of arc (from setpoint to end)
                        gfx->fillArc(width / 2, height / 2, 115, 100, higherArc, 390, this->color_arc_rest);

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
                        gfx->fillCircle(currentX, currentY, 5, this->color_current_temp_marker);  // Small circle
                        gfx->drawCircle(currentX, currentY, 5, BLACK);  // Black outline

                        ESP_LOGI("DISPLAY", ">>> Drew current temp marker at angle %.1f (x=%d, y=%d)", currentTempOnArc, currentX, currentY);

                        // Setpoint marker (larger circle)
                        float setpointRad = setpointOnArc * PI / 180.0;
                        int setpointX = centerX + arcRadius * cos(setpointRad);
                        int setpointY = centerY + arcRadius * sin(setpointRad);
                        gfx->fillCircle(setpointX, setpointY, 8, this->color_setpoint_marker);  // Larger circle
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
                    gfx->fillCircle(leftX, tempY, circleRadius, this->color_current_temp_bg);
                    gfx->drawCircle(leftX, tempY, circleRadius, this->color_current_temp_outline);
                    gfx->drawCircle(leftX, tempY, circleRadius - 1, this->color_current_temp_outline);  // Thicker outline

                    // Current temp text
                    display.setFontsize(1.1);
                    gfx->setTextColor(this->color_current_temp_text);
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
                    gfx->fillCircle(rightX, tempY, circleRadius, this->color_setpoint_bg);
                    gfx->drawCircle(rightX, tempY, circleRadius, this->color_setpoint_outline);
                    gfx->drawCircle(rightX, tempY, circleRadius - 1, this->color_setpoint_outline);  // Thicker outline

                    // Setpoint text
                    display.setFontsize(1.1);
                    gfx->setTextColor(this->color_setpoint_text);
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

                    // CENTER: HVAC Action Status (actual heating state) - PROMINENT
                    display.setFontsize(1.2);  // Larger font for prominence
                    String actionLabel;
                    uint16_t actionColor;
                    uint16_t actionBgColor;

                    if (strcmp(this->hvac_action.c_str(), "heating") == 0) {
                        actionLabel = "Heating";
                        actionColor = this->color_badge_heating_text;
                        actionBgColor = this->color_badge_heating_bg;
                    } else if (strcmp(this->hvac_action.c_str(), "cooling") == 0) {
                        actionLabel = "Cooling";
                        actionColor = this->color_badge_cooling_text;
                        actionBgColor = this->color_badge_cooling_bg;
                    } else if (strcmp(this->hvac_action.c_str(), "idle") == 0) {
                        actionLabel = "Idle";
                        actionColor = this->color_badge_idle_text;
                        actionBgColor = this->color_badge_idle_bg;
                    } else {
                        // "off" or other
                        actionLabel = "Off";
                        actionColor = this->color_badge_off_text;
                        actionBgColor = this->color_badge_off_bg;
                    }

                    // Draw action badge in center (prominent)
                    int actionWidth = 100;  // Wider for longer words like "Heating"
                    int actionHeight = 30;  // Taller for larger font
                    int actionY = height / 2 + 28;  // Lowered by ~1/4 of height (was +20)

                    gfx->fillRoundRect(width / 2 - actionWidth/2, actionY - actionHeight/2,
                                       actionWidth, actionHeight, 5, actionBgColor);

                    gfx->setTextColor(actionColor);
                    gfx->drawString(actionLabel.c_str(),
                                    width / 2,
                                    actionY + 2);
                    ESP_LOGI("DISPLAY", ">>> Drew action status: %s at y=%d", actionLabel.c_str(), actionY);

                    gfx->setTextColor(MAROON);  // Reset text color

                    // BOTTOM: Device Name + Mode combined in full-width bar
                    int nameY = height / 2 + 78;  // Moved higher by ~60% of bar height
                    int nameWidth = width + 20;   // Full-width bar
                    int nameHeight = 28;

                    // Draw full-width background bar
                    gfx->fillRect(width / 2 - nameWidth/2, nameY - nameHeight/2,
                                  nameWidth, nameHeight, this->color_bottom_bar_bg);

                    // Combine device name with mode in parentheses
                    String modeText;
                    if (strcmp(this->hvac_mode.c_str(), "off") == 0) {
                        modeText = " (OFF)";
                    } else if (strcmp(this->hvac_mode.c_str(), "heat") == 0) {
                        modeText = " (Heat)";
                    } else if (strcmp(this->hvac_mode.c_str(), "cool") == 0) {
                        modeText = " (Cool)";
                    } else if (strcmp(this->hvac_mode.c_str(), "auto") == 0) {
                        modeText = " (Auto)";
                    } else {
                        modeText = "";
                    }

                    String nameAndMode = String(this->device.getName().c_str()) + modeText;

                    display.setFontsize(0.9);
                    gfx->setTextColor(this->color_bottom_bar_text);
                    gfx->drawString(nameAndMode.c_str(),
                                    width / 2,
                                    nameY + 2);
                    ESP_LOGI("DISPLAY", ">>> Drew device name + mode: %s at y=%d", nameAndMode.c_str(), nameY);

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

                    // Subscribe to hvac_action attribute (actual heating status)
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                optional<std::string>("hvac_action"),
                                [this](const std::string &state) {

                        ESP_LOGI("HA_API", ">>> HVAC_ACTION callback triggered for %s", this->device.getEntityId().c_str());
                        ESP_LOGI("HA_API", ">>> Received state (hvac_action attribute): '%s'", state.c_str());
                        ESP_LOGI("HA_API", ">>> Current stored hvac_action before update: '%s'", this->hvac_action.c_str());

                        this->setHvacAction(state.c_str());
                        ESP_LOGI("HA_API", ">>> Stored hvac_action after update: '%s'", this->hvac_action.c_str());
                        ESP_LOGI("HA_API", "Got HVAC Action %s for %s", state.c_str(), this->device.getEntityId().c_str());

                        this->displayRefreshNeeded = true;
                        ESP_LOGI("HA_API", ">>> Set displayRefreshNeeded = true");
                    });
                }


                bool onTouch(M5DialDisplay& display, uint16_t x, uint16_t y) override {
                    ESP_LOGI("HA_API", ">>> onTouch called: x=%d, y=%d", x, y);

                    uint16_t height = display.getGfx()->height();
                    uint16_t width = display.getGfx()->width();

                    // Calculate action badge bounds (same as in showClimateMenu)
                    int actionWidth = 100;
                    int actionHeight = 30;
                    int actionY = height / 2 + 28;

                    int badgeLeft = width / 2 - actionWidth / 2;
                    int badgeRight = width / 2 + actionWidth / 2;
                    int badgeTop = actionY - actionHeight / 2;
                    int badgeBottom = actionY + actionHeight / 2;

                    // Check if touch is within the action badge area
                    if (x >= badgeLeft && x <= badgeRight && y >= badgeTop && y <= badgeBottom) {
                        ESP_LOGI("HA_API", ">>> Touch detected on action badge - toggling mode");
                        ESP_LOGI("HA_API", ">>> Current hvac_mode: '%s'", this->getHvacMode().c_str());

                        // Toggle climate mode
                        if(strcmp(this->getHvacMode().c_str(), "off")==0){
                            ESP_LOGI("HA_API", ">>> Mode IS 'off', calling turnClimateOn");
                            haApi.turnClimateOn(this->device.getEntityId());
                        } else {
                            ESP_LOGI("HA_API", ">>> Mode is '%s' (NOT 'off'), calling turnClimateOff", this->getHvacMode().c_str());
                            haApi.turnClimateOff(this->device.getEntityId());
                        }

                        this->displayRefreshNeeded = true;
                        ESP_LOGI("HA_API", ">>> Set displayRefreshNeeded = true after touch toggle");

                        return true;  // Touch handled
                    }

                    // Touch outside badge - use default handling (arc adjustment)
                    ESP_LOGD("HA_API", ">>> Touch outside action badge - using default handler");
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
