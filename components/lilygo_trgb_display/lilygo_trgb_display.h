#pragma once
#include "esphome/core/component.h"
#include "esphome/components/display/display_buffer.h"

namespace esphome {
namespace lilygo_trgb_display {

class LilyGoTRGBDisplay : public display::DisplayBuffer, public Component {
 public:
  void setup() override;
  void update() override;
  void draw_absolute_pixel_internal(int x, int y, Color color) override;
  void flush_() override;
  display::DisplayType get_display_type() override { return display::DisplayType::DISPLAY_TYPE_RGB; }
};

}  // namespace lilygo_trgb_display
}  // namespace esphome
