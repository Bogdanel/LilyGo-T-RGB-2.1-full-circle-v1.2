import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display
from esphome.const import CONF_ID  # 👈 this line was missing
from esphome import pins

CODEOWNERS = ["@Bogdanel"]

lilygo_trgb_display_ns = cg.esphome_ns.namespace("lilygo_trgb_display")
LilyGoTRGBDisplay = lilygo_trgb_display_ns.class_(
    "LilyGoTRGBDisplay", display.DisplayBuffer, cg.Component
)

CONFIG_SCHEMA = display.BASIC_DISPLAY_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(LilyGoTRGBDisplay),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await display.register_display(var, config)
