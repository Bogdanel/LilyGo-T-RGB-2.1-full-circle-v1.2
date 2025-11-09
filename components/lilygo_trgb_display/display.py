import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display
from esphome.const import (
    CONF_ID,
    CONF_LAMBDA)
from esphome.final_validate import full_config

DEPENDENCIES = ["display"]
CODEOWNERS = ["@Bogdanel"]

lilygo_trgb_display_ns = cg.esphome_ns.namespace("lilygo_trgb_display")
LilyGoTRGBDisplay = lilygo_trgb_display_ns.class_(
    "LilyGoTRGBDisplay",
    display.DisplayBuffer,
)

CONFIG_SCHEMA = display.BASIC_DISPLAY_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(LilyGoTRGBDisplay),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await display.register_display(var, config)
    if lamb := config.get(CONF_LAMBDA):
        lambda_ = await cg.process_lambda(
            lamb, [(display.DisplayRef, "it")], return_type=cg.void
        )
        cg.add(var.set_writer(lambda_))
