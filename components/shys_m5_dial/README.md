```yaml
# example configuration:

substitutions:
  devicename: "m5-dial"
  display_name: "M5 Dial"
  wifi_ssid: !secret wifi_ssid
  wifi_password: !secret wifi_password

packages:
  m5_tough_package:
    url: https://github.com/SmartHome-yourself/m5-dial-for-esphome
    ref: main
    file: shys-m5-dial.yaml
    refresh: 1h

text_sensor:
  - platform: shys_m5_dial
    name: dial
    screenOffTime: 45000
    rotaryStepWidth: 5
    longPressDuration: 1200
    sendValueDelay: 1200
    receiveValueDelay: 3000

    devices:
      - entity: light.nanoleaf
        name: "Nanoleaf"
        rgb_enabled: true
        dimm_enabled: true
      - entity: light.rgb_led_pc_backlight_daniel
        name: "Moodlight PC"
        rgb_enabled: true
      - entity: light.shys_rgbw_smd_stream_panel_links
        name: "Panel Links"
        dimm_enabled: true
      - entity: light.shys_rgbw_smd_stream_panel_rechts
        name: "Panel Rechts"
        dimm_enabled: false

```
