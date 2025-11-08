#include "lilygo_trgb_display.h"
#include "esphome/core/log.h"
#include "Arduino.h"
#include <LilyGo_RGBPanel.h>

static LGFX lcd;

static const char *TAG = "lilygo_trgb_display";

namespace esphome {
namespace lilygo_trgb_display {

void LilyGoTRGBDisplay::setup() {
  ESP_LOGCONFIG(TAG, "Initializing LilyGo T-RGB Display with LGFX driver...");

  framebuffer_ = (uint16_t *)heap_caps_malloc(480 * 480 * 2,
                                              MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  if (!framebuffer_) {
    ESP_LOGE(TAG, "Failed to allocate framebuffer!");
    return;
  }

  lcd.begin();        // initializes RGB + ST7701S
  lcd.setRotation(0);
  lcd.fillScreen(TFT_BLACK);

  ESP_LOGI(TAG, "Framebuffer allocated (%lu bytes)", 480UL * 480 * 2);
}

void LilyGoTRGBDisplay::update() {
  this->do_update_();
  this->flush_();
}

// Write pixel into framebuffer
void LilyGoTRGBDisplay::draw_absolute_pixel_internal(int x, int y, Color color) {
  if (x < 0 || y < 0 || x >= 480 || y >= 480 || framebuffer_ == nullptr)
    return;

  uint16_t c = ((color.red & 0xF8) << 8) | ((color.green & 0xFC) << 3) |
               ((color.blue & 0xF8) >> 3);
  framebuffer_[y * 480 + x] = c;
}

// Push framebuffer to LCD
void LilyGoTRGBDisplay::flush_() {
  if (!framebuffer_)
    return;

  // Push the whole framebuffer to the display
  lcd.pushImage(0, 0, 480, 480, framebuffer_);

  static uint32_t counter = 0;
  if ((++counter % 20) == 0)
    ESP_LOGD(TAG, "Flushing framebuffer (%lu frames)", counter);
}

}  // namespace lilygo_trgb_display
}  // namespace esphome
