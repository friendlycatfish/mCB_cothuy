#include <Arduino.h> // Bắt buộc phải có dòng này trong PlatformIO
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>  // Thư viện TFT
#include <Adafruit_SSD1306.h> // Thư viện OLED
#include <Adafruit_AHTX0.h>   // Thư viện AHT

// --- CẤU HÌNH CHÂN TFT (SPI) ---
#define TFT_CS     5
#define TFT_DC     2
#define TFT_RST    4
// TFT_MOSI = 23, TFT_SCLK = 18 (Tự nhận)

// --- CẤU HÌNH OLED (I2C) ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define OLED_ADDRESS  0x3C

// --- KHỞI TẠO ĐỐI TƯỢNG ---
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_AHTX0 aht;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // I2C cho AHT và OLED

  // 1. KHỞI TẠO TFT
  tft.initR(INITR_144GREENTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  // 2. KHỞI TẠO OLED
  if(!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("OLED failed"));
    // Vẫn chạy tiếp để hiển thị TFT
  }
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);

  // 3. KHỞI TẠO CẢM BIẾN
  if (!aht.begin()) {
    tft.setCursor(10, 50); tft.setTextColor(ST77XX_RED); tft.print("AHT ERROR");
    oled.setCursor(0,0); oled.print("AHT ERROR"); oled.display();
    while (1) delay(100);
  }

  // 4. VẼ GIAO DIỆN TĨNH (TFT)
  tft.drawRect(0, 0, 128, 128, ST77XX_BLUE);
  tft.setCursor(30, 10);
  tft.setTextColor(ST77XX_CYAN);
  tft.setTextSize(2);
  tft.print("SENSOR");
  tft.drawLine(0, 30, 128, 30, ST77XX_BLUE);
}

void loop() {
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);

  // === PHẦN 1: HIỂN THỊ TFT ===
  // Temp
  tft.setCursor(10, 45);
  tft.setTextColor(ST77XX_ORANGE, ST77XX_BLACK);
  tft.setTextSize(1);
  tft.print("Temperature:");

  tft.setCursor(10, 58);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setTextSize(3);
  tft.print(temp.temperature, 1);
  tft.setTextSize(2);
  tft.print("C  ");

  // Humi
  tft.setCursor(10, 90);
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.setTextSize(1);
  tft.print("Humidity:");

  tft.setCursor(10, 103);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setTextSize(3);
  tft.print((int)humidity.relative_humidity);
  tft.setTextSize(2);
  tft.print("%  ");

  // === PHẦN 2: HIỂN THỊ OLED ===
  oled.clearDisplay();
  oled.drawRect(0, 0, 128, 64, SSD1306_WHITE);

  oled.setTextSize(1);
  oled.setCursor(40, 3);
  oled.print("MONITOR");
  oled.drawLine(0, 12, 128, 12, SSD1306_WHITE);

  // Temp (OLED)
  oled.setCursor(5, 20); oled.print("TEMP");
  oled.setTextSize(2); oled.setCursor(5, 35);
  oled.print(temp.temperature, 1);
  oled.setTextSize(1); oled.cp437(true); oled.write(248); oled.print("C");

  // Humi (OLED)
  oled.setTextSize(1); oled.setCursor(70, 20); oled.print("HUMI");
  oled.setTextSize(2); oled.setCursor(70, 35);
  oled.print((int)humidity.relative_humidity); oled.print("%");

  oled.display();

  delay(2000);
}