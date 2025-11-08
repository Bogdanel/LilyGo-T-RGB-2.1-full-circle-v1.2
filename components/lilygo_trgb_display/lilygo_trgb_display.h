#pragma once
#include "esphome/core/component.h"
#include "esphome/components/display/display_buffer.h"

namespace esphome {
namespace lilygo_trgb_display {

class LilyGoTRGBDisplay : public display::DisplayBuffer, public Component {
 public:
  void setup() override;
  void loop() override { this->update(); }
  void update() override;
  void draw_absolute_pixel_internal(int x, int y, Color color) override;
  void flush_();

 protected:
  void do_update_() override {}
};

}  // namespace lilygo_trgb_display
}  // namespace esphome
