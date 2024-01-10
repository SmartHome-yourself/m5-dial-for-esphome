import esphome.codegen as cg
import esphome.config_validation as cv
# import voluptuous as vol
from esphome.components import text_sensor
from esphome.const import CONF_ID, CONF_NAME


# PARAMETER
CONF_DEVICES = "devices"
CONF_ENTRY_ID = "entity"
CONF_ENTRY_NAME = "name"


# DEFAULTS
DEFAULT_NAME = "M5 Stack Dial"

shys_m5_dial_ns = cg.esphome_ns.namespace('shys_m5_dial')
ShysM5Dial = shys_m5_dial_ns.class_('ShysM5Dial', text_sensor.TextSensor, cg.Component)

CONFIG_SCHEMA = text_sensor.TEXT_SENSOR_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(ShysM5Dial),
    cv.Optional(CONF_NAME, default=DEFAULT_NAME): cv.string,
    cv.Optional(CONF_DEVICES, default=[]):  cv.All([dict({
        cv.Required(CONF_ENTRY_ID): cv.string,
        cv.Required(CONF_ENTRY_NAME): cv.string
    })])
}).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield text_sensor.register_text_sensor(var, config)
    yield cg.register_component(var, config)

    if CONF_DEVICES in config:
        confDevices = config[CONF_DEVICES]
        for deviceEntry in confDevices:
            cg.add(var.addDevice(deviceEntry[CONF_ENTRY_ID], deviceEntry[CONF_ENTRY_NAME]))

