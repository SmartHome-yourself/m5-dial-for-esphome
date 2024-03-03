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
* [m5dial quick start](https://docs.m5stack.com/en/quick_start/m5dial/arduino) Docs
* on which library is `M5GFX` based? Where is the [API to find](https://docs.m5stack.com/en/api/m5gfx/m5gfx)?   
  * `bool loadFont(const uint8_t *array)` oder alternativ einen FS-Path. Was passiert nach dem Laden, ist das dann der aktuelle Font? Kann ich in YAML Fonts einbinden und dann benutzen?
  * `void setFont(const IFont *font)` Specify the font for Text.
  * `void setBrightness(uint8_t brightness)`
  * `int32_t width()`
  * `void drawString(const char* string, int32_t x, int32_t y, const IFont* font)`
  * [m5gfx_button](https://docs.m5stack.com/en/api/m5gfx/m5gfx_button) This class is designed to be compatible with Adafruit GFX Library .Virtual buttons can be placed on the screen for easy control.
    * `void initButton(M5GFX *gfx, int16_t x, int16_t y, int16_t w, int16_t h, const T& outline, const T& fill, const T& textcolor, const char* label, float textsize_x = 1.0f, float textsize_y = 1.0f)`
  * Die Grafikroutinen, z.B. das Farbrad stehen in `m5dial_display.h`
    * Farbrad ist ineffizient und unschön. Es werden 5 Linien gezeichnet: Von degree-0.25 bis degree+0.25. 
      1. Die degree +/- 0 wird dreimal gezeichnet
      2. eigentlich müsste es reichen nur auf _"einer Seite"_ breiter zu werden und drei Linien zu  zeichnen
         * `for(int i=0;i>-3;i--) { coord c1 = getColorCoord(r1, degree+(i*.25)); coord c2 = getColorCoord(r2, degree+(i*.25)); M5Dial.Display.drawLine(c1.x, c1.y, c2.x, c2.y, color); } ` 
  * ``
  * ``
  * ``


``` CPP
#include <Arduino.h>
#include <M5GFX.h>
M5GFX display;

void setup() {
  display.begin();
  display.setTextDatum(middle_center);
  uint16_t x = display.width() / 2;
  uint16_t y = display.height() / 2;
  display.drawString("Text", x, y);
}

// ----------------------


LGFX_Button button;

void setup(void)
{
  auto cfg = M5.config();
  M5.begin(cfg);
  w = M5.Lcd.width();
  h = M5.Lcd.height();
  button.initButton(&M5.Lcd, w / 2 , h / 2, 100, 50, TFT_RED, TFT_YELLOW, TFT_BLACK, "Btn" );
  button.drawButton();
}

void loop(void)
{

  M5.update();
  if (M5.BtnA.isPressed()) {
    button.press(true);
    button.drawButton(true);
    M5.Lcd.drawString("Button Pressed ", 0, 0, &fonts::lgfxJapanGothic_16);
    Serial.println("Pressed");
  } else {
    button.press(false);
    button.drawButton(false, "Test");
    M5.Lcd.drawString("Button Released", 0, 0, &fonts::lgfxJapanGothic_16);
    Serial.println("Released");
  }
  delay(50);
}

```

### Klassenhierarchie

* esphome::shys_m5_dial::ShysM5Dial	
* esphome::shys_m5_dial::M5DialDisplay::coord	
* esphome::shys_m5_dial::M5DialTouch::Coord	
* esphome::shys_m5_dial::HaApi	
* esphome::shys_m5_dial::HaDevice	
* esphome::shys_m5_dial::M5DialDisplay::hsl	
* esphome::shys_m5_dial::M5DialDisplay	
* esphome::shys_m5_dial::M5DialRfid	
* esphome::shys_m5_dial::M5DialRotary	
* esphome::shys_m5_dial::M5DialTouch	
* esphome::shys_m5_dial::M5DialDisplay::rgb	

## understanding the code

* uses the M5Dial lib from ... in `C:\Users\Wolfgang\.esphome\build\m5dial\.piolibdeps\m5dial\M5Dial\src`
  * there are `M5Dial.h` and `M5Dial.cpp` with build class `M5Dial` with all technical components as public members
* looks like the code really sends commands to HA, but without serial connection it _doesn't connect to HA_
* should Doxygen the code in `C:\Users\Wolfgang\.esphome\build\m5dial\` 

### generated setup()

Finde nicht das letztlich relevante Codestück, das das Display definiert!

#### `C:\Users\Wolfgang\.esphome\build\m5dial\src`

``` CPP
  shys_m5_dial_shysm5dial = new shys_m5_dial::ShysM5Dial();
  shys_m5_dial_shysm5dial->set_component_source("esphome.coroutine");
  App.register_component(shys_m5_dial_shysm5dial);
  shys_m5_dial_shysm5dial->addDevice("light.hue_lightstrip_schreibtisch", "strip", true, true);
  shys_m5_dial_shysm5dial->addDevice("light.signe_gradient_table_1", "Signe", true, true);
  shys_m5_dial_shysm5dial->addDevice("light.hue_lightstrip_plus_1", "StripFlur", true, true);
  shys_m5_dial_shysm5dial->addDevice("light.hue_ambiance_panel_1", "Panel", false, true);
  shys_m5_dial_shysm5dial->setScreenOffTime(30000);
  shys_m5_dial_shysm5dial->setLongPressDuration(1200);
  shys_m5_dial_shysm5dial->setSendValueDelay(600);
  shys_m5_dial_shysm5dial->setReceiveValueDelay(3000);
  shys_m5_dial_shysm5dial->setRotaryStepWidth(4);
```

#### `C:\Users\Wolfgang\.esphome\build\m5dial\src\esphome\components\shys_m5_dial\shys_m5_dial.h` 

``` CPP
      void initDevice(){
        using std::placeholders::_1;
        using std::placeholders::_2;

        ESP_LOGI("DEVICE", "Initialisierung...");

        auto cfg = M5.config();
        M5Dial.begin(cfg, enableEncoder, enableRFID);

        m5DialRotary.on_rotary_left(std::bind(&esphome::shys_m5_dial::ShysM5Dial::turnRotaryLeft, this));
        m5DialRotary.on_rotary_right(std::bind(&esphome::shys_m5_dial::ShysM5Dial::turnRotaryRight, this));
        m5DialRotary.on_short_button_press(std::bind(&esphome::shys_m5_dial::ShysM5Dial::shortButtonPress, this));
        m5DialRotary.on_long_button_press(std::bind(&esphome::shys_m5_dial::ShysM5Dial::longButtonPress, this));

        m5DialTouch.on_touch(std::bind(&esphome::shys_m5_dial::ShysM5Dial::touchInput, this, _1, _2));
        m5DialTouch.on_swipe(std::bind(&esphome::shys_m5_dial::ShysM5Dial::touchSwipe, this, _1));

        m5DialDisplay.on_publish_state(std::bind(&esphome::shys_m5_dial::ShysM5Dial::publish_state, this, _1));
      }

```


#### `C:\Users\Wolfgang\.esphome\build\m5dial\.piolibdeps\m5dial\M5GFX\src\M5ModuleDisplay.h` 

``` CPP
  M5ModuleDisplay( uint16_t logical_width  = M5MODULEDISPLAY_LOGICAL_WIDTH
                 , uint16_t logical_height = M5MODULEDISPLAY_LOGICAL_HEIGHT
                 , float refresh_rate      = M5MODULEDISPLAY_REFRESH_RATE
                 , uint16_t output_width   = M5MODULEDISPLAY_OUTPUT_WIDTH
                 , uint16_t output_height  = M5MODULEDISPLAY_OUTPUT_HEIGHT
                 , uint_fast8_t scale_w    = M5MODULEDISPLAY_SCALE_W
                 , uint_fast8_t scale_h    = M5MODULEDISPLAY_SCALE_H
                 , uint32_t pixel_clock    = M5MODULEDISPLAY_PIXELCLOCK
                 )
  {
    _board = lgfx::board_t::board_M5ModuleDisplay;
    _config.logical_width  = logical_width;
    _config.logical_height = logical_height;
    _config.refresh_rate   = refresh_rate;
    _config.output_width   = output_width;
    _config.output_height  = output_height;
    _config.scale_w        = scale_w;
    _config.scale_h        = scale_h;
    _config.pixel_clock    = pixel_clock;
  }
```

## emojis

[getemoji](https://getemoji.com/) 💖📈🚀🚧🌧🌞🌩️✅✔️🆗❌💾📤📥<br>📓📒📦✍️📧📌⚙️☠️🔥💥💣⭐📁💶