#pragma once
#include "esphome/core/component.h"
#include "esphome/components/display/display_buffer.h"

namespace esphome {
namespace lilygo_trgb_display {

class LilyGoTRGBDisplay : public PollingComponent, public display::DisplayBuffer {
 public:
  void setup() override;
  void update() override;
  display::DisplayType get_display_type() override { return display::DisplayType::DISPLAY_TYPE_COLOR; }
  int get_width() override { return 480; }
  int get_height() override { return 480; }

 protected:
  void draw_absolute_pixel_internal(int x, int y, Color color) override;
  void flush_();  // Push buffer to display
};

}  // namespace lilygo_trgb_display
}  // namespace esphome