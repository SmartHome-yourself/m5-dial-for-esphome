# Journaling my goals and archievements

## first steps

* ✅ compile original example
  * took a while to download a lot of stuff
  * `packages:  m5_tough_package:` not work - copies content in own `yaml`
* ✅ create working `.yaml` for some of my home assistant lights
  * can compile and upload my `M5DialFirst.yaml` but it shows **DISCONNECTED** after **Offline**- no wifi, bit display is working
  * wifi credentials in clear not via secrets!
  * example scans wifi sucessfully!
  * wifi connects but HA API does not
  * After Update esphome it works nearly
    1. UI is usable and log is informative
    1. HA lights are not responding - sending wrong commands?
       1. ✅ No, first integrate the new esphome device into HA. Second config to [allow service calls](https://esphome.io/components/api.html#:~:text=Before%20a%20newly%20added%20ESPHome,to%20make%20service%20calls%E2%80%9D%20option.)
* ✅ Change paths to my fork in github online
* Change paths to local files off my fork 


## first goals

* add temperature control for lights
* 🚧 dive deeper into custom code components
  

## understanding the code

* uses the M5Dial lib from ... in `C:\Users\Wolfgang\.esphome\build\m5dial\.piolibdeps\m5dial\M5Dial\src`
  * there are `M5Dial.h` and `M5Dial.cpp` with build class `M5Dial` with all technical components as public members
* looks like the code really sends commands to HA, but without serial connection it _doesn't connect to HA_
* should Doxygen the code in `C:\Users\Wolfgang\.esphome\build\m5dial\` 

## emojis

[getemoji](https://getemoji.com/) 💖📈🚀🚧🌧🌞🌩️✅✔️🆗❌💾📤📥<br>📓📒📦✍️📧📌⚙️☠️🔥💥💣⭐📁💶