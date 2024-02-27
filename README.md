# M5 Stack Dial Custom Component für ESPHome
![image](https://github.com/SmartHome-yourself/m5-dial-for-esphome/assets/705724/6d268fe4-ef71-40bb-b70c-797453b1d06b)
Mehr Infos zum [M5 Stack Dial](https://shop.m5stack.com/products/m5stack-dial-esp32-s3-smart-rotary-knob-w-1-28-round-touch-screen?ref=smarthomeyourself)
  
Mit dieser Komponente wird der M5 Stack Dial zu einer universal-Fernbedienung für Home Assistant.  
Aktuell werden nur Light-Entitäten unterstützt, in Zukunft sollen aber auch weitere Gerätearten darüber steuerbar sein.  
  
## Video
[![M5 Stack Dial](http://img.youtube.com/vi/4dE7YONEYVk/0.jpg)](https://www.youtube.com/watch?v=4dE7YONEYVk "M5 Dial als Home Assistant Fernbedienung")

&nbsp;
  
# Beispiel Konfiguration:
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
  name: dial
  rotary_step_width: 10

  devices:
    lights:
      - entity: light.my_light1
        name: Light 1
        modes:
          rgb_mode:
            enable: true
            rotary_step_width: 30
          dimm_mode:
            enable: true
            rotary_step_width: 5

      - entity: light.my_light2
        name: Light 2
        modes:
          dimm_mode:
            enable: true
          white_mode: 
            enable: true

      - entity: light.my_light3
        name: Light 3

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
  
# Konfiguration
In den Substitutions muss auf jeden Fall der `devicename` angegeben werden.  
Die Funknetzwerk-Verbindung muss entweder über den normalen `wifi:` Abschnitt vollständig konfiguriert werden, oder man kann alternativ auch einfach die WLAN-SSID und das WLAN-Passwort in den Substitutions angeben. 
  
## Substitutions
**Alle zur Verfügung stehenden Parameter:**  
```
substitutions:
  name: "m5-dial"
  wifi_ssid: !secret wifi_ssid
  wifi_password: !secret wifi_password
```
  
**name**  
Der Hostname des Geräts.  *(default: m5-dial)*  

**wifi_ssid**  
Der Hostname des Geräts.  *(default: !secret wifi_ssid)*  

**wifi_password**  
Der Hostname des Geräts.  *(default: !secret wifi_password)*  

  
&nbsp;  
  
## Custom-Component
**Beispiel mit allen möglichen Parametern:**  
```
shys_m5_dial:
  name: Dial
  screenOffTime: 45000
  rotaryStepWidth: 5
  longPressDuration: 1200

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
  
## Allgemeine Attribute
**name**  
Legt den Namen der Komponente fest.  
  
**screenOffTime (optional)** *(Default: 30000)*  
Gibt an, nach wie viel Millisekunden das Display sich automatisch abschaltet  
  
**rotaryStepWidth (optional)** *(Default: 10)*  
Gibt die allgemeine Schrittweite an, um die der Wert bei Verwendung des Drehreglers pro Schritt verändert wird.  
Der hier eingestellte Wert gilt für alle Modi, bei denen keine abweichende Schrittweite angegeben wird.  
  
**longPressDuration (optional)** *(Default: 1200)*  
Gibt die Dauer an, ab wie viel Millisekunden ein Tastendruck als Long-Press gelten soll.  
  
------

## devices
Unter Devices werden alle Entitäten angegeben, die mit dem M5 Dial gesteuert werden sollen.  
Die Angabe der Entitäten erfolgt in einzelnen Listen je Geräteart (Domain) wie z.B. Lichter, Schalter, Jalousien usw.  
  
------
  
## **LIGHTS**

**entity**  
Angabe der Light-Entity-ID aus Home Assistant, die gesteuert werden soll.  
  
**name**  
Der auf dem Display angezeigte Name der Entität.  
  
## modes
#### dimm_mode
Mit Hilfe des Dimm-Mode lässt sich die Helligkeit der Light-Entität regeln.  
  
**enable** *(Default: false)*  
Durch setzen auf true wird der Modus für die Entität aktiviert.  
  
**rotary_step_width (optional)**  
Gibt die allgemeine Schrittweite an, um die der Wert bei Verwendung des Drehreglers pro Schritt verändert wird.  
Der hier eingestellte Wert überschreibt den allgemein eingestellten Wert und gilt nur für den Helligkeits-Modus dieser Light Entität.  
*Gültige Werte 1-100*  
  
&nbsp;
  
#### white_mode
Mit Hilfe des White-Mode lässt sich der Weißton für die Light-Entität in Kelvin regeln.  
  
**enable** *(Default: false)*  
Durch setzen auf true wird der Modus für die Entität aktiviert.  
  
**rotary_step_width (optional)**  
Gibt die allgemeine Schrittweite an, um die der Wert bei Verwendung des Drehreglers pro Schritt verändert wird.  
Der hier eingestellte Wert überschreibt den allgemein eingestellten Wert und gilt nur für den Weiß-Ton Modus dieser Light Entität.  
*Gültige Werte 1-500*  
  
**min_kelvin (optional)** *(Default: 2000)*  
Gibt den Minimalwert in Kelvin für die Weiß-Ton Steuerung an. 
*Gültige Werte 1000-10000*  
  
**max_kelvin (optional)** *(Default: 6500)*  
Gibt den Maximalwert in Kelvin für die Weiß-Ton Steuerung an. 
*Gültige Werte 1000-10000*  
  
&nbsp;
  
#### rgb_mode
Mit Hilfe des RGB-Mode (Farbwahl) lässt sich die Farbe für die Light-Entität auswählen.  
  
**enable** *Default: false*  
Durch setzen auf true wird der Modus für die Entität aktiviert.
  
**rotary_step_width (optional)**  
Gibt die allgemeine Schrittweite an, um die der Wert bei Verwendung des Drehreglers pro Schritt verändert wird.  
Der hier eingestellte Wert überschreibt den allgemein eingestellten Wert und gilt nur für den Farbwahl-Modus dieser Light Entität.  
*Gültige Werte 1-100*  
  
&nbsp;  
  
------
  
## **CLIMATES**

**entity**  
Angabe der Light-Entity-ID aus Home Assistant, die gesteuert werden soll.  
  
**name**  
Der auf dem Display angezeigte Name der Entität.  
  
## modes
#### temp_mode
Mit Hilfe des Temp-Mode lässt sich die Temperatur der Climate-Entität regeln.  
  
**rotary_step_width (optional)**  
Gibt die allgemeine Schrittweite an, um die der Wert bei Verwendung des Drehreglers pro Schritt verändert wird.  
Der hier eingestellte Wert überschreibt den allgemein eingestellten Wert und gilt nur für den Temp-Modus dieser Climate Entität.  
*Gültige Werte 1-100*  
  
&nbsp;  
  
------
  
## **COVER**

**entity**  
Angabe der Light-Entity-ID aus Home Assistant, die gesteuert werden soll.  
  
**name**  
Der auf dem Display angezeigte Name der Entität.  
  
## modes
#### position_mode
Mit Hilfe des Position-Mode lässt sich die Position der Cover-Entität steuern.  
  
**rotary_step_width (optional)**  
Gibt die allgemeine Schrittweite an, um die der Wert bei Verwendung des Drehreglers pro Schritt verändert wird.  
Der hier eingestellte Wert überschreibt den allgemein eingestellten Wert und gilt nur für den Temp-Modus dieser Climate Entität.  
*Gültige Werte 1-100*  
  
&nbsp;  
  
------
  
## **FANS**

**entity**  
Angabe der Fan-Entity-ID aus Home Assistant, die gesteuert werden soll.  
  
**name**  
Der auf dem Display angezeigte Name des Ventilators.  
  
## modes
#### speed_mode
Mit Hilfe des Speed-Mode lässt sich die Geschwindigkeit der Fan-Entität steuern.  
  
**changeable_direction (optional)**  *(Default: false)*  
Gibt an, ob der Ventilator zwei Laufrichtungen hat und somit zwischen vorwärts und rückwärts umgestellt werden kann.  
*Gültige Werte true / false*  
  
**rotary_step_width (optional)**  
Gibt die allgemeine Schrittweite an, um die der Wert bei Verwendung des Drehreglers pro Schritt verändert wird.  
Der hier eingestellte Wert überschreibt den allgemein eingestellten Wert und gilt nur für den Temp-Modus dieser Climate Entität.  
*Gültige Werte 1-100*  
  
  
&nbsp;  
  
&nbsp;  
  
------  
------  
  
## Advanced
Die folgenden Attribute sind zwar vorhanden, sollten aber eigentlich nicht geändert werden müssen.  
  
**send_value_delay**  
Gibt die Verzögerung in Millisekunden an, die bei Wertänderung gewartet wird, bevor die Änderung an Home Assistant übertragen wird.  
Das ist gerade bei Verwendung des Rotary-Encoders wichtig um nicht unnötig viele API-Aufrufe zu erzeugen. *(Default: 1200)*
  
**send_value_lock**  
Gibt an, wie lange nach einem API-Aufruf gewartet werden soll, bevor der nächste API Aufruf erfolgen darf. *(Default: 3000)*  
  
  
------  
------  
    
&nbsp;
  


