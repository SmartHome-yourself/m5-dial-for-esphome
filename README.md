# My Fork of M5 Stack Dial Custom Component für ESPHome

Trying to improve and add functions.

## [Original Repository](https://github.com/SmartHome-yourself/m5-dial-for-esphome) on github from [SmartHome yourself](https://github.com/SmartHome-yourself) 

![image](https://github.com/SmartHome-yourself/m5-dial-for-esphome/assets/705724/6d268fe4-ef71-40bb-b70c-797453b1d06b)
Mehr Infos zum [M5 Stack Dial](https://shop.m5stack.com/products/m5stack-dial-esp32-s3-smart-rotary-knob-w-1-28-round-touch-screen)
  
Mit dieser Komponente wird der M5 Stack Dial zu einer universal-Fernbedienung für Home Assistant.  
Aktuell werden nur Light-Entitäten unterstützt, in Zukunft sollen aber auch weitere Gerätearten darüber steuerbar sein.  

  
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
  devices:
    - entity: light.my_light1
      name: Light 1
      rgb_enabled: true
      dimm_enabled: true
    - entity: light.my_light2
      name: Light 2
    - entity: light.my_light3
      name: Light 3
    - entity: light.my_light4
      name: Light 4
```
  
&nbsp;  
  
# Konfiguration
In den Substitutions muss auf jeden Fall der `devicename` angegeben werden.  
Die Funknetzwerk-Verbindung muss entweder über den normalen `wifi:` Abschnitt vollständig konfiguriert werden, oder man kann alternativ auch einfach die WLAN-SSID und das WLAN-Passwort in den Substitutions angeben. 
  
## Alle zur Verfügung stehenden Parameter (Substitutions):

```
substitutions:
  name: "m5-dial"
  wifi_ssid: !secret wifi_ssid
  wifi_password: !secret wifi_password
```
  
### name
> Der Hostname des Geräts.  *(default: m5-dial)*  
### wifi_ssid
> Der Hostname des Geräts.  *(default: m5-dial)*  
### wifi_password
> Der Hostname des Geräts.  *(default: m5-dial)*  

  
&nbsp;  
  
## Custom-Component
**Beispiel:**  
```
shys_m5_dial:
  name: Dial
  screenOffTime: 45000
  rotaryStepWidth: 5
  longPressDuration: 1200

  devices:
    - entity: light.my_light1
      name: Light 1
      rgb_enabled: true
      dimm_enabled: true
    - entity: light.my_light2
      name: Light 2
      rgb_enabled: true
    - entity: light.my_light3
      name: Light 3
      rgb_enabled: false        
```
  
### name
> Legt den Namen der Komponente fest.
### screenOffTime (optional)
> Gibt an, nach wie viel Millisekunden das Display sich automatisch abschaltet *(Default: 30000)*  
### rotaryStepWidth (optional)
> Gibt die Schrittweite an, um die der Wert bei Verwendung des Drehreglers pro Schritt verändert wird. *(Default: 10)*  
### longPressDuration (optional)
> Gibt die Dauer an, ab wie viel Millisekunden der Tastendruck als Long-Press gelten soll. *(Default: 1200)*  
  
### devices
> Unter Devices werden die Entitäten in einer Liste angegeben, die mit dem M5 Dial gesteuert werden sollen. Dabei sind die Angabe der Entity-ID und eines Namen pflicht. Alle weiteren Attribute sind optional.  
  
#### - entity
  > Angabe der Entity-ID aus Home Assistant, die gesteuert werden soll. *(Aktuell funktionieren nur light-Entitäten)*  
#### - name
  > Der auf dem Display angezeigte Name der Entität.  
#### - rgb_enabled (optional)
  > Regelt, ob die Farbauswahl für die Entität aktiviert sein soll. *(Nur für Light-Entitäten / Default: false)*  
#### - dimm_enabled (optional)
  > Regelt, ob die Helligkeitsregelung für die Entität aktiviert sein soll. *(Nur für Light-Entitäten / Default: false)*
  
  
# Advanced
> Die folgenden Attribute sind zwar vorhanden, sollten aber eigentlich nicht geändert werden müssen.  
  
### sendValueDelay
> Gibt die Verzögerung in Millisekunden an, die bei Wertänderung gewartet wird, bevor die Änderung an Home Assistant übertragen wird.  
> Das ist gerade bei Verwendung des Rotary-Encoders wichtig um nicht unnötig viele API-Aufrufe zu erzeugen. *(Default: 1200)*  
### receiveValueDelay
> Gibt an, wie lange nach einem API-Aufruf gewartet werden soll, bevor neue Werte vom Zielgerät als gültig angesehen werden. *(Default: 3000)*  

&nbsp;



