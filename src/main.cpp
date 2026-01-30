#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define TFT_CS     5
#define TFT_DC     2
#define TFT_RST    4

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define OLED_ADDRESS  0x3C

#define DHTPIN 27
#define DHTTYPE DHT11

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  tft.initR(INITR_144GREENTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  if(!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("OLED failed"));
  }
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);

  dht.begin();

  tft.drawRect(0, 0, 128, 128, ST77XX_BLUE);
  tft.setCursor(30, 10);
  tft.setTextColor(ST77XX_CYAN);
  tft.setTextSize(2);
  tft.print("SENSOR");
  tft.drawLine(0, 30, 128, 30, ST77XX_BLUE);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    tft.setCursor(10, 50); tft.setTextColor(ST77XX_RED, ST77XX_BLACK); tft.print("DHT ERROR");
    oled.setCursor(0,0); oled.clearDisplay(); oled.print("DHT ERROR"); oled.display();
    return;
  }

  tft.setCursor(10, 45);
  tft.setTextColor(ST77XX_ORANGE, ST77XX_BLACK);
  tft.setTextSize(1);
  tft.print("Temperature:");

  tft.setCursor(10, 58);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setTextSize(3);
  tft.print(t, 1);
  tft.setTextSize(2);
  tft.print("C  ");

  tft.setCursor(10, 90);
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.setTextSize(1);
  tft.print("Humidity:");

  tft.setCursor(10, 103);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setTextSize(3);
  tft.print((int)h);
  tft.setTextSize(2);
  tft.print("%  ");

  oled.clearDisplay();
  oled.drawRect(0, 0, 128, 64, SSD1306_WHITE);

  oled.setTextSize(1);
  oled.setCursor(40, 3);
  oled.print("MONITOR");
  oled.drawLine(0, 12, 128, 12, SSD1306_WHITE);

  oled.setCursor(5, 20); 
  oled.print("TEMP");
  oled.setTextSize(2); 
  oled.setCursor(5, 35);
  oled.print(t, 1);
  oled.setTextSize(1); 
  oled.cp437(true); 
  oled.write(248); 
  oled.print("C");

  oled.setTextSize(1); 
  oled.setCursor(70, 20); 
  oled.print("HUMI");
  oled.setTextSize(2); 
  oled.setCursor(70, 35);
  oled.print((int)h); 
  oled.print("%");

  oled.display();

  delay(2000);
}
