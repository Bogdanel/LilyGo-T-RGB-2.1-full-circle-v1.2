#pragma once
#include "esphome/core/component.h"
#include "esphome/components/display/display_buffer.h"
#include "LilyGo_RGBPanel.h"

namespace esphome {
namespace lilygo_trgb_display {

class LilyGoTRGBDisplay : public display::DisplayBuffer {
 public:
  // === Core lifecycle methods ===
  void setup() override;
  void dump_config() override;
  void update() override;  // Called automatically by ESPHome
  void flush();
  float get_setup_priority() const override { return setup_priority::HARDWARE; }

  display::DisplayType get_display_type() override {
    return display::DisplayType::DISPLAY_TYPE_COLOR;
  }

 protected:
  // === Rendering methods used by ESPHome ===
  void draw_absolute_pixel_internal(int x, int y, Color color) override;
  void fill(Color color) override;

  int get_width_internal() override { return 480; }
  int get_height_internal() override { return 480; }

 private:
  LilyGo_RGBPanel panel_;
  bool initialized_ = false;
};

}  // namespace lilygo_trgb_display
}  // namespace esphome