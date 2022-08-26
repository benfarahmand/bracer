#include "Display.h"
#include "Adafruit_TSC2007.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_MISO 19
#define TFT_MOSI 18
#define TFT_CLK 5
#define TFT_CS   16  // Chip select control pin
#define TFT_DC   17  // Data Command control pin
#define TFT_RST  21 // Reset pin (could connect to RST pin)
// #define TFT_LIGHT

Display::Display():tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO) {}

Adafruit_ILI9341 Display::init() {
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2.5);

  Adafruit_TSC2007 touch;//touch screen
  if (!touch.begin()) {
    while (1) {
      delay(10);
    }
  }

  //  tft.fillScreen(ILI9341_BLACK);
  return tft;
}

Adafruit_ILI9341 Display::homeScreen(Adafruit_ILI9341 tft) {
  //can show date and time with a settings button
  return tft;
}

Adafruit_ILI9341 Display::settingsScreen(Adafruit_ILI9341 tft) {
  return tft;
}
