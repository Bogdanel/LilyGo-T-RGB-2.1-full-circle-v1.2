#include "lilygo_trgb_display.h"
#include "esphome/core/log.h"
#include "Arduino.h"

static const char *TAG = "lilygo_trgb_display";

namespace esphome {
namespace lilygo_trgb_display {

#define LCD_PIN_BLK   38
#define LCD_PIN_RST   17
#define LCD_PIN_CS    14
#define LCD_PIN_SDA   18
#define LCD_PIN_SCL   45

// --- ST7701S low-level 3-wire serial interface (bit-bang) ---
static void lcd_write_cmd(uint8_t cmd) {
  digitalWrite(LCD_PIN_CS, LOW);
  digitalWrite(LCD_PIN_SCL, LOW);
  digitalWrite(LCD_PIN_SDA, LOW);  // D/C bit = 0 for command
  digitalWrite(LCD_PIN_SCL, HIGH);
  for (int i = 7; i >= 0; i--) {
    digitalWrite(LCD_PIN_SCL, LOW);
    digitalWrite(LCD_PIN_SDA, (cmd >> i) & 0x01);
    digitalWrite(LCD_PIN_SCL, HIGH);
  }
  digitalWrite(LCD_PIN_CS, HIGH);
}

static void lcd_write_data(uint8_t data) {
  digitalWrite(LCD_PIN_CS, LOW);
  digitalWrite(LCD_PIN_SCL, LOW);
  digitalWrite(LCD_PIN_SDA, HIGH); // D/C bit = 1 for data
  digitalWrite(LCD_PIN_SCL, HIGH);
  for (int i = 7; i >= 0; i--) {
    digitalWrite(LCD_PIN_SCL, LOW);
    digitalWrite(LCD_PIN_SDA, (data >> i) & 0x01);
    digitalWrite(LCD_PIN_SCL, HIGH);
  }
  digitalWrite(LCD_PIN_CS, HIGH);
}

// --- Minimal ST7701S init commands (safe defaults) ---
static const uint8_t init_sequence[][6] = {
    {0xFF, 0x77, 0x01, 0x00, 0x00, 0x10},
    {0xC0, 0x3B, 0x00},
    {0xC1, 0x0B, 0x02},
    {0xC2, 0x07, 0x02},
    {0xCC, 0x10},
    {0xB0, 0x00},
};

static void st7701s_init() {
  for (auto &seq : init_sequence) {
    lcd_write_cmd(seq[0]);
    for (int i = 1; i < 6 && seq[i] != 0x00; i++)
      lcd_write_data(seq[i]);
    delay(5);
  }

  // Display ON sequence
  lcd_write_cmd(0x11);
  delay(120);
  lcd_write_cmd(0x29);
  delay(20);
}

void LilyGoTRGBDisplay::setup() {
  ESP_LOGCONFIG(TAG, "Initializing LilyGo T-RGB Display (Arduino mode)");

  pinMode(LCD_PIN_BLK, OUTPUT);
  pinMode(LCD_PIN_RST, OUTPUT);
  pinMode(LCD_PIN_CS, OUTPUT);
  pinMode(LCD_PIN_SDA, OUTPUT);
  pinMode(LCD_PIN_SCL, OUTPUT);

  // Reset sequence
  digitalWrite(LCD_PIN_RST, LOW);
  delay(100);
  digitalWrite(LCD_PIN_RST, HIGH);
  delay(100);

  // Send initialization
  st7701s_init();

  // Enable backlight
  digitalWrite(LCD_PIN_BLK, HIGH);
  ESP_LOGI(TAG, "ST7701S initialized successfully (bit-bang mode).");
}

void LilyGoTRGBDisplay::update() {
  this->do_update_();
  this->flush_();
}

void LilyGoTRGBDisplay::draw_absolute_pixel_internal(int x, int y, Color color) {
  // TODO: Implement framebuffer drawing if needed
}

void LilyGoTRGBDisplay::flush_() {
  // TODO: Send framebuffer data to RGB lines if supported later
  static int counter = 0;
  if (++counter % 20 == 0)
    ESP_LOGD(TAG, "Flush called (stub)");
}

}  // namespace lilygo_trgb_display
}  // namespace esphome
