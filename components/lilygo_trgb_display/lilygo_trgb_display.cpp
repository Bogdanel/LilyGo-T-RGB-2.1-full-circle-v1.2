#include "lilygo_trgb_display.h"
#include "esphome/core/log.h"
#include "Arduino.h"

static const char *TAG = "lilygo_trgb";

namespace esphome {
namespace lilygo_trgb_display {

void LilyGoTRGBDisplay::setup() {
  ESP_LOGCONFIG(TAG, "Initializing LilyGo T-RGB Display (ST7701S 480x480)...");
  pinMode(38, OUTPUT);   // Example: backlight
  digitalWrite(38, HIGH);
  ESP_LOGI(TAG, "Display initialized.");
}

void LilyGoTRGBDisplay::update() {
  this->do_update_();
  this->flush_();
}

void LilyGoTRGBDisplay::draw_absolute_pixel_internal(int x, int y, Color color) {
  // Example: Framebuffer write stub
}

void LilyGoTRGBDisplay::flush_() {
  static int counter = 0;
  if (++counter % 10 == 0)
    ESP_LOGD(TAG, "Flush called (stub).");
}

}  // namespace lilygo_trgb_display
}  // namespace esphome
