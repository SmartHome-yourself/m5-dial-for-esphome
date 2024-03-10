#### Language Selection:
[![en](https://img.shields.io/badge/lang-en-red.svg)](https://github.com/SmartHome-yourself/m5-dial-for-esphome/blob/main/README.md) 
[![de](https://img.shields.io/badge/lang-de-blue.svg)](https://github.com/SmartHome-yourself/m5-dial-for-esphome/blob/main/README.de.md)
  
# M5 Stack Dial Custom Component for ESPHome
![image](https://github.com/SmartHome-yourself/m5-dial-for-esphome/assets/705724/6d268fe4-ef71-40bb-b70c-797453b1d06b)
More information about the [M5 Stack Dial](https://shop.m5stack.com/products/m5stack-dial-esp32-s3-smart-rotary-knob-w-1-28-round-touch-screen?ref=smarthomeyourself)
  
This component turns the M5 Stack Dial into a universal remote control for Home Assistant.  
  
Currently supported entities are:  
- climate  
- cover  
- fan  
- light  
- switch  
  
## Video
[![M5 Stack Dial](http://img.youtube.com/vi/4dE7YONEYVk/0.jpg)](https://www.youtube.com/watch?v=4dE7YONEYVk "M5 Dial as Home Assistant Remote Control")

&nbsp;
  
# Example Configuration:
```yaml
substitutions:
  devicename: "m5-dial"
  wifi_ssid: !secret wifi_ssid
  wifi_password: !secret wifi_password  

packages:
  m5_tough_package:
    url: https://github.com/SmartHome-yourself/m5-dial-for-esphome
    ref: main
    file: shys-m5-dial.yaml
    refresh: 1h

shys_m5_dial:
  name: Dial
  screenOffTime: 45000
  rotaryStepWidth: 5
  longPressDuration: 1200
  font: FreeSans12pt7b
  font_factor: 1.2

  devices:
    lights:
      - entity: light.my_light1
        name: Light 1
        modes:
          dimm_mode:
            enable: true
            rotary_step_width: 10
          rgb_mode: 
            enable: true
            rotary_step_width: 30
          white_mode: 
            enable: true
            rotary_step_width: 100
            min_kelvin: 2500
            min_kelvin: 7000
            
      - entity: light.my_light2
        name: Light 2

    switches:
      - entity: switch.my_switch
        name: Switch 1
        
    climates:
      - entity: climate.my_climate1
        name: Heater
        modes:
          temp_mode:
            rotary_step_width: 1
            min_temperature: 4
            max_temperature: 30
            
    covers:
      - entity: cover.my_cover1
        name: Cover 1
        modes:
          position_mode:
            rotary_step_width: 5

    fans:
      - entity: fan.my_fan1
        name: Fan 1
        modes:
          speed_mode:
            changeable_direction: true
            rotary_step_width: 10
```
  
&nbsp;  
  
# Configuration
In substitutions, the `devicename` must be specified.  
The wireless network connection must be fully configured either through the normal `wifi:` section, or alternatively, you can simply specify the WLAN SSID and WLAN password in the substitutions. 
  
## Substitutions
```
substitutions:
  name: "m5-dial"
  wifi_ssid: !secret wifi_ssid
  wifi_password: !secret wifi_password
```
  
**name**  
The hostname of the device. *(default: m5-dial)*  

**wifi_ssid**  
The hostname of the device. *(default: !secret wifi_ssid)*  

**wifi_password**  
The hostname of the device. *(default: !secret wifi_password)*  

  
&nbsp;  
  
-----
## Custom-Component
-----
## General Attributes
General attributes are all parameters available directly under the custom component "shys_m5_dial".  
  
**Code:**
```
shys_m5_dial:
  name: Dial
  screenOffTime: 45000
  rotaryStepWidth: 5
  longPressDuration: 1200
```
  
**name**  
Sets the name of the component.  
  
**screenOffTime (optional)** *(Default: 30000)*  
Indicates after how many milliseconds the display automatically turns off.  
  
**rotaryStepWidth (optional)** *(Default: 10)*  
Specifies the general step width by which the value changes per step when using the rotary encoder.  
The value set here applies to all modes where no different step width is specified.  
  
**longPressDuration (optional)** *(Default: 1200)*  
Indicates the duration, in milliseconds, after which a button press is considered a long press.  
  
**font (optional)** *(Default: FreeMono12pt7b)*  
Specifies the font to be used.  
All available fonts are defined in a map in [globals.h](components/shys_m5_dial/globals.h).  
  
**font_factor (optional)** *(Default: 1)*  
Specifies the factor to be applied to the font size.  
*Valid values: 0.1 - 10.0*
  
&nbsp;  
  
------

## devices
  
Under Devices, all entities to be controlled with the M5 Dial are specified.  
The entities are specified in individual lists per device type (domain) such as lights, switches, covers, etc.  
  
**Code:**
```
shys_m5_dial:
  ...
  devices:
```
  
&nbsp;  
  
------
  
## **LIGHTS**
Under "devices - lights" all light entities are specified.  
Without further specifications, the lamp can only be turned on/off via the Dial.  
To adjust color, brightness, or white value, the respective mode must be activated.  
  
**Code:**
```
shys_m5_dial:
  ...
  devices:
    lights:
      - entity: light.my_light1
        name: Light 1  
```
  
**entity**  
Specifies the Light Entity ID from Home Assistant to be controlled.  
  
**name**  
The name of the entity displayed on the screen.  
  
## modes (optional)
#### dimm_mode
The Dimm Mode allows regulating the brightness of the light entity.  
When Dimm Mode is active, it replaces the standard On/Off Mode.  
Brightness can be controlled both via the rotary encoder and touch.  
A press on the button turns the light on/off.  
  
**Code:**
```
shys_m5_dial:
  ...
  devices:
    lights:
      - entity: light.my_light1
        name: Light 1  
        modes:
          dimm_mode:
            enable: true
            rotary_step_width: 10
```
  
**enable** *(Default: false)*  
Setting to true activates the mode for the entity.  
  
**rotary_step_width (optional)**  
Specifies the general step width by which the value changes per step when using the rotary

 encoder.  
The value set here overrides the general setting and applies only to the brightness mode of this light entity.  
*Valid values: 1 - 100*  
  
&nbsp;
  
#### white_mode
The White Mode allows regulating the white tone for the light entity in Kelvin.  
The white tone can be controlled both via the rotary encoder and touch.  
A press on the button turns the light on/off.  
  
**Code:**
```
shys_m5_dial:
  ...
  devices:
    lights:
      - entity: light.my_light1
        name: Light 1  
        modes:
          white_mode: 
            enable: true
            rotary_step_width: 100
            min_kelvin: 2500
            min_kelvin: 7000
```  
  
**enable** *(Default: false)*  
Setting to true activates the mode for the entity.  
  
**rotary_step_width (optional)**  
Specifies the general step width by which the value changes per step when using the rotary encoder.  
The value set here overrides the general setting and applies only to the white tone mode of this light entity.  
*Valid values: 1 - 500*  
  
**min_kelvin (optional)** *(Default: 2000)*  
Specifies the minimum value in Kelvin for white tone control. 
*Valid values: 1000 - 10000*  
  
**max_kelvin (optional)** *(Default: 6500)*  
Specifies the maximum value in Kelvin for white tone control. 
*Valid values: 1000 - 10000*  
  
&nbsp;
  
#### rgb_mode
The RGB Mode (color selection) allows selecting the color for the light entity.  
Color can be controlled both via the rotary encoder and touch.  
A press on the button turns the light on/off.
  
**Code:**
```
shys_m5_dial:
  ...
  devices:
    lights:
      - entity: light.my_light1
        name: Light 1  
        modes:
          rgb_mode: 
            enable: true
            rotary_step_width: 30
```
  
**enable** *Default: false*  
Setting to true activates the mode for the entity.
  
**rotary_step_width (optional)**  
Specifies the general step width by which the value changes per step when using the rotary encoder.  
The value set here overrides the general setting and applies only to the color selection mode of this light entity.  
*Valid values: 1 - 100*  
  
&nbsp;  
  
------
  
## **CLIMATES**
Under "devices - climates" all climate entities are specified.  
  
**Code:**
```
shys_m5_dial:
  ...
  devices:
    climates:
      - entity: climate.my_climate1
        name: Heater
```
    
**entity**  
Specifies the Climate Entity ID from Home Assistant to be controlled.  
  
**name**  
The name of the entity displayed on the screen.  
  
## modes (optional)
#### temp_mode
The Temp Mode allows regulating the temperature of the climate entity.  
Temperature can be controlled both via the rotary encoder and touch.  
A press on the button turns the heater on/off.  
  
**Code:**
```
shys_m5_dial:
  ...
  devices:
    climates:
      - entity: climate.my_climate1
        name: Heater
        modes:
          temp_mode:
            rotary_step_width: 1
            min_temperature: 4
            max_temperature: 30
```
  
**rotary_step_width (optional)** *Default: 1*  
Specifies the general step width by which the value changes per step when using the rotary encoder.  
For climates, 1 is the default, regardless of what is set as the general standard in the component.  
*Valid values: 1 - 100*  
  
**min_temperature (optional)** *Default: 4*  
Sets the minimum temperature that can be set for this climate entity on the Dial.  
  
**max_temperature (optional)** *Default: 30*  
Sets the maximum temperature that can be set for this climate entity on the Dial.
  
&nbsp;  
  
------
  
## **COVER**
Under "devices - cover" all cover entities are specified.  
  
**Code:**
```
shys_m5_dial:
  ...
  devices:
    covers:
      - entity: cover.my_cover1
        name: Cover 1
```
  
**entity**  
Specifies the Cover Entity ID from Home Assistant to be controlled.  
  
**name**  
The name of the entity displayed on the screen.  
  
## modes (optional)
#### position_mode
The Position Mode allows controlling the position of the cover entity.  
Position can be controlled both via the rotary encoder and touch.  

**Code:**
```
shys_m5_dial:
  ...
  devices:
    covers:
      - entity: cover.my_cover1
        name: Cover 1
        modes:
          position_mode:
            rotary_step_width: 5
```
  
**rotary_step_width (optional)**  
Specifies the general step width by which the value changes per step when using the rotary encoder.  
The value set here overrides the general setting and applies only to the position mode of this cover entity.  
*Valid values: 1 - 100*  
  
&nbsp;  
  
------
  
## **FANS**
Under "devices - cover" all fan entities are specified.  
  
**Code:**
```
shys_m5_dial:
  ...
  devices:
    fans:
      - entity: fan.my_fan1
        name: Fan 1
```
  
**entity**  
Specifies the Fan Entity ID from Home Assistant to be controlled.  
  
**name**  
The name of the fan displayed on the screen.  
  
## modes (optional)
#### speed_mode
The Speed Mode allows controlling the speed of the fan entity.  
Speed can be controlled both via the rotary encoder and touch.  
A press on the button turns the fan on/off.
  
**Code:**
```
shys_m5_dial:
  ...
  devices:
    fans:
      - entity: fan.my_fan1
        name: Fan 1
        modes:
          speed_mode:
            changeable_direction: true
            rotary_step_width: 10
```
  
**changeable_direction (optional)**  *(Default: false)*  
Indicates whether the fan has two directions of rotation and can therefore be switched between forward and reverse.  
False = Only one direction of rotation. Turning right increases the speed, turning left reduces the speed.
True = Two directions of rotation. Turning right increases the speed for forward, turning left reduces the speed for forward.  
However, you can now turn past 0 and thus change the direction of rotation.  
If the fan is running in reverse, turning left increases the speed and turning right reduces it.
*Valid values: true / false*  
  
**rotary_step_width (optional)**  
Specifies the general step width by which the value changes per step when using the rotary encoder.  
The value set here overrides the general setting and applies only to the speed mode of this fan entity.  
*Valid values: 1 - 100*  
  
  
&nbsp;  
  
&nbsp;  
  
------  
------  
  
## Advanced
The following attributes are present, but generally should not need to be changed.  
  
**send_value_delay**  
Specifies the delay in milliseconds to wait after a value change before

 transmitting the change to Home Assistant.  
This is especially important when using the rotary encoder to avoid generating unnecessary API calls. *(Default: 1200)*
  
**send_value_lock**  
Specifies how long to wait after an API call before the next API call can be made. *(Default: 3000)*  
  
  
------  
------  
    
&nbsp;
  