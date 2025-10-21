#### Language Selection:
[![en](https://img.shields.io/badge/lang-en-red.svg)](README.md) 
[![de](https://img.shields.io/badge/lang-de-blue.svg)](README.de.md)
  
# M5 Stack Dial Custom Component for ESPHome
![image](https://github.com/SmartHome-yourself/.github/blob/main/profile/assets/m5-dial-screens.png)  
More information about the [M5 Stack Dial](https://shop.m5stack.com/products/m5stack-dial-esp32-s3-smart-rotary-knob-w-1-28-round-touch-screen?ref=smarthomeyourself)
  
This component turns the M5 Stack Dial into a universal remote control for Home Assistant.  
  
Currently supported entities are:  
- climate  
- cover  
- fan  
- input_number
- light  
- lock  
- media_player  
- number  
- switch  
- timer   
  
## Video
[![M5 Stack Dial](http://img.youtube.com/vi/4dE7YONEYVk/0.jpg)](https://www.youtube.com/watch?v=4dE7YONEYVk "M5 Dial as Home Assistant Remote Control")
  
&nbsp;  
  
## Discord
For quick exchanges, suggestions, and updates on the current development status, our Discord is also available.  
https://discord.com/channels/293705077812625408/1217503791541387294  
  
&nbsp;  
  
# Example Configuration:
```yaml
substitutions:
  name: "m5-dial"
  wifi_ssid: !secret wifi_ssid
  wifi_password: !secret wifi_password  

packages:
  m5_tough_package:
    url: https://github.com/SmartHome-yourself/m5-dial-for-esphome
    ref: main
    file: shys-m5-dial.yaml
    refresh: 1h

time:
  - platform: homeassistant
    id: homeassistant_time
    timezone: Europe/Berlin
    
shys_m5_dial:
  name: Dial
  time_component: homeassistant_time
  screen_off_time: 45000
  screensaver: clock
  rotary_step_width: 5
  long_press_duration: 1200
  font: default
  font_factor: 1

  display_rotate: 2
  
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
            max_kelvin: 7000
            
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
      - entity: climate.my_climate1
        name: Air Conditioning
        modes:
          temp_mode:
            rotary_step_width: 1
            min_temperature: 16
            max_temperature: 24
          fan_mode:
            rotary_step_width: 1
            modes:
              - auto
              - low
              - high

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

    media_player:
      - entity: media_player.my_player1
        name: MediaPlayer 1
        modes:
          play_mode:
            rotary_step_width: 10
          source_mode:
            rotary_step_width: 1
            sources:
              - name: 1Live
                content_id: 1Live
                content_type: TUNEIN
              - name: WDR2
                content_id: "http://wdr-wdr2-bergischesland.icecast.wdr.de/wdr/wdr2/bergischesland/mp3/128/stream.mp3"
                content_type: custom

    lock:
      - entity: lock.my_lock1
        name: Lock 1
        modes:
          lock_mode:
            rotary_step_width: 1
            open_on_button: false

    number:
      - entity: number.my_number
        name: Number 1

      - entity: input_number.my_input_number
        name: Input Number 1

    timer:
      - entity: timer.my_timer
        name: Timer 1

```
  
## DEV Branch
To use the Dev Branch, the ref entry in the packages section must be changed to dev.  
Additionally, the configuration must be expanded to include the external_component section, where ref must also be set as dev.  
This would look like this, for example:  
```
packages:
  m5_tough_package:
    url: https://github.com/SmartHome-yourself/m5-dial-for-esphome
    ref: dev
    file: shys-m5-dial.yaml
    refresh: 1s

external_components:
  - source:
      type: git
      url: https://github.com/SmartHome-yourself/m5-dial-for-esphome/
      ref: dev
    components: [shys_m5_dial]
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
  time_component: homeassistant_time
  screenOffTime: 45000
  screensaver: clock
  rotaryStepWidth: 5
  longPressDuration: 1200
  font: default
  font_factor: 1  
```
  
**name**  
Sets the name of the component.  
  
**time_component**  
Here, the ID of a Time component must be specified, through which the Dial can determine the date and current time.  
I would recommend using the Home Assistant time here, as this component is also required for timer control.  
  
**screenOffTime (optional)** *(Default: 30000)*  
Specifies after how many milliseconds the display automatically turns off.  
  
**screensaver (optional)** *(Default: clock)*  
Specifies whether a screensaver should be used or not.  
*Valid values: clock, off*
  
**rotaryStepWidth (optional)** *(Default: 10)*  
Specifies the general step width by which the value changes per step when using the rotary encoder.  
The value set here applies to all modes where no different step width is specified.  
  
**longPressDuration (optional)** *(Default: 1200)*  
Indicates the duration, in milliseconds, after which a button press is considered a long press.  
  
**font (optional)** *(Default: default)*  
Specifies the font to be used.  
The default "default" font is a font generated by me, which also includes umlauts and various special characters.  
All alternative available fonts such as FreeSans12pt7b are defined in a map in [globals.h](components/shys_m5_dial/globals.h).  
However, the alternative fonts do not support umlauts and special characters!  
  
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
            min_brightness: 0
            max_brightness: 100
            rotary_step_width: 10
```
  
**enable** *(Default: false)*  
Setting to true activates the mode for the entity.  
  
**min_brightness (optional)** *(Default: 0)*  
Specifies the maximum brightness value to which dimming can be lowered.  
Useful if light sources only emit light from 10% brightness and may flicker below that.  
*Valid values 0 - 100*  
  
**max_brightness (optional)** *(Default: 100)*  
Specifies the maximum brightness value to which dimming can be adjusted via the Dial.  
*Valid values 0 - 100*  
  
**rotary_step_width (optional)**  
Specifies the general step width by which the value changes per step when using the rotary encoder.  
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
            max_kelvin: 7000
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
Touching the color area in the middle switches the lamp back to white mode.  
  
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
  
#### fan_mode
The Fan Mode allows controlling the fan mode of the Climate entity.  
The mode is selected using the rotary encoder and activated by pressing the button.  
  
Typically, the entry `fan_mode: {}` under `modes:` is sufficient to activate this fan control.  
The mode automatically reads the possible values from the `fan_modes` attribute of the entity.  
  
**Code:**
```
shys_m5_dial:
  ...
  devices:
    climates:
      - entity: climate.my_climate2
        name: Heater
        modes:
          fan_mode: {}
      - entity: climate.my_climate1
        name: Heater
        modes:
          fan_mode:
            rotary_step_width: 1
            modes:
              - auto
              - low
              - medium
              - high
```
  
**rotary_step_width (optional)** *Default: 1*  
Specifies the general step width by which the value changes per step when using the rotary encoder.  
Even for the Fan Mode, 1 is the default, regardless of the general standard set in the component.  
*Valid values 1 - 5*  
  
**modes (optional)**  
Modes allow specifying a list of available modes.  
If modes are specified here, automatic detection of modes will not occur anymore.  
This parameter can be used to restrict the available modes or if the climate entity does not provide its modes via the "fan_modes" attribute.  
  
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
Under "devices - fans" all fan entities are specified.  
  
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
  
------
  
## **MEDIA PLAYER**
Under "devices - media_player" all MediaPlayer entities are specified.  
  
**Code:**
```
shys_m5_dial:
  ...
  devices:
    media_player:
      - entity: media_player.my_player1
        name: MediaPlayer 1
```
  
**entity**  
Specifies the MediaPlayer Entity ID from Home Assistant to be controlled.  
  
**name**  
The name of the MediaPlayer displayed on the screen.  
  
## modes (optional)
#### play_mode
The Play Mode allows adjusting the volume via the rotary encoder, as well as starting/pausing playback or skipping to the next or previous track via touch.  
A press on the button toggles between play and pause.
  
**Code:**
```
shys_m5_dial:
  ...
  devices:
    media_player:
      - entity: media_player.my_player1
        name: MediaPlayer 1
        modes:
          play_mode:
            rotary_step_width: 10
```

**rotary_step_width (optional)**  
Specifies the general step width by which the value changes per step when using the rotary encoder.  
The value set here overrides the general setting and applies only to the play mode of this MediaPlayer entity.  
*Valid values: 1 - 100*  
  
  
&nbsp;  
  
#### source_mode
The Source Mode allows selecting a source from the stored "Sources" using the rotary encoder for playback.  
A press on the button starts playback of the selected source.  
  
**Code:**
```
shys_m5_dial:
  ...
  devices:
    media_player:
      - entity: media_player.my_player1
        name: MediaPlayer 1
        modes:
          play_mode:
            rotary_step_width: 10
          source_mode:
            rotary_step_width: 1
            sources:
              - name: 1Live
                content_id: 1Live
                content_type: TUNEIN
              - name: WDR2
                content_id: "http://wdr-wdr2-bergischesland.icecast.wdr.de/wdr/wdr2/bergischesland/mp3/128/stream.mp3"
                content_type: custom
```

**rotary_step_width (optional)** *(Default: 1)*  
Specifies the general step width by which the value changes per step when using the rotary encoder.  
For source_mode of media_player, 1 is the default, regardless of what is set as the general standard in the component.  
*Valid values: 1 - 100*  
  
**sources (optional)**
A list of audio sources available for playback selection.  
Entries must always include the attributes "name", "content_id", and "content_type"!  

Example for Amazon Echo (Alexa):  
```
              - name: 1Live
                content_id: 1Live
                content_type: TUNEIN

              - name: Metallica
                content_id: metallica
                content_type: AMAZON_MUSIC

              - name: Nothing else matters
                content_id: play metallica nothing else matters
                content_type: custom				
```
  
Example for DLNA Player:  
```
              - name: 1Live
                content_id: "https://wdr-1live-live.icecastssl.wdr.de/wdr/1live/live/mp3/128/stream.mp3"
                content_type: custom
```
  
&nbsp;  
  
------ 
  
## **LOCKS**
Under "devices - lock" all lock entities are listed.  
  
**Code:**
```
shys_m5_dial:
  ...
  devices:
    lock:
      - entity: lock.my_lock1
        name: Lock 1
```
  
**entity**  
Specifies the lock entity ID from Home Assistant to be controlled.  
  
**name**  
The name of the lock displayed on the screen.  
  
## modes (optional)
#### lock_mode
With the lock mode, the state of the lock entity can be controlled.  
The lock can be unlocked, locked, and opened using the rotary encoder.
A press on the button usually unlocks or locks the lock depending on the current state.
However, if *open_on_button* is set to *true*, pressing the button will open the lock directly instead of just unlocking it.  
  
**Code:**
```
shys_m5_dial:
  ...
  devices:
    lock:
      - entity: lock.my_lock1
        name: Lock 1
        modes:
          lock_mode:
            open_on_button: true
            rotary_step_width: 10
```
  
**open_on_button (optional)**  *(Default: false)*  
Setting to true will directly open the lock when pressing the button instead of just unlocking it.  
If the parameter is set to false (or not specified), the lock will only be unlocked but not fully opened.
*Valid values: true / false*  
  
**rotary_step_width (optional)**  *(Default: 1)*  
Specifies the general step width by which the value changes when using the rotary encoder per step.  
For lock mode of lock, 1 is the default value, regardless of what is set as the general standard in the component.  
If a step width of 2 is specified here, turning the encoder will not only unlock the lock but also automatically fully open it.  
*Valid values: 1 - 2*  
  
&nbsp;  
  
------
  
## **Number / Input_Number**
Under "devices - number," all Number and Input-Number entities are listed.  
  
**Code:**
```
shys_m5_dial:
  ...
  devices:
    number:
      - entity: number.my_number
        name: Number 1
        modes:
          lock_mode:
            unit: mm
            rotary_step_width: 10
            
      - entity: input_number.my_input_number
        name: Input Number 1
```
  
**entity**  
Specifies the entity ID of the number from Home Assistant to be controlled.  
Both entities from the Number and Input-Number domains are supported here.
  
**name**  
The name of the entity displayed on the display.  
  
## modes (optional)
#### value_mode
The Value Mode allows controlling the value of the Number entity.  
The value can be set both using the rotary encoder and via touch.  

**Code:**
```
shys_m5_dial:
  ...
  devices:
    number:
      - entity: number.my_number
        name: Number 1
        modes:
          lock_mode:
            unit: mm
            rotary_step_width: 10
```
  
**unit (optional)** *(Default: '')*  
Specifies the unit to be displayed after the value of the number.  
  
**rotary_step_width (optional)**  
Specifies the general step width by which the value changes per step when using the rotary encoder.  
The value set here overrides the globally set value and only applies to the Value Mode of this Number entity.  
*Valid values 1 - 500*  
  
&nbsp;  
  
------
  
## **Timer**
Under "devices - timer," all timer entities to be controlled via the Dial are listed.  
  
**Code:**
```
shys_m5_dial:
  ...
  devices:
    timer:
      - entity: timer.test_timer
        name: Test timer
        modes:
          timer_mode:
            rotary_step_width: 10        
```
  
**entity**  
Specifies the entity ID of the timer from Home Assistant to be controlled.  

**name**  
The name of the entity displayed on the display.  
  
## modes (optional)
#### timer_mode
The Timer Mode allows controlling the position of the timer entity.  
The position can be controlled both using the rotary encoder and via touch.  

**Code:**
```
shys_m5_dial:
  ...
  devices:
    timer:
      - entity: timer.my_timer
        name: Timer 1
        modes:
          timer_mode:
            rotary_step_width: 10
```
  
**rotary_step_width (optional)**  
Specifies the general step width by which the value changes per step when using the rotary encoder.  
The value set here overrides the globally set value and only applies to the Timer Mode of this Timer entity.  
*Valid values 1 - 500*  
  
&nbsp;  
  
&nbsp;  
  
------  
------  
  
## Advanced
The following attributes are present, but generally should not need to be changed.  
  
**send_value_delay**  
Specifies the delay in milliseconds to wait after a value change before transmitting the change to Home Assistant.  
This is especially important when using the rotary encoder to avoid generating unnecessary API calls. *(Default: 1200)*
  
**send_value_lock**  
Specifies how long to wait after an API call before the next API call can be made. *(Default: 3000)*  
  
  
------  
------  
  
&nbsp;  
  
&nbsp;  
  
  
# Services in Home Assistant
This integration provides various services in Home Assistant to control the Dial, for example, via automations.  
The services are automatically available immediately after integrating the Dial into Home Assistant.  
  
All services start with: **esphome.YOUR_DIAL_NAME_**
  
### select_device
This service allows switching the currently active device on the Dial.  
```
service: esphome.m5_dial_select_device
data:
  entity_id: light.my_light
```
##### Parameters
**entity_id**
Specifies the entity ID to be selected as the active entity on the Dial.
*(The entity must be configured within the component!)*
  
### lock_device
This service allows locking the operation of a single entity for the Dial.  
The service expects the entity ID as a parameter. (The entity must be configured within the component!)
```
service: esphome.m5_dial_lock_device
data:
  entity_id: light.my_light
```
##### Parameters
**entity_id**
Specifies the entity ID for which the operation via the Dial should be locked.
*(The entity must be configured within the component!)*
  
### unlock_device
This service allows unlocking the operation lock on the Dial for an entity.  
The service expects the entity ID as a parameter.  (The entity must be configured within the component!)
```
service: esphome.m5_dial_unlock_device
data:
  entity_id: light.my_light
```
##### Parameters
**entity_id**
Specifies the entity ID for which the operation via the Dial should be unlocked.
*(The entity must be configured within the component!)*
  
&nbsp;

------  
------  
    
&nbsp;
  
  
