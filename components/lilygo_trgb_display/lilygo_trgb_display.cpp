#include "lilygo_trgb_display.h"
#include "esphome/core/log.h"
#include "Arduino.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"

static const char *TAG = "lilygo_trgb";

namespace esphome {
namespace lilygo_trgb_display {

#define LCD_H_RES 480
#define LCD_V_RES 480

#define LCD_PIN_DE   40
#define LCD_PIN_VSYNC 41
#define LCD_PIN_HSYNC 39
#define LCD_PIN_PCLK  42

#define LCD_PIN_R0 45
#define LCD_PIN_R1 48
#define LCD_PIN_R2 47
#define LCD_PIN_R3 21
#define LCD_PIN_R4 14

#define LCD_PIN_G0 5
#define LCD_PIN_G1 6
#define LCD_PIN_G2 7
#define LCD_PIN_G3 15
#define LCD_PIN_G4 16
#define LCD_PIN_G5 4

#define LCD_PIN_B0 8
#define LCD_PIN_B1 3
#define LCD_PIN_B2 46
#define LCD_PIN_B3 9
#define LCD_PIN_B4 1

#define LCD_PIN_BACKLIGHT 38
#define LCD_PIN_RESET 17

static esp_lcd_panel_handle_t panel_handle = NULL;
static esp_lcd_panel_io_handle_t io_handle = NULL;

// Command sequence for ST7701S
typedef struct {
  uint8_t cmd;
  uint8_t data[16];
  uint8_t len;
  uint16_t delay_ms;
} st7701s_cmd_t;

static const st7701s_cmd_t init_cmds[] = {
    {0xFF, {0x77, 0x01, 0x00, 0x00, 0x10}, 0x05},
    {0xC0, {0x3b, 0x00}, 0x02},
    {0xC1, {0x0b, 0x02}, 0x02},
    {0xC2, {0x07, 0x02}, 0x02},
    {0xCC, {0x10}, 0x01},
    {0xCD, {0x08}, 0x01}, // 用565时屏蔽    666打开
    {0xb0, {0x00, 0x11, 0x16, 0x0e, 0x11, 0x06, 0x05, 0x09, 0x08, 0x21, 0x06, 0x13, 0x10, 0x29, 0x31, 0x18}, 0x10},
    {0xb1, {0x00, 0x11, 0x16, 0x0e, 0x11, 0x07, 0x05, 0x09, 0x09, 0x21, 0x05, 0x13, 0x11, 0x2a, 0x31, 0x18}, 0x10},
    {0xFF, {0x77, 0x01, 0x00, 0x00, 0x11}, 0x05},
    {0xb0, {0x6d}, 0x01},
    {0xb1, {0x37}, 0x01},
    {0xb2, {0x81}, 0x01},
    {0xb3, {0x80}, 0x01},
    {0xb5, {0x43}, 0x01},
    {0xb7, {0x85}, 0x01},
    {0xb8, {0x20}, 0x01},
    {0xc1, {0x78}, 0x01},
    {0xc2, {0x78}, 0x01},
    {0xc3, {0x8c}, 0x01},
    {0xd0, {0x88}, 0x01},
    {0xe0, {0x00, 0x00, 0x02}, 0x03},
    {0xe1, {0x03, 0xa0, 0x00, 0x00, 0x04, 0xa0, 0x00, 0x00, 0x00, 0x20, 0x20}, 0x0b},
    {0xe2, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x0d},
    {0xe3, {0x00, 0x00, 0x11, 0x00}, 0x04},
    {0xe4, {0x22, 0x00}, 0x02},
    {0xe5, {0x05, 0xec, 0xa0, 0xa0, 0x07, 0xee, 0xa0, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x10},
    {0xe6, {0x00, 0x00, 0x11, 0x00}, 0x04},
    {0xe7, {0x22, 0x00}, 0x02},
    {0xe8, {0x06, 0xed, 0xa0, 0xa0, 0x08, 0xef, 0xa0, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x10},
    {0xeb, {0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00}, 0x07},
    {0xed, {0xff, 0xff, 0xff, 0xba, 0x0a, 0xbf, 0x45, 0xff, 0xff, 0x54, 0xfb, 0xa0, 0xab, 0xff, 0xff, 0xff}, 0x10},
    {0xef, {0x10, 0x0d, 0x04, 0x08, 0x3f, 0x1f}, 0x06},
    {0xFF, {0x77, 0x01, 0x00, 0x00, 0x13}, 0x05},
    {0xef, {0x08}, 0x01},
    {0xFF, {0x77, 0x01, 0x00, 0x00, 0x00}, 0x05},
    {0x36, {0x08}, 0x01},
    {0x3a, {0x66}, 0x01},
    {0x11, {0x00}, 0x80},
    {0x29, {0x00}, 0x80},
    {0, {0}, 0xff}
};

void LilyGoTRGBDisplay::setup() {
  ESP_LOGCONFIG(TAG, "Initializing LilyGo T-RGB Display (ST7701S 480x480)...");

  pinMode(LCD_PIN_BACKLIGHT, OUTPUT);
  digitalWrite(LCD_PIN_BACKLIGHT, LOW);

  pinMode(LCD_PIN_RESET, OUTPUT);
  digitalWrite(LCD_PIN_RESET, HIGH);
  delay(10);
  digitalWrite(LCD_PIN_RESET, LOW);
  delay(10);
  digitalWrite(LCD_PIN_RESET, HIGH);
  delay(120);

  esp_lcd_rgb_panel_config_t panel_config = {
      .data_width = 16,
      .psram_trans_align = 64,
      .num_fbs = 1,
      .clk_src = LCD_CLK_SRC_DEFAULT,
      .disp_gpio_num = -1,
      .pclk_gpio_num = LCD_PIN_PCLK,
      .vsync_gpio_num = LCD_PIN_VSYNC,
      .hsync_gpio_num = LCD_PIN_HSYNC,
      .de_gpio_num = LCD_PIN_DE,
      .data_gpio_nums = {
          LCD_PIN_B0, LCD_PIN_B1, LCD_PIN_B2, LCD_PIN_B3, LCD_PIN_B4,
          LCD_PIN_G0, LCD_PIN_G1, LCD_PIN_G2, LCD_PIN_G3, LCD_PIN_G4, LCD_PIN_G5,
          LCD_PIN_R0, LCD_PIN_R1, LCD_PIN_R2, LCD_PIN_R3, LCD_PIN_R4},
      .timings =
          {
              .pclk_hz = 16 * 1000 * 1000,
              .h_res = LCD_H_RES,
              .v_res = LCD_V_RES,
              .hsync_back_porch = 10,
              .hsync_front_porch = 20,
              .hsync_pulse_width = 8,
              .vsync_back_porch = 10,
              .vsync_front_porch = 10,
              .vsync_pulse_width = 8,
              .flags = {
                  .hsync_idle_low = false,
                  .vsync_idle_low = false,
                  .de_idle_high = false,
                  .pclk_active_neg = true,
                  .pclk_idle_high = false,
              },
          },
      .flags = {.fb_in_psram = true},
  };

  ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));

  // Send ST7701S initialization commands
  ESP_LOGI(TAG, "Sending ST7701S initialization sequence...");
  for (auto &cmd : init_cmds) {
    ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, cmd.cmd, cmd.data, cmd.len));
    if (cmd.delay_ms)
      delay(cmd.delay_ms);
  }

  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
  digitalWrite(LCD_PIN_BACKLIGHT, HIGH);
  ESP_LOGI(TAG, "ST7701S initialized successfully.");
}

void LilyGoTRGBDisplay::update() {
  this->do_update_();
  this->flush_();
}

void LilyGoTRGBDisplay::draw_absolute_pixel_internal(int x, int y, Color color) {
  // Implement framebuffer write if needed
}

void LilyGoTRGBDisplay::flush_() {
  static int counter = 0;
  if (++counter % 20 == 0)
    ESP_LOGD(TAG, "Flush called");
}

}  // namespace lilygo_trgb_display
}  // namespace esphome
