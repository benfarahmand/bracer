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
#define TFT_CCS 32 //maybe need a separate class for reading and writing to the SD Card

Display::Display(): tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO), settingsButton(), homeButton() {}

void Display::init() {
  screen = 0; //0 = home, 1 = settings
  minX = 0;//380;
  maxX = 3684;
  minY = 0;//585; 
  maxY = 3825; //value determined from touching the corners of the resistive touch screen
  screenWidth = 320; //pixel value
  screenHeight = 240; //pixel value
  xConvert = (screenWidth / (maxX - minX));
  yConvert = (screenHeight / (maxY - minY));
  
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(textSize);

  pinMode(TFT_LIGHT, OUTPUT);
  digitalWrite(TFT_LIGHT, HIGH);


  //  settingsLabel = "Settings";
  homeButton.initButton(tft, 0, 200, 80, 40, ILI9341_WHITE, ILI9341_BLACK, ILI9341_WHITE, "Home", 2);
  settingsButton.initButton(tft, 80, 200, 120, 40, ILI9341_WHITE, ILI9341_BLACK, ILI9341_WHITE, "Settings", 2);

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

void Display::drawButtons() {
  if (!wereButtonsDrawn) {
    //insert buttons here to draw
    settingsButton.drawButton(tft);
    homeButton.drawButton(tft);
    wereButtonsDrawn = true;
  }
}

void Display::checkForButtonClicks() {
  uint16_t x, y, z1, z2;
  //due to screen being rotated, the x and y values are flipped
  if (touch.read_touch(&y, &x, &z1, &z2) && (z1 > 50)) {
    x = 320 - (int)(((float)x) * xConvert);
    y = (int)(((float)y) * yConvert);
    //if the backlight is off, then any touch should turn it on
    //eventually it would be nice to have the accelerometer also control the backlight
    if (!backLightOn) {
      turnOnBacklight();
      backLightOn = true;
    } else {
      if (homeButton.contains(x, y) && screen != 0) {
        screen = 0;
        wasScreenCleared = false;
        wereButtonsDrawn = false;
        return;
      }
      if (settingsButton.contains(x, y) && screen != 1) {
        screen = 1;
        wasScreenCleared = false;
        wereButtonsDrawn = false;
        return;
      }
    }
  }
}

void Display::drawHomeScreen(ESP32Time& rtc, Sensors& s, String batteryDetails) {
  tft.setCursor(0, 0);
  tft.setTextSize(textSize);
  tft.println(rtc.getDate());
  tft.setTextSize(textSize * 2);
  tft.println(rtc.getTime());
  tft.setTextSize(textSize);
  tft.print("CO2:");
  tft.println(s.getCO2());
  tft.print("Temp:");
  tft.println(s.getTemp());
  tft.print("Hum:");
  tft.println(s.getHumidity());
  //can add vOC information here
  //can also add pulse information here
  //should also add some information about whether GPS has a fix or not
  tft.println(batteryDetails);
  drawButtons();
}

void Display::draw(ESP32Time& rtc, Sensors& s, String batteryDetails) {
  if(!wasScreenCleared) {
    clearScreen();
  }
  if (screen == 0) {
    drawHomeScreen(rtc, s, batteryDetails);
  }
  else if (screen == 1) {
    drawSettingsScreen();
  } else {
    return;
  }
}

void Display::drawSettingsScreen() {
  tft.setCursor(0, 0);
  //settings options: high power mode and low power mode
  //1. high power mode can collect data frequently. 
  //2. low power mode can collect data once every 5 or so minutes. how can this work?
  //3. set display brightness. apparently, it can only be on or off. So we can't have a display brightness setting. but we could try to pass a spcific value to the TFT and see what happens.
  //4. set time, maybe a button can be pressed to grab time from the GPS module
  //5. log data, maybe this is a button instead or maybe it can have an auto logging mode too
  //6. turn on/off wifi for transfering data
  //7. turn on/off BT for notifications?
  //8. show total up time and amount of battery voltage used
  //9. Sync the time with GPS time
  drawButtons();
}

void Display::drawGraphScreen(){
  
}

void Display::clearScreen(){
  // tft.fillScreen(ILI9341_BLACK);
  //instead of filling the entire screen with black, 
  //we can draw a rect over the top portion of the screen and not have to redraw the buttons
  tft.drawRect(0,0,screenWidth,199,ILI9341_BLACK);
  wasScreenCleared=true;
}
