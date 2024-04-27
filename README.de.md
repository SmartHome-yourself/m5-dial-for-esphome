#### Sprachauswahl:
[![en](https://img.shields.io/badge/lang-en-red.svg)](README.md) 
[![de](https://img.shields.io/badge/lang-de-blue.svg)](README.de.md)
  
# M5 Stack Dial Custom Component für ESPHome
![image](https://github.com/SmartHome-yourself/.github/blob/main/profile/assets/m5-dial-screens.png)  
Mehr Infos zum [M5 Stack Dial](https://shop.m5stack.com/products/m5stack-dial-esp32-s3-smart-rotary-knob-w-1-28-round-touch-screen?ref=smarthomeyourself)
  
Mit dieser Komponente wird der M5 Stack Dial zu einer universal-Fernbedienung für Home Assistant.  
  
Aktuell werden folgende Entitäten unterstützt:  
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
[![M5 Stack Dial](http://img.youtube.com/vi/4dE7YONEYVk/0.jpg)](https://www.youtube.com/watch?v=4dE7YONEYVk "M5 Dial als Home Assistant Fernbedienung")
  
&nbsp;  
  
## Discord
Für einen schnellen Austausch, Anregungen so wie Info's zum aktuellen Entwicklungsstand steht auch unser Discord zur Verfügung.  
https://discord.com/channels/293705077812625408/1217503791541387294  
  
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
  
&nbsp;  
  
# Konfiguration
In den Substitutions muss auf jeden Fall der `devicename` angegeben werden.  
Die Funknetzwerk-Verbindung muss entweder über den normalen `wifi:` Abschnitt vollständig konfiguriert werden, oder man kann alternativ auch einfach die WLAN-SSID und das WLAN-Passwort in den Substitutions angeben. 
  
## Substitutions
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
  
-----
## Custom-Component
-----
## Allgemeine Attribute
Allgemeine Attribute sind alle Parameter, die direkt unter der Custom-Component "shys_m5_dial" zur Verfügung stehen.  
  
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
Legt den Namen der Komponente fest.  
  
**time_component**  
Hier muss die ID einer Time Komponente angegeben werden, worüber der Dial das Datum und die aktuelle Uhrzeit ermitteln kann.  
Ich würde hier empfehlen, die Home Assistant Uhrzeit zu verwenden, da diese Komponente auch für die Timer-Steuerung benötigt wird.  
  
**screenOffTime (optional)** *(Default: 30000)*  
Gibt an, nach wie viel Millisekunden das Display sich automatisch abschaltet  
  
**screensaver (optional)** *(Default: clock)*  
Gibt an, ob ein Bildschirmschoner verwendet werden soll, oder nicht.  
*Gültige Werte: clock, off*
  
**rotaryStepWidth (optional)** *(Default: 10)*  
Gibt die allgemeine Schrittweite an, um die der Wert bei Verwendung des Drehreglers pro Schritt verändert wird.  
Der hier eingestellte Wert gilt für alle Modi, bei denen keine abweichende Schrittweite angegeben wird.  
  
**longPressDuration (optional)** *(Default: 1200)*  
Gibt die Dauer an, ab wie viel Millisekunden ein Tastendruck als Long-Press gelten soll.  
  
**font (optional)** *(Default: default)*  
Gibt die zu verwendende Schriftart an.  
Beim Standard "default" handelt es sich um eine von mir generierte Schriftart, die auch Umlaute und verschiedene Sonderzeichen enthält.  
Alle alternativen zur Verfügung stehenden Schriften wie z.B. FreeSans12pt7b sind in einer Map in [globals.h](components/shys_m5_dial/globals.h) definiert.  
Die alternativen Schriftarten unterstützen allerdings keine Umlaute und Sonderzeichen!  
  
**font_factor (optional)** *(Default: 1)*  
Gibt den Faktor an, der auf die Schriftgröße angewendet werden soll.  
*Gültige Werte: 0.1 - 10.0*
  
&nbsp;  
  
------

## devices
  
Unter Devices werden alle Entitäten angegeben, die mit dem M5 Dial gesteuert werden sollen.  
Die Angabe der Entitäten erfolgt in einzelnen Listen je Geräteart (Domain) wie z.B. Lichter, Schalter, Jalousien usw.  
  
**Code:**
```
shys_m5_dial:
  ...
  devices:
```
  
&nbsp;  
  
------
  
## **LIGHTS**
Unter "devices - lights" werden alle Light-Entitäten angegeben.  
Ohne weitere Angaben lässt sich die Lampe über den Dial nur an-/aus-schalten.  
Um Farbe, Helligkeit oder den Weißwert einstellen zu können muss der jeweilige Mode aktiviert werden.  
  
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
Angabe der Light-Entity-ID aus Home Assistant, die gesteuert werden soll.  
  
**name**  
Der auf dem Display angezeigte Name der Entität.  
  
## modes (optional)
#### dimm_mode
Mit Hilfe des Dimm-Mode lässt sich die Helligkeit der Light-Entität regeln.  
Ist der Dimm-Mode aktiv, ersetzt dieser den Standard-On/Off-Mode.  
Die Helligkeit lässt sich sowohl über den Drehregler als auch per Touch steuern.  
Ein Druck auf den Button schaltet das Licht an/aus.  
  
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
Durch setzen auf true wird der Modus für die Entität aktiviert.  
  
**min_brightness (optional)** *(Default: 0)*  
Legt den maximal möglichen Helligkeitswert fest auf den herunter gedimmt werden kann.  
Ggf. nützlich, wenn Leuchtmittel erst ab 10% Helligkeit leuchten und darunter evtl. flackern.  
*Gültige Werte 0 - 100*  
  
**max_brightness (optional)** *(Default: 100)*  
Legt den maximal möglichen Helligkeitswert fest auf den über den Dial gedimmt werden kann.
*Gültige Werte 0 - 100*  
  
**rotary_step_width (optional)**  
Gibt die allgemeine Schrittweite an, um die der Wert bei Verwendung des Drehreglers pro Schritt verändert wird.  
Der hier eingestellte Wert überschreibt den allgemein eingestellten Wert und gilt nur für den Helligkeits-Modus dieser Light Entität.  
*Gültige Werte 1 - 100*  
  
&nbsp;
  
#### white_mode
Mit Hilfe des White-Mode lässt sich der Weißton für die Light-Entität in Kelvin regeln.  
Der Weiß-Ton lässt sich sowohl über den Drehregler als auch per Touch steuern.  
Ein Druck auf den Button schaltet das Licht an/aus.  
  
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
Durch setzen auf true wird der Modus für die Entität aktiviert.  
  
**rotary_step_width (optional)**  
Gibt die allgemeine Schrittweite an, um die der Wert bei Verwendung des Drehreglers pro Schritt verändert wird.  
Der hier eingestellte Wert überschreibt den allgemein eingestellten Wert und gilt nur für den Weiß-Ton Modus dieser Light Entität.  
*Gültige Werte 1 - 500*  
  
**min_kelvin (optional)** *(Default: 2000)*  
Gibt den Minimalwert in Kelvin für die Weiß-Ton Steuerung an. 
*Gültige Werte 1000 - 10000*  
  
**max_kelvin (optional)** *(Default: 6500)*  
Gibt den Maximalwert in Kelvin für die Weiß-Ton Steuerung an. 
*Gültige Werte 1000 - 10000*  
  
&nbsp;
  
#### rgb_mode
Mit Hilfe des RGB-Mode (Farbwahl) lässt sich die Farbe für die Light-Entität auswählen.  
Die Farbe lässt sich sowohl über den Drehregler als auch per Touch steuern.  
Ein Druck auf den Button schaltet das Licht an/aus.  
Mit einem Touch auf die Farbfläche in der Mitte wechselt die Lampe wieder zurück in den Weiß-Modus.  
  
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
Durch setzen auf true wird der Modus für die Entität aktiviert.
  
**rotary_step_width (optional)**  
Gibt die allgemeine Schrittweite an, um die der Wert bei Verwendung des Drehreglers pro Schritt verändert wird.  
Der hier eingestellte Wert überschreibt den allgemein eingestellten Wert und gilt nur für den Farbwahl-Modus dieser Light Entität.  
*Gültige Werte 1 - 100*  
  
&nbsp;  
  
------
  
## **CLIMATES**
Unter "devices - climates" werden alle Climate-Entitäten angegeben.  
  
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
Angabe der Light-Entity-ID aus Home Assistant, die gesteuert werden soll.  
  
**name**  
Der auf dem Display angezeigte Name der Entität.  
  
## modes (optional)
#### temp_mode
Mit Hilfe des Temp-Mode lässt sich die Temperatur der Climate-Entität regeln.  
Die Temperatur lässt sich sowohl über den Drehregler als auch per Touch steuern.  
Ein Druck auf den Button schalten die Heizung an/aus.  
  
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
Gibt die allgemeine Schrittweite an, um die der Wert bei Verwendung des Drehreglers pro Schritt verändert wird.  
Bei Climates gilt die 1 als Default, egal was als allgemeiner Standard in der Component eingestellt wurde.  
*Gültige Werte 1 - 100*  
  
**min_temperature (optional)** *Default: 4*  
Legt die minimale Temperatur fest, die am Dial für diese Climate-Entität eingestellt werden kann.  
  
**max_temperature (optional)** *Default: 30*  
Legt die maximale Temperatur fest, die am Dial für diese Climate-Entität eingestellt werden kann.
  
&nbsp;  
  
------
  
## **COVER**
Unter "devices - cover" werden alle Cover-Entitäten angegeben.  
  
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
Angabe der Light-Entity-ID aus Home Assistant, die gesteuert werden soll.  
  
**name**  
Der auf dem Display angezeigte Name der Entität.  
  
## modes (optional)
#### position_mode
Mit Hilfe des Position-Mode lässt sich die Position der Cover-Entität steuern.  
Die Position lässt sich sowohl über den Drehregler als auch per Touch steuern.  

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
Gibt die allgemeine Schrittweite an, um die der Wert bei Verwendung des Drehreglers pro Schritt verändert wird.  
Der hier eingestellte Wert überschreibt den allgemein eingestellten Wert und gilt nur für den Position-Modus dieser Cover-Entität.  
*Gültige Werte 1 - 100*  
  
&nbsp;  
  
------
  
## **FANS**
Unter "devices - fans" werden alle Fan-Entitäten angegeben.  
  
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
Angabe der Fan-Entity-ID aus Home Assistant, die gesteuert werden soll.  
  
**name**  
Der auf dem Display angezeigte Name des Ventilators.  
  
## modes (optional)
#### speed_mode
Mit Hilfe des Speed-Mode lässt sich die Geschwindigkeit der Fan-Entität steuern.  
Die Geschwindigkeit lässt sich sowohl über den Drehregler als auch per Touch steuern.  
Ein Druck auf den Button schalten den Ventilator an/aus.
  
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
Gibt an, ob der Ventilator zwei Laufrichtungen hat und somit zwischen vorwärts und rückwärts umgestellt werden kann.  
False = Nur eine Laufrichtung. Drehen nach rechts erhöht die Geschwindigkeit, Drehen nach links reduziert die Geschwindigkeit.
True = Zwei Laufrichtungen. Drehen nach rechts erhöht für Vorwärts die Geschwindigkeit. Drehen nach links reduziert die Geschwindigkeit für Vorwärts.  
Man kann allerdings jetzt über 0 hinweg drehen und somit die Drehrichtung zu ändern.  
Läuft der Lüfter rückwärts, erhöht links drehen die Geschwindigkeit und rechts drehen reduziert diese
*Gültige Werte true / false*  
  
**rotary_step_width (optional)**  
Gibt die allgemeine Schrittweite an, um die der Wert bei Verwendung des Drehreglers pro Schritt verändert wird.  
Der hier eingestellte Wert überschreibt den allgemein eingestellten Wert und gilt nur für den Speed-Modus dieser Ventilator-Entität.  
*Gültige Werte 1 - 100*  
  
&nbsp;  
  
------
  
## **MEDIA PLAYER**
Unter "devices - media_player" werden alle MediaPlayer-Entitäten angegeben.  
  
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
Angabe der MediaPlayer-Entity-ID aus Home Assistant, die gesteuert werden soll.  
  
**name**  
Der auf dem Display angezeigte Name des MediaPlayers.  
  
## modes (optional)
#### play_mode
Mit Hilfe des Play-Mode lässt sich über den Drehregler die Lautstärke regeln, so wie per Touch die Wiedergabe starten/pausieren oder zum nächsten bzw. vorherigen Lied wechseln.  
Ein Druck auf den Button wechselt zwischen Play und Pause.
  
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
Gibt die allgemeine Schrittweite an, um die der Wert bei Verwendung des Drehreglers pro Schritt verändert wird.  
Der hier eingestellte Wert überschreibt den allgemein eingestellten Wert und gilt nur für den Play-Modus dieser MediaPlayer-Entität.  
*Gültige Werte 1 - 100*  
  
  
&nbsp;  
  
#### source_mode
Mit Hilfe des Source-Mode lässt sich über den Drehregler eine Quelle aus den hinterlegten "Sources" auswählen, die abgespielt werden soll.  
Ein Druck auf den Button startet die Wiedergabe der ausgewählten Quelle.  
  
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
Gibt die allgemeine Schrittweite an, um die der Wert bei Verwendung des Drehreglers pro Schritt verändert wird.  
Bei source_mode von media_player gilt die 1 als Default, egal was als allgemeiner Standard in der Component eingestellt wurde.  
*Gültige Werte 1 - 100*  
  
**sources (optional)**
Eine Liste von Audio-Quellen, die zum Abspielen zur Auswahl stehen sollen.  
Die Einträge müssen immer die Attribute "name", "content_id" und "content_type" enthalten!  

Beispiele für Amazon Echo (Alexa):  
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
  
Beispiel für DLNA Player:  
```
              - name: 1Live
                content_id: "https://wdr-1live-live.icecastssl.wdr.de/wdr/1live/live/mp3/128/stream.mp3"
                content_type: custom
```
  
&nbsp;  
  
------
  
## **LOCKS**
Unter "devices - lock" werden alle Fan-Entitäten angegeben.  
  
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
Angabe der Lock-Entity-ID aus Home Assistant, die gesteuert werden soll.  
  
**name**  
Der auf dem Display angezeigte Name des Schlosses.  
  
## modes (optional)
#### lock_mode
Mit Hilfe des Lock-Mode lässt sich der Zustand der Lock-Entität steuern.  
Das Schloss lässt sich über den Drehregler aufschließen, abschließen und öffnen.
Ein Druck auf den Button schließt normalerweise das Schloss je nach aktuellem Zustand auf oder ab.
Ist *open_on_button* allerdings auf *true* gesetzt, wird das Schloss wenn es abgeschlossen ist bei Druck auf den Button nicht aufgeschlossen, sondern geöffnet.  
  
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
Durch setzen auf true, öffnet ein Druck auf den Button das Schloss direkt, anstatt nur das Schloss aufzuschließen.  
Steht der Parameter auf false (oder wird nicht angegeben), wird das Schloss nur aufgeschlossen, aber nicht ganz geöffnet.
*Gültige Werte true / false*  
  
**rotary_step_width (optional)**  
Gibt die allgemeine Schrittweite an, um die der Wert bei Verwendung des Drehreglers pro Schritt verändert wird.  
Bei lock_mode von lock gilt die 1 als Default, egal was als allgemeiner Standard in der Component eingestellt wurde.  
Wird hier eine Schrittweite von 2 angegeben, lässt sich durch drehen das Schloss nicht mehr nur aufschließen, sondern wird automatisch direkt geöffnet.  
*Gültige Werte 1 - 2*  
  
&nbsp;  
  
------
  
## **Number / Input_Number**
Unter "devices - number" werden alle Number- und Input-Number Entitäten angegeben.  
  
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
Angabe der Entity-ID der Nummer aus Home Assistant, die gesteuert werden soll.  
Hier werden sowohl Entitäten der Domain Number als auch von Input-Number unterstützt.
  
**name**  
Der auf dem Display angezeigte Name der Entität.  
  
## modes (optional)
#### value_mode
Mit Hilfe des Value-Mode lässt sich der Wert der Number-Entität steuern.  
Der Wert lässt sich sowohl über den Drehregler als auch per Touch festlegen.  

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
Gibt die Einheit an, die hinter dem Wert der Nummer angezeigt werden soll.  
  
**rotary_step_width (optional)**  
Gibt die allgemeine Schrittweite an, um die der Wert bei Verwendung des Drehreglers pro Schritt verändert wird.  
Der hier eingestellte Wert überschreibt den allgemein eingestellten Wert und gilt nur für den Value-Modus dieser Number-Entität.  
*Gültige Werte 1 - 500*  
  
&nbsp;  
  
------
  
## **Timer**
Unter "devices - timer" werden alle Timer Entitäten angegeben die über den Dial gesteuert werden sollen.  
  
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
Angabe der Entity-ID des Timers aus Home Assistant, der gesteuert werden soll.  

**name**  
Der auf dem Display angezeigte Name der Entität.  
  
## modes (optional)
#### timer_mode
Mit Hilfe des Position-Mode lässt sich die Position der Cover-Entität steuern.  
Die Position lässt sich sowohl über den Drehregler als auch per Touch steuern.  

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
Gibt die allgemeine Schrittweite an, um die der Wert bei Verwendung des Drehreglers pro Schritt verändert wird.  
Der hier eingestellte Wert überschreibt den allgemein eingestellten Wert und gilt nur für den Timer-Modus dieser Timer-Entität.  
*Gültige Werte 1 - 500*  
  
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
  
&nbsp;  
  
  
# Dienste in Home Assistant
Diese Integration stellt verschiedene Dienste in Home Assistant zur Verfügung, um zum Beispiel über Automatisierungen den Dial zu steuern.  
Die Dienste stehen unmittelbar nach dem Einbinden des Dial in Home Assistant automatisch zur Verfügung.  
  
Alle Dienste beginnen mit: **esphome.DEIN_DIAL_NAME_**
  
### select_device
Mit diesem Dienst kann das aktuell aktive Gerät auf dem Dial gewechselt werden.  
```
service: esphome.m5_dial_select_device
data:
  entity_id: light.my_light
```
##### Parameter
**entity_id**
Angabe der Entity-ID, die auf dem Dial als aktive Entität ausgewählt werden soll.
*(Die Entität muss über die Konfiguration in der Komponente hinterlegt sein!)*
  
### lock_device
Über diesen Dienst kann man die Bedienung einer einzelnen Entität für den Dial Sperren.  
Als Parameter erwartet der Dienst die Angabe der Entity-ID. (Die Entität muss über die Konfiguration in der Komponente hinterlegt sein!)
```
service: esphome.m5_dial_lock_device
data:
  entity_id: light.my_light
```
##### Parameter
**entity_id**
Angabe der Entity-ID, für die die Bedienung über den Dial gesperrt werden soll.
*(Die Entität muss über die Konfiguration in der Komponente hinterlegt sein!)*
  
### unlock_device
Über diesen Dienst kann man eine Sperre der Bedienung über den Dial für eine Entität wieder aufheben.  
Als Parameter erwartet der Dienst die Angabe der Entity-ID.  (Die Entität muss über die Konfiguration in der Komponente hinterlegt sein!)
```
service: esphome.m5_dial_unlock_device
data:
  entity_id: light.my_light
```
##### Parameter
**entity_id**
Angabe der Entity-ID, für die die Bedienung über den Dial gesperrt werden soll.
*(Die Entität muss über die Konfiguration in der Komponente hinterlegt sein!)*
  
&nbsp;
  
------  
------  
    
&nbsp;
  
  