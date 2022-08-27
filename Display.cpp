#include "Display.h"
#include "Adafruit_TSC2007.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "ESP32Time.h"
#include "Sensors.h"
#include "Wireless.h"
#include "Button.h"

#define TFT_MISO 19
#define TFT_MOSI 18
#define TFT_CLK 5
#define TFT_CS   16  // Chip select control pin
#define TFT_DC   17  // Data Command control pin
#define TFT_RST  21 // Reset pin (could connect to RST pin)
#define TFT_LIGHT 33
#define TFT_CCS 32

Display::Display(): tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO), settingsButton() {}

void Display::init() {
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(textSize);

  pinMode(TFT_LIGHT, OUTPUT);
  digitalWrite(TFT_LIGHT, HIGH);


  //  settingsLabel = "Settings";
    settingsButton.initButton(tft, 0, 200, 120, 40, ILI9341_WHITE, ILI9341_BLACK, ILI9341_WHITE, "Settings", 2);


  if (!touch.begin()) {
    while (1) {
      delay(10);
    }
  }

  //    tft.fillScreen(ILI9341_BLACK);
  //  return tft;
}

void Display::turnOffBacklight() {
  digitalWrite(TFT_LIGHT, LOW);
}

void Display::turnOnBacklight() {
  digitalWrite(TFT_LIGHT, HIGH);
}

void Display::drawButtons(){
  if (!wereButtonsDrawn) {
    //insert buttons here to draw
    settingsButton.drawButton(tft);
    wereButtonsDrawn = true;
  }
}

void Display::drawHomeScreen(ESP32Time rtc, Sensors s, String batteryDetails) {
  tft.setCursor(0, 0);
  tft.setTextSize(textSize);
  tft.println(rtc.getDate());
  tft.setTextSize(textSize*2);
  tft.println(rtc.getTime());
  tft.setTextSize(textSize);
  tft.print("CO2:");
  tft.println(s.getCO2());
  tft.print("Temp:");
  tft.println(s.getTemp());
  tft.print("Hum:");
  tft.println(s.getHumidity());
  tft.println(batteryDetails);
  drawButtons();
}

void Display::checkForButtonClicks() {
  uint16_t x, y, z1, z2;
  if (touch.read_touch(&x, &y, &z1, &z2) && (z1 > 100)) {
    //    deepSleep(tft);
    if(backLightOn){
      turnOffBacklight();
      backLightOn = false;
    }
    else {
      turnOnBacklight();
      backLightOn = true;
    }
  }
}

void Display::changeScreen() {

}

void Display::drawSettingsScreen() {
  //settings options: high power mode and low power mode
  //1. high power mode can collect data frequently
  //2. low power mode can collect data once every 5 or so minutes
  //3. set display brightness
  //4. set time, maybe a button can be pressed to grab time from the GPS module
  //5. log data, maybe this is a button instead
  //6. turn on/off wifi for transfering data
  //7. turn on/off BT for notifications?
  //8. show total up time and amount of battery voltage used
  //  return tft;
}
