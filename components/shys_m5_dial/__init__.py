import json

import esphome.codegen as cg
import esphome.config_validation as cv

from esphome.const import CONF_ID, CONF_NAME
from esphome.components import time


# LIMITS
MAX_DEVICES = 50

# PARAMETER
CONF_DEVICES                          = "devices"
CONF_DEVICE_ENTRY_ID                  = "entity"
CONF_DEVICE_ENTRY_NAME                = "name"
CONF_DEVICE_MODES                     = "modes"

CONF_SCREEN_OFF_TIME                  = "screen_off_time"
CONF_SCREENSAVER                      = "screensaver"
CONF_LONG_PRESS_DURATION              = "long_press_duration"
CONF_SEND_VALUE_DELAY                 = "send_value_delay"
CONF_SEND_VALUE_LOCK                  = "send_value_lock"
CONF_ROTARY_STEP_WIDTH                = "rotary_step_width"
CONF_FONT                             = "font"
CONF_FONT_FACTOR                      = "font_factor"

CONF_TIME_COMPONENT                   = "time_component"


# ALLGEMEINE MODE PARAMETER
CONF_DEVICE_MODE_ENABLE               = "enable"

# LIGHTS
CONF_DEVICE_LIGHTS                    = "lights"
CONF_DEVICE_LIGHT_RGB_MODE            = "rgb_mode"
CONF_DEVICE_LIGHT_DIMM_MODE           = "dimm_mode"
CONF_DEVICE_LIGHT_WHITE_MODE          = "white_mode"

# DIMM-MODE PARAMETER
CONF_DEVICE_MODE_DIMM_MIN_BRIGHTNESS = "min_brightness"
CONF_DEVICE_MODE_DIMM_MAX_BRIGHTNESS = "max_brightness"

# WHITE-MODE PARAMETER
CONF_DEVICE_MODE_WHITE_MIN_KELVIN     = "min_kelvin"
CONF_DEVICE_MODE_WHITE_MAX_KELVIN     = "max_kelvin"


# CLIMATES
CONF_DEVICE_CLIMATES                  = "climates"
CONF_DEVICE_CLIMATE_TEMP_MODE         = "temp_mode"
CONF_DEVICE_CLIMATE_FAN_MODE          = "fan_mode"
CONF_DEVICE_CLIMATE_FAN_MODES         = "modes"

# TEMP-MODE PARAMETER
CONF_DEVICE_MODE_TEMP_MIN_TEMP        = "min_temperature"
CONF_DEVICE_MODE_TEMP_MAX_TEMP        = "max_temperature"


# COVER
CONF_DEVICE_COVER                     = "covers"
CONF_DEVICE_COVER_POSITION_MODE       = "position_mode"


# SWITCH
CONF_DEVICE_SWITCH                    = "switches"


# FAN
CONF_DEVICE_FAN                       = "fans"
CONF_DEVICE_FAN_SPEED_MODE            = "speed_mode"
CONF_CHANGEABLE_DIRECTION             = "changeable_direction"


# MEDIA PLAYER
CONF_DEVICE_MEDIA_PLAYER              = "media_player"
CONF_DEVICE_MEDIA_PLAYER_PLAY_MODE    = "play_mode"
CONF_DEVICE_MEDIA_PLAYER_SOURCE_MODE  = "source_mode"
CONF_MEDIA_PLAYER_SOURCES             = "sources"
CONF_CONTENT_ID                       = "content_id"
CONF_CONTENT_TYPE                     = "content_type"


# LOCK
CONF_DEVICE_LOCK                      = "lock"
CONF_DEVICE_LOCK_MODE                 = "lock_mode"
CONF_DEVICE_LOCK_OPEN_ON_BUTTON       = "open_on_button"


# NUMBER
CONF_DEVICE_NUMBER                    = "number"
CONF_DEVICE_NUMBER_MODE               = "value_mode"
CONF_DEVICE_NUMBER_UNIT               = "unit"


# TIMER
CONF_DEVICE_TIMER                     = "timer"
CONF_DEVICE_TIMER_MODE                = "timer_mode"




# MENU --- NEW
CONF_DEVICE_MENU                    = "menus"
CONF_DEVICE_MENU_SPEED_MODE         = "speed_mode"
CONF_CHANGEABLE_DIRECTION           = "changeable_direction"


# DEFAULTS
DEFAULT_NAME                           = "M5 Stack Dial"
DEFAULT_SCREEN_OFF_TIME                = 30000
DEFAULT_LONG_PRESS_DURATION            = 1200
DEFAULT_SEND_VALUE_DELAY               = 1200
DEFAULT_SEND_VALUE_LOCK                = 3000
DEFAULT_ROTARY_STEP_WIDTH              = 10
DEFAULT_FONT                           = "default"    #"FreeSans12pt7b"
DEFAULT_FONT_FACTOR                    = 1
DEFAULT_WHITE_MIN_KELVIN               = 2000
DEFAULT_WHITE_MAX_KELVIN               = 6500
DEFAULT_WHITE_MIN_TEMP                 = 4
DEFAULT_WHITE_MAX_TEMP                 = 30
DEFAULT_CLIMATE_ROTARY_STEP_WIDTH      = 1
DEFAULT_MEDIA_PLAYER_ROTARY_STEP_WIDTH = 1
DEFAULT_LOCK_ROTARY_STEP_WIDTH         = 1
DEFAULT_SCREENSAVER                    = "clock"



SCREENSAVER = ["off", "clock"]


# ------------------------------------------------------

shys_m5_dial_ns = cg.esphome_ns.namespace('shys_m5_dial')
ShysM5Dial = shys_m5_dial_ns.class_('ShysM5Dial', cg.Component)


CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(ShysM5Dial),
    cv.Optional(CONF_NAME, default=DEFAULT_NAME): cv.string,

    cv.Optional(CONF_SCREEN_OFF_TIME, default=DEFAULT_SCREEN_OFF_TIME): cv.int_range(0, 999999),
    cv.Optional(CONF_SCREENSAVER, default=DEFAULT_SCREENSAVER): cv.one_of(*SCREENSAVER, lower=True),
    cv.Optional(CONF_LONG_PRESS_DURATION, default=DEFAULT_LONG_PRESS_DURATION): cv.int_range(0, 5000),
    cv.Optional(CONF_SEND_VALUE_DELAY, default=DEFAULT_SEND_VALUE_DELAY): cv.int_range(0, 999999),
    cv.Optional(CONF_SEND_VALUE_LOCK, default=DEFAULT_SEND_VALUE_LOCK): cv.int_range(0, 999999),
    cv.Optional(CONF_ROTARY_STEP_WIDTH, default=DEFAULT_ROTARY_STEP_WIDTH): cv.int_range(0, 100),
    cv.Optional(CONF_FONT, default=DEFAULT_FONT): cv.string,
    cv.Optional(CONF_FONT_FACTOR, default=DEFAULT_FONT_FACTOR): cv.float_range(0.1, 10.0),
    
    cv.Required(CONF_TIME_COMPONENT): cv.use_id(time),

    cv.Optional(CONF_DEVICES, default=dict()): cv.All(dict({
    
        cv.Optional(CONF_DEVICE_LIGHTS, default=[]): cv.ensure_list(dict({
            cv.Required(CONF_DEVICE_ENTRY_ID): cv.string,
            cv.Required(CONF_DEVICE_ENTRY_NAME): cv.string,

            cv.Optional(CONF_DEVICE_MODES, default=dict()): cv.All(dict({
                cv.Optional(CONF_DEVICE_LIGHT_RGB_MODE, default=dict()): cv.All(dict({
                    cv.Optional(CONF_DEVICE_MODE_ENABLE, default=False): cv.boolean,
                    cv.Optional(CONF_ROTARY_STEP_WIDTH): cv.int_range(1, 100)
                })),

                cv.Optional(CONF_DEVICE_LIGHT_DIMM_MODE, default=dict()): cv.All(dict({
                    cv.Optional(CONF_DEVICE_MODE_ENABLE, default=False): cv.boolean,
                    cv.Optional(CONF_ROTARY_STEP_WIDTH): cv.int_range(1, 100),
                    cv.Optional(CONF_DEVICE_MODE_DIMM_MIN_BRIGHTNESS): cv.int_range(1, 100),
                    cv.Optional(CONF_DEVICE_MODE_DIMM_MAX_BRIGHTNESS): cv.int_range(1, 100)
                })),

                cv.Optional(CONF_DEVICE_LIGHT_WHITE_MODE, default=dict()): cv.All(dict({
                    cv.Optional(CONF_DEVICE_MODE_ENABLE, default=False): cv.boolean,
                    cv.Optional(CONF_ROTARY_STEP_WIDTH): cv.int_range(1, 500),
                    cv.Optional(CONF_DEVICE_MODE_WHITE_MIN_KELVIN, default=DEFAULT_WHITE_MIN_KELVIN): cv.int_range(1000, 10000),
                    cv.Optional(CONF_DEVICE_MODE_WHITE_MAX_KELVIN, default=DEFAULT_WHITE_MAX_KELVIN): cv.int_range(1000, 10000)                    
                }))
               
            }))
        })),


        cv.Optional(CONF_DEVICE_CLIMATES, default=[]): cv.ensure_list(dict({
            cv.Required(CONF_DEVICE_ENTRY_ID): cv.string,
            cv.Required(CONF_DEVICE_ENTRY_NAME): cv.string,

            cv.Optional(CONF_DEVICE_MODES, default=dict()): cv.All(dict({
                cv.Optional(CONF_DEVICE_CLIMATE_TEMP_MODE, default=dict()): cv.All(dict({
                    cv.Optional(CONF_ROTARY_STEP_WIDTH, default=DEFAULT_CLIMATE_ROTARY_STEP_WIDTH): cv.int_range(1, 500),
                    cv.Optional(CONF_DEVICE_MODE_TEMP_MIN_TEMP, default=DEFAULT_WHITE_MIN_TEMP): cv.int_range(0, 500),
                    cv.Optional(CONF_DEVICE_MODE_TEMP_MAX_TEMP, default=DEFAULT_WHITE_MAX_TEMP): cv.int_range(0, 500)
                })),

                cv.Optional(CONF_DEVICE_CLIMATE_FAN_MODE, default=dict()): cv.All(dict({
                    cv.Optional(CONF_DEVICE_CLIMATE_FAN_MODES, default=[]): cv.ensure_list(cv.string),
                    cv.Optional(CONF_ROTARY_STEP_WIDTH, default=1): cv.int_range(1, 5)                  
                }))

            }))
            
        })),


        cv.Optional(CONF_DEVICE_COVER, default=[]): cv.ensure_list(dict({
            cv.Required(CONF_DEVICE_ENTRY_ID): cv.string,
            cv.Required(CONF_DEVICE_ENTRY_NAME): cv.string,

            cv.Optional(CONF_DEVICE_MODES, default=dict()): cv.All(dict({
                cv.Optional(CONF_DEVICE_COVER_POSITION_MODE, default=dict()): cv.All(dict({
                    cv.Optional(CONF_ROTARY_STEP_WIDTH): cv.int_range(1, 500),
                }))
            }))
        })),


        cv.Optional(CONF_DEVICE_SWITCH, default=[]): cv.ensure_list(dict({
            cv.Required(CONF_DEVICE_ENTRY_ID): cv.string,
            cv.Required(CONF_DEVICE_ENTRY_NAME): cv.string
        })),


        cv.Optional(CONF_DEVICE_FAN, default=[]): cv.ensure_list(dict({
            cv.Required(CONF_DEVICE_ENTRY_ID): cv.string,
            cv.Required(CONF_DEVICE_ENTRY_NAME): cv.string,

            cv.Optional(CONF_DEVICE_MODES, default=dict()): cv.All(dict({
                cv.Optional(CONF_DEVICE_FAN_SPEED_MODE, default=dict()): cv.All(dict({
                    cv.Optional(CONF_ROTARY_STEP_WIDTH): cv.int_range(1, 100),
                    cv.Optional(CONF_CHANGEABLE_DIRECTION, default=False): cv.boolean
                }))
            }))
        })),


        cv.Optional(CONF_DEVICE_MEDIA_PLAYER, default=[]): cv.ensure_list(dict({
            cv.Required(CONF_DEVICE_ENTRY_ID): cv.string,
            cv.Required(CONF_DEVICE_ENTRY_NAME): cv.string,

            cv.Optional(CONF_DEVICE_MODES, default=dict()): cv.All(dict({
                cv.Optional(CONF_DEVICE_MEDIA_PLAYER_PLAY_MODE , default=dict()): cv.All(dict({
                    cv.Optional(CONF_ROTARY_STEP_WIDTH): cv.int_range(1, 100)
                })),

                cv.Optional(CONF_DEVICE_MEDIA_PLAYER_SOURCE_MODE , default=dict()): cv.All(dict({
                    cv.Optional(CONF_ROTARY_STEP_WIDTH, default=DEFAULT_MEDIA_PLAYER_ROTARY_STEP_WIDTH): cv.int_range(1, 100),
                    cv.Optional(CONF_MEDIA_PLAYER_SOURCES, default=[]): cv.ensure_list(dict({
                        cv.Required(CONF_NAME): cv.string,
                        cv.Required(CONF_CONTENT_ID): cv.string,
                        cv.Required(CONF_CONTENT_TYPE): cv.string,
                    }))
                })),
            }))
        })),


        cv.Optional(CONF_DEVICE_LOCK, default=[]): cv.ensure_list(dict({
            cv.Required(CONF_DEVICE_ENTRY_ID): cv.string,
            cv.Required(CONF_DEVICE_ENTRY_NAME): cv.string,

            cv.Optional(CONF_DEVICE_MODES, default=dict()): cv.All(dict({
                cv.Optional(CONF_DEVICE_LOCK_MODE, default=dict()): cv.All(dict({
                    cv.Optional(CONF_ROTARY_STEP_WIDTH, default=DEFAULT_LOCK_ROTARY_STEP_WIDTH): cv.int_range(1, 2),
                    cv.Optional(CONF_DEVICE_LOCK_OPEN_ON_BUTTON, default=False): cv.boolean
                }))
            }))
        })),


        cv.Optional(CONF_DEVICE_NUMBER, default=[]): cv.ensure_list(dict({
            cv.Required(CONF_DEVICE_ENTRY_ID): cv.string,
            cv.Required(CONF_DEVICE_ENTRY_NAME): cv.string,

            cv.Optional(CONF_DEVICE_MODES, default=dict()): cv.All(dict({
                cv.Optional(CONF_DEVICE_NUMBER_MODE, default=dict()): cv.All(dict({
                    cv.Optional(CONF_ROTARY_STEP_WIDTH): cv.int_range(1, 500),
                    cv.Optional(CONF_DEVICE_NUMBER_UNIT): cv.string,
                }))
            }))
        })),


        cv.Optional(CONF_DEVICE_TIMER, default=[]): cv.ensure_list(dict({
            cv.Required(CONF_DEVICE_ENTRY_ID): cv.string,
            cv.Required(CONF_DEVICE_ENTRY_NAME): cv.string,

            cv.Optional(CONF_DEVICE_MODES, default=dict()): cv.All(dict({
                cv.Optional(CONF_DEVICE_TIMER_MODE, default=dict()): cv.All(dict({
                    cv.Optional(CONF_ROTARY_STEP_WIDTH): cv.int_range(1, 500),
                }))
            }))
        })),


    }))

}).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    if CONF_SCREEN_OFF_TIME in config:
        screenOffTime = config[CONF_SCREEN_OFF_TIME]
        cg.add(var.setScreenOffTime(screenOffTime))

    if CONF_SCREENSAVER in config:
        screensaver = config[CONF_SCREENSAVER]
        cg.add(var.setScreensaver(screensaver))

    if CONF_LONG_PRESS_DURATION in config:
        longPressDuration = config[CONF_LONG_PRESS_DURATION]
        cg.add(var.setLongPressDuration(longPressDuration))

    if CONF_SEND_VALUE_DELAY in config:
        apiSendDelay = config[CONF_SEND_VALUE_DELAY]
        cg.add(var.setApiSendDelay(apiSendDelay))

    if CONF_SEND_VALUE_LOCK in config:
        apiSendLock = config[CONF_SEND_VALUE_LOCK]
        cg.add(var.setApiSendLock(apiSendLock))

    if CONF_ROTARY_STEP_WIDTH in config:
        rotaryStepWidth = config[CONF_ROTARY_STEP_WIDTH]
        cg.add(var.setRotaryStepWidth(rotaryStepWidth))

    if CONF_FONT in config:
        fontname = config[CONF_FONT]
        cg.add(var.setFontName(fontname))

    if CONF_FONT_FACTOR in config:
        fontfactor = config[CONF_FONT_FACTOR]
        cg.add(var.setFontFactor(fontfactor))


    if CONF_TIME_COMPONENT in config:
        time_component = await cg.get_variable(config[CONF_TIME_COMPONENT])
        cg.add(var.setTimeComponent(time_component))



    if CONF_DEVICES in config:
        confDevices = config[CONF_DEVICES]

        if CONF_DEVICE_LIGHTS in confDevices:
            confLights = confDevices[CONF_DEVICE_LIGHTS]
            for lightEntry in confLights:
                cg.add(var.addLight(lightEntry[CONF_DEVICE_ENTRY_ID], 
                                    lightEntry[CONF_DEVICE_ENTRY_NAME], 
                                    json.dumps(lightEntry[CONF_DEVICE_MODES])
                                   ))

        if CONF_DEVICE_CLIMATES in confDevices:
            confClimates = confDevices[CONF_DEVICE_CLIMATES]
            for climateEntry in confClimates:
                cg.add(var.addClimate(climateEntry[CONF_DEVICE_ENTRY_ID], 
                                      climateEntry[CONF_DEVICE_ENTRY_NAME], 
                                      json.dumps(climateEntry[CONF_DEVICE_MODES])
                                     ))

        if CONF_DEVICE_COVER in confDevices:
            confCover = confDevices[CONF_DEVICE_COVER]
            for coverEntry in confCover:
                cg.add(var.addCover(coverEntry[CONF_DEVICE_ENTRY_ID], 
                                      coverEntry[CONF_DEVICE_ENTRY_NAME], 
                                      json.dumps(coverEntry[CONF_DEVICE_MODES])
                                     ))


        if CONF_DEVICE_SWITCH in confDevices:
            confSwitch = confDevices[CONF_DEVICE_SWITCH]
            for switchEntry in confSwitch:
                cg.add(var.addSwitch(switchEntry[CONF_DEVICE_ENTRY_ID], 
                                     switchEntry[CONF_DEVICE_ENTRY_NAME], 
                                     "{}")
                                    )


        if CONF_DEVICE_FAN in confDevices:
            confFan = confDevices[CONF_DEVICE_FAN]
            for fanEntry in confFan:
                cg.add(var.addFan(fanEntry[CONF_DEVICE_ENTRY_ID], 
                                  fanEntry[CONF_DEVICE_ENTRY_NAME], 
                                  json.dumps(fanEntry[CONF_DEVICE_MODES]))
                                 )


        if CONF_DEVICE_MEDIA_PLAYER in confDevices:
            confMediaPlayer = confDevices[CONF_DEVICE_MEDIA_PLAYER]
            for mediaPlayerEntry in confMediaPlayer:
                cg.add(var.addMediaPlayer(mediaPlayerEntry[CONF_DEVICE_ENTRY_ID], 
                                          mediaPlayerEntry[CONF_DEVICE_ENTRY_NAME], 
                                          json.dumps(mediaPlayerEntry[CONF_DEVICE_MODES]))
                                         )


        if CONF_DEVICE_LOCK in confDevices:
            confLock = confDevices[CONF_DEVICE_LOCK]
            for lockEntry in confLock:
                cg.add(var.addLock(lockEntry[CONF_DEVICE_ENTRY_ID], 
                                   lockEntry[CONF_DEVICE_ENTRY_NAME], 
                                   json.dumps(lockEntry[CONF_DEVICE_MODES]))
                                  )


        if CONF_DEVICE_NUMBER in confDevices:
            confNumber = confDevices[CONF_DEVICE_NUMBER]
            for numberEntry in confNumber:
                cg.add(var.addNumber(numberEntry[CONF_DEVICE_ENTRY_ID], 
                                     numberEntry[CONF_DEVICE_ENTRY_NAME], 
                                     json.dumps(numberEntry[CONF_DEVICE_MODES]))
                                    )


        if CONF_DEVICE_TIMER in confDevices:
            confTimer = confDevices[CONF_DEVICE_TIMER]
            for timerEntry in confTimer:
                cg.add(var.addTimer(timerEntry[CONF_DEVICE_ENTRY_ID], 
                                     timerEntry[CONF_DEVICE_ENTRY_NAME], 
                                     json.dumps(timerEntry[CONF_DEVICE_MODES]))
                                    )
        if CONF_DEVICE_MENU in confDevices:
            confMenu = confDevices[CONF_DEVICE_MENU]
            for fanEntry in confMenu:
                cg.add(var.addMenu(fanEntry[CONF_DEVICE_ENTRY_ID], 
                                  fanEntry[CONF_DEVICE_ENTRY_NAME], 
                                  json.dumps(fanEntry[CONF_DEVICE_MODES]))
                                 )