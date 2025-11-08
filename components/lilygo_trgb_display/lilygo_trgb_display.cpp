#include "lilygo_trgb_display.h"
#include "esphome/core/log.h"
#include "Arduino.h"
#include <LilyGo_RGBPanel.h>


static const char *TAG = "lilygo_trgb_display";

namespace esphome {
namespace lilygo_trgb_display {

static LGFX lcd;

void LilyGoTRGBDisplay::setup() {
  ESP_LOGCONFIG(TAG, "Initializing LilyGo T-RGB Display (LGFX driver)");

  // Initialize RGB driver and panel
  lcd.begin();
  lcd.setRotation(0);
  lcd.fillScreen(TFT_BLACK);

  // Allocate framebuffer in PSRAM (480x480x2 = ~450kB)
  framebuffer_ = (uint16_t *)heap_caps_malloc(480 * 480 * sizeof(uint16_t),
                                              MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  if (!framebuffer_) {
    ESP_LOGE(TAG, "Failed to allocate framebuffer!");
    return;
  }
  memset(framebuffer_, 0, 480 * 480 * 2);
  ESP_LOGI(TAG, "Framebuffer allocated (%lu bytes)", 480UL * 480 * 2);
}

void LilyGoTRGBDisplay::update() {
  this->do_update_();
  this->flush_();
}

// Convert ESPHome Color (8-bit RGB) to RGB565 and store in framebuffer
void LilyGoTRGBDisplay::draw_absolute_pixel_internal(int x, int y, Color color) {
  if (!framebuffer_ || x < 0 || y < 0 || x >= 480 || y >= 480)
    return;

  uint16_t c = ((color.red & 0xF8) << 8) |
               ((color.green & 0xFC) << 3) |
               ((color.blue & 0xF8) >> 3);
  framebuffer_[y * 480 + x] = c;
}

// Flush framebuffer to display using LGFX pushImage()
void LilyGoTRGBDisplay::flush_() {
  if (!framebuffer_)
    return;

  lcd.pushImage(0, 0, 480, 480, framebuffer_);

  static uint32_t frame_counter = 0;
  if (++frame_counter % 20 == 0)
    ESP_LOGD(TAG, "Flushing framebuffer (%lu frames)", frame_counter);
}

}  // namespace lilygo_trgb_display
}  // namespace esphome
