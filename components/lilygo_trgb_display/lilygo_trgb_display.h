#pragma once
#include "esphome/core/component.h"
#include "esphome/components/display/display_buffer.h"

namespace esphome {
namespace lilygo_trgb_display {

class LilyGoTRGBDisplay : public display::DisplayBuffer {
 public:
  void setup() override;
  void update() override;
  void draw_absolute_pixel_internal(int x, int y, Color color) override;
  void flush_();

  display::DisplayType get_display_type() override {
    return display::DisplayType::DISPLAY_TYPE_COLOR;
  }

 protected:
  int get_width_internal() override { return 480; }
  int get_height_internal() override { return 480; }

 private:
  uint16_t *framebuffer_ = nullptr;
};

}  // namespace lilygo_trgb_display
}  // namespace esphome
