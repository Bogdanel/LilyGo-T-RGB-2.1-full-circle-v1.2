#include "lilygo_trgb_display.h"
#include "esphome/core/log.h"
#include "Arduino.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

static const char *TAG = "lilygo_trgb";

namespace esphome {
namespace lilygo_trgb_display {

// ST7701S initialization sequence (from LilyGo repo)
static const uint8_t st7701s_init_cmds[][16] = {
    {0xFF, 3, 0x77, 0x01, 0x00},
    {0xEF, 1, 0x08},
    {0xFF, 3, 0x77, 0x01, 0x00},
    {0xC0, 2, 0x3B, 0x00},
    {0xC1, 2, 0x10, 0x02},
    {0xC2, 2, 0x07, 0x02},
    {0xCC, 1, 0x10},
    {0xCD, 1, 0x08},
    {0xB0, 16, 0x00, 0x10, 0x1A, 0x0E, 0x11, 0x06, 0x07, 0x08, 0x07, 0x22, 0x04, 0x12, 0x0F, 0x29, 0x31, 0x18},
    {0xB1, 16, 0x00, 0x10, 0x1A, 0x0E, 0x11, 0x07, 0x08, 0x08, 0x07, 0x22, 0x04, 0x12, 0x0F, 0x29, 0x31, 0x18},
    {0xFF, 3, 0x77, 0x01, 0x00},
    {0xB2, 2, 0x80, 0xFE},
    {0xE0, 2, 0x00, 0x00},
    {0x36, 1, 0x00},
    {0x3A, 1, 0x70},
    {0x21, 0},
    {0x11, 0},
    {0x29, 0}
};

void LilyGoTRGBDisplay::setup() {
  ESP_LOGCONFIG(TAG, "Initializing LilyGo T-RGB Display (ST7701S 480x480)...");

  // Backlight & reset pins
  pinMode(38, OUTPUT);
  digitalWrite(38, LOW);
  pinMode(17, OUTPUT);
  digitalWrite(17, HIGH);
  delay(50);
  digitalWrite(17, LOW);
  delay(50);
  digitalWrite(17, HIGH);
  delay(120);

  // Configure RGB interface using esp_lcd API
  esp_lcd_rgb_panel_config_t panel_config = {
      .data_width = 16,
      .psram_trans_align = 64,
      .num_fbs = 1,
      .bounce_buffer_size_px = 10,
      .clk_src = LCD_CLK_SRC_DEFAULT,
      .timings =
          {
              .pclk_hz = 16000000,
              .h_res = 480,
              .v_res = 480,
              .hsync_pulse_width = 10,
              .hsync_back_porch = 20,
              .hsync_front_porch = 10,
              .vsync_pulse_width = 10,
              .vsync_back_porch = 20,
              .vsync_front_porch = 10,
              .flags = {.pclk_active_neg = false},
          },
      .data_gpio_nums = {39, 0, 1, 2, 3, 4, 5, 7, 15, 16, 9, 46, 14, 13, 12, 11},
      .hsync_gpio_num = 40,
      .vsync_gpio_num = 41,
      .de_gpio_num = 45,
      .pclk_gpio_num = 42,
      .disp_gpio_num = -1,
      .flags =
          {
              .fb_in_psram = 1,
              .double_fb = 0,
              .disp_active_low = 0,
          },
  };

  esp_lcd_panel_handle_t panel_handle;
  esp_lcd_new_rgb_panel(&panel_config, &panel_handle);

  esp_lcd_panel_reset(panel_handle);

  // Send ST7701S initialization commands
  for (auto &cmd : st7701s_init_cmds) {
    uint8_t cmd_id = cmd[0];
    uint8_t param_count = cmd[1];
    if (param_count > 0)
      esp_lcd_panel_io_tx_param(panel_handle, cmd_id, &cmd[2], param_count);
    else
      esp_lcd_panel_io_tx_param(panel_handle, cmd_id, nullptr, 0);
    delay(10);
  }

  esp_lcd_panel_disp_on_off(panel_handle, true);
  digitalWrite(38, HIGH);  // Backlight on

  ESP_LOGI(TAG, "Display initialized OK");
}

void LilyGoTRGBDisplay::update() {
  this->do_update_();
  this->flush_();
}

void LilyGoTRGBDisplay::draw_absolute_pixel_internal(int x, int y, Color color) {
  // You can later implement framebuffer-based rendering
}

void LilyGoTRGBDisplay::flush_() {
  // For now, just placeholder
}

}  // namespace lilygo_trgb_display
}  // namespace esphome
