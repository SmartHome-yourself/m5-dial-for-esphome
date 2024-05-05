# My example config with menu


Should eliminate unecessary parameters in menu modes.

``` yaml

shys_m5_dial:

  ...

  devices:
    menus:
      - entity: fan.my_menu
        name: MenuTest
        modes:
          speed_mode:
            changeable_direction: true
            rotary_step_width: 10          

    lights:
      - entity: light.hue_lightstrip_schreibtisch
        name: strip
        modes:
          dimm_mode:
            enable: true
            rotary_step_width: 4        
          white_mode: 
            enable: true
            rotary_step_width: 100
            min_kelvin: 2000
            max_kelvin: 6000        
          rgb_mode: 
            enable: true
            rotary_step_width: 15
      - entity: light.signe_gradient_table_1
        name: Signe
        modes:
          dimm_mode:
            enable: true
            rotary_step_width: 4        
          white_mode: 
            enable: true
            rotary_step_width: 100
            min_kelvin: 2000
            max_kelvin: 6000        
          rgb_mode: 
            enable: true
            rotary_step_width: 15
      - entity: light.hue_ambiance_panel_1
        name: Panel
        modes:
          dimm_mode:
            enable: true
            rotary_step_width: 4        
          white_mode: 
            enable: true
            rotary_step_width: 100
            min_kelvin: 2000
            max_kelvin: 7000        
```