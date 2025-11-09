#include "lilygo_trgb_display.h"
#include "esphome/core/log.h"
#include "esp_heap_caps.h"
#include <cstring>

namespace esphome {
namespace lilygo_trgb_display {

static const char *const TAG = "lilygo_trgb_display";


void LilyGoTRGBDisplay::setup() {
  ESP_LOGI(TAG, "Initializing LilyGo T-RGB display...");

  // Initialize panel
  if (!panel_.begin(LILYGO_T_RGB_2_1_INCHES_FULL_CIRCLE, LILYGO_T_RGB_ORDER_RGB)) {
    ESP_LOGE(TAG, "Panel initialization failed!");
    this->mark_failed();
    return;
  }

  panel_.setBrightness(16);

  int w = this->get_width_internal();
  int h = this->get_height_internal();

  
  const size_t bytes = (size_t)w * (size_t)h * sizeof(uint16_t);

  ESP_LOGI(TAG, "Allocating framebuffer %dx%d (%d bytes)", w, h, w * h * 2);
  
  this->init_internal_(bytes);

  ESP_LOGI(TAG, "Panel initialized successfully (%dx%d)", w, h);
  ESP_LOGI(TAG, "Free PSRAM: %d bytes", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));

  ESP_LOGI(TAG, "Running panel color test...");
  uint16_t *buf565 = reinterpret_cast<uint16_t *>(this->buffer_);

  const uint16_t colors[] = {
    display::ColorUtil::color_to_565(Color(255, 0, 0)),   // Red
    display::ColorUtil::color_to_565(Color(0, 255, 0)),   // Green
    display::ColorUtil::color_to_565(Color(0, 0, 255)),   // Blue
    display::ColorUtil::color_to_565(Color(255, 255, 255)) // White
  };

  for (auto c : colors) {
    for (int i = 0; i < w * h; i++) buf565[i] = c;
    esp_lcd_panel_draw_bitmap(panel_.get_panel(), 0, 0, w, h, buf565);
    delay(100);
  }

  ESP_LOGI(TAG, "Color test complete. Clearing screen...");
  this->fill(Color(0, 0, 0));
}

void LilyGoTRGBDisplay::fill(Color color) {
  if (!this->buffer_) return;

  uint16_t color565 = display::ColorUtil::color_to_565(color);
  uint16_t *buf = reinterpret_cast<uint16_t *>(this->buffer_);
  const int pixels = this->get_width_internal() * this->get_height_internal();

  for (int i = 0; i < pixels; i++) buf[i] = color565;
}

void LilyGoTRGBDisplay::draw_absolute_pixel_internal(int x, int y, Color color) {
  if (x < 0 || y < 0 || x >= this->get_width_internal() || y >= this->get_height_internal()) return;
  uint16_t *buf = reinterpret_cast<uint16_t *>(this->buffer_);
  buf[y * this->get_width_internal() + x] = display::ColorUtil::color_to_565(color);
}

void LilyGoTRGBDisplay::update() {
  if (this->buffer_ == nullptr) {
    ESP_LOGE(TAG, "Framebuffer not allocated!");
    return;
  }

  this->do_update_();

  // Check if lambda rendered anything
  bool all_zero = true;
  uint16_t *buf = reinterpret_cast<uint16_t *>(this->buffer_);
  for (int i = 0; i < 256; i++) {
    if (buf[i] != 0) {
      all_zero = false;
      break;
    }
  }

  this->flush();
}

void LilyGoTRGBDisplay::dump_config() {
  LOG_DISPLAY("", "LilyGo T-RGB Display", this);
}

void LilyGoTRGBDisplay::flush() {
  if (this->buffer_ == nullptr) return;

  panel_.pushColors(0, 0, this->get_width_internal(), this->get_height_internal(), reinterpret_cast<uint16_t *>(this->buffer_));
}

}  // namespace lilygo_trgb_display
}  // namespace esphome
