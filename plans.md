# M5Dial Climate Control Screen Analysis

This document outlines the findings of an investigation into why the M5Dial climate control screen does not display the current temperature or the current HVAC mode (On/Off).

## Summary of Findings

The investigation into the `shys_m5_dial` component revealed the following:

1.  **Missing Data Subscription**: The component does not subscribe to the `current_temperature` attribute from the Home Assistant climate entity. It only subscribes to the `temperature` attribute (the target temperature setpoint) and the entity's main state (which provides the `hvac_mode` like "heat" or "off"). This is done in `components/shys_m5_dial/ha_device_mode_climate_temperature.h`.

2.  **Incomplete Display Logic**: The screen is rendered by the `showPercentageMenu` function in `components/shys_m5_dial/ha_device_mode_percentage.h`. This function is generic and designed for any device that uses a percentage-based control. It currently displays:
    *   The target temperature.
    *   A hardcoded label ("Temperature").
    *   An icon.
    *   The device name.

    It lacks the logic to display the current temperature or the HVAC mode.

3.  **Available Data**: The `hvac_mode` (e.g., "off", "heat") is already being fetched from Home Assistant and is available within the `HaDeviceModeClimateTemperature` class. The `onButton` logic uses this to toggle the climate entity on and off.

## Proposed Solution

To display the missing information, the following changes are required:

1.  **Subscribe to `current_temperature`**:
    *   In `components/shys_m5_dial/ha_device_mode_climate_temperature.h`, add a new `subscribe_home_assistant_state` call within the `registerHAListener` function to retrieve the `current_temperature` attribute.
    *   Store this value in a new member variable within the `HaDeviceModeClimateTemperature` class.

2.  **Update Display Logic**:
    *   Modify the `showPercentageMenu` function in `components/shys_m5_dial/ha_device_mode_percentage.h` to display the new information.
    *   Alternatively, and for a cleaner design, create a new `showClimateMenu` function inside `ha_device_mode_climate_temperature.h`. This new function could call the base `showPercentageMenu` and then draw the additional climate-specific information (current temperature and HVAC mode) on the screen.
    *   The `refreshDisplay` method in `ha_device_mode_climate_temperature.h` would then need to be updated to call `showClimateMenu` instead of `showPercentageMenu`.

## Climate Icon Source

The source of the icon used on the climate screen has been identified:

1.  **Icon Assignment**: The icon is assigned within the constructor of the `HaDeviceModeClimateTemperature` class in the file `components/shys_m5_dial/ha_device_mode_climate_temperature.h`. The specific line is:
    ```cpp
    this->setIcon(HEATER_ON_IMG, 4900);
    ```

2.  **Icon Definition**: The icon data itself, `HEATER_ON_IMG`, is defined in `components/shys_m5_dial/global_image_data.h`.

3.  **Source Format**: The icon is not a standard image file. It is a hardcoded C++ array of `unsigned short` values that represents the raw pixel data of the image, embedded directly into the firmware.

