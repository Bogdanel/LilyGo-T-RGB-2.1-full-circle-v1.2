#include "lilygo_trgb_display.h"
#include "esphome/core/log.h"
#include "Arduino.h"

static const char *TAG = "lilygo_trgb_display";

namespace esphome {
namespace lilygo_trgb_display {

void LilyGoTRGBDisplay::setup() {
  ESP_LOGCONFIG(TAG, "Initializing LilyGo T-RGB Display (ST7701S 480x480)");

  // Allocate framebuffer in PSRAM
  framebuffer_ = (uint16_t *)ps_malloc(480 * 480 * sizeof(uint16_t));
  if (!framebuffer_) {
    ESP_LOGE(TAG, "Failed to allocate framebuffer!");
    return;
  }
  memset(framebuffer_, 0, 480 * 480 * 2);

  // Init GPIOs & reset sequence
  pinMode(38, OUTPUT);  // BLK
  pinMode(17, OUTPUT);  // RST
  digitalWrite(38, LOW);
  digitalWrite(17, LOW);
  delay(100);
  digitalWrite(17, HIGH);
  delay(100);

  // Run ST7701S init (bit-bang) here — omitted for brevity
  // st7701s_init();

  digitalWrite(38, HIGH);  // backlight on
  ESP_LOGI(TAG, "Framebuffer allocated and backlight enabled.");
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

  // TODO: Replace with actual RGB data push
  // For now, log flush and maybe toggle a GPIO
  static uint32_t counter = 0;
  if ((++counter % 20) == 0)
    ESP_LOGD(TAG, "Flushing framebuffer (%lu frames)", counter);
}

}  // namespace lilygo_trgb_display
}  // namespace esphome
