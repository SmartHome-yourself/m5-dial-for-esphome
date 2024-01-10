```yaml
# example configuration:

substitutions:
  devicename: "m5-dial"
  display_name: "M5 Dial"
  wifi_ssid: !secret wifi_ssid
  wifi_password: !secret wifi_password
  m5_textsensor_id: "m5_textsensor_id"

packages:
  m5_tough_package:
    url: https://github.com/SmartHome-yourself/m5-dial-for-esphome
    ref: main
    file: shys-m5-dial.yaml
    refresh: 1h

text_sensor:
  - platform: shys_m5_dial
    name: "M5 Dial Status"
    id: m5_textsensor_id
    login_enabled: off

    devices:
      - entity: light.my_light1
        name: Light 1
      - entity: light.my_light2
        name: Light 2
      - entity: cover.my_cover1
        name: Cover 1
```
