#include "Display.h"
#include "Adafruit_TSC2007.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "ESP32Time.h"
#include "Sensors.h"
#include "Wireless.h"
#include "Button.h"
#include "Settings.h"
#include "Graph.h"

#define TFT_MISO 19
#define TFT_MOSI 18
#define TFT_CLK 5
#define TFT_CS   16  // Chip select control pin
#define TFT_DC   17  // Data Command control pin
#define TFT_RST  21 // Reset pin (could connect to RST pin)
#define TFT_LIGHT 33
#define TFT_CCS 32 //maybe need a separate class for reading and writing to the SD Card

Display::Display(): tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO),
  settingsButton(), homeButton(), graphButton() {}

void Display::init() {
  screen = 0; //0 = home, 1 = settings
  minX = 0;//380;
  maxX = 3684;
  minY = 0;//585;
  maxY = 3825; //value determined from touching the corners of the resistive touch screen
  //  screenWidth = 320; //pixel value
  //  screenHeight = 240; //pixel value
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

  homeButton.initButton(tft, 0, 0, 80, 40, ILI9341_WHITE, ILI9341_BLACK, ILI9341_WHITE, "Home", 2);
  graphButton.initButton(tft, 80, 0, 120, 40,  ILI9341_WHITE, ILI9341_BLACK, ILI9341_WHITE, "Graph", 2);
  settingsButton.initButton(tft, 200, 0, 120, 40, ILI9341_WHITE, ILI9341_BLACK, ILI9341_WHITE, "Settings", 2);

  if (!touch.begin()) { //wait for touch to begin
    while (1) {
      delay(10);
    }
  }
}

//used for saving battery power
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
    graphButton.drawButton(tft);
    wereButtonsDrawn = true;
  }
}

void Display::checkForButtonClicks(Settings& settingsScreen) {
  uint16_t x, y, z1, z2;
  //due to screen being rotated, the x and y values are flipped
  if (touch.read_touch(&y, &x, &z1, &z2) && (z1 > 50)) {
    x = 320 - (int)(((float)x) * xConvert);
    y = (int)(((float)y) * yConvert);
    Serial.print("x:");
    Serial.println(x);
    Serial.print("y:");
    Serial.println(y);
    //if the backlight is off, then any touch should turn it on
    //eventually it would be nice to have the accelerometer also control the backlight
    if (!backLightOn) {
      turnOnBacklight();
      backLightOn = true;
    } else {

      //buttons for switching between screens
      if (homeButton.contains(x, y) && screen != 0) {
        Serial.println("home screen");
        screen = 0;
        wasScreenCleared = false;
        wereButtonsDrawn = false;
        return;
      }
      if (settingsButton.contains(x, y) && screen != 1) {
        Serial.println("settings screen");
        screen = 1;
        wasScreenCleared = false;
        wereButtonsDrawn = false;
        return;
      }
      if (graphButton.contains(x, y) && screen != 2) {
        Serial.println("graph screen");
        screen = 2;
        wasScreenCleared = false;
        wereButtonsDrawn = false;
        return;
      }

      //now check for specific screen buttons
      if(screen == 1){
        settingsScreen.checkForButtonClicks(x,y);
      }
      
    }
  }
}

void Display::drawHomeScreen(ESP32Time& rtc, Sensors& s, String batteryDetails, String upTime, String gpsFix) {
  tft.setCursor(0, 41);
  tft.setTextSize(textSize);
  tft.println(rtc.getDate());
  tft.setTextSize(textSize * 2);
  tft.println(rtc.getTime());
  tft.setTextSize(textSize);
  tft.print("CO2:");
  tft.print(s.getCO2());
  tft.println("  ");
  tft.print("vOC:");
  tft.print(s.getVOC());
  tft.println("  ");
  tft.print("Pres:");
  tft.print(s.getPressure());
  tft.println(" hPa  ");  
  tft.print("Temp:");
  tft.print(s.getTemp());
  tft.println("  ");
  tft.print("Hum:");
  tft.print(s.getHumidity());
  tft.println("  ");
  //can add vOC information here
  //can also add pulse information here
  //should also add some information about whether GPS has a fix or not
  tft.println(batteryDetails);
  tft.print("Up:");
  tft.println(upTime);
  tft.print("GPS Fix:");
  tft.print(gpsFix);
}

void Display::draw(ESP32Time& rtc, Sensors& s, Graph& myGraph, Settings& mySettings, String batteryDetails, String upTime, String gpsFix) {
  if (!wasScreenCleared) {
    clearScreen();
  }
  if (screen == 0) {
    drawHomeScreen(rtc, s, batteryDetails, upTime, gpsFix);
  }
  else if (screen == 1) {
    drawSettingsScreen(mySettings, rtc);
  }
  else if (screen == 2) {
    drawGraphScreen(myGraph);
  }
  else {
    return;
  }
  drawButtons();
}

void Display::drawSettingsScreen(Settings& mySettings, ESP32Time& rtc) {
  tft.setCursor(0, 42);
  //settings options: high power mode and low power mode
  //1. high power mode can collect data frequently.
  //2. low power mode can collect data once every 5 or so minutes. how can this work?
  //3. set display brightness. apparently, it can only be on or off. So we can't have a display brightness setting. but we could try to pass a spcific value to the TFT and see what happens.
  //4. set time, maybe a button can be pressed to grab time from the GPS module
  //5. log data, maybe this is a button instead or maybe it can have an auto logging mode too
  //6. turn on/off wifi for transfering data
  //7. turn on/off BT for notifications?
  //8. show total up time and amount of battery voltage used, maybe this can be
  //9. Sync the time with GPS time
  tft.setTextSize(textSize);
  tft.println(rtc.getDate());
  tft.setTextSize(textSize * 2);
  tft.println(rtc.getTime());
  tft.setTextSize(textSize);
  mySettings.draw(tft);
}

void Display::drawGraphScreen(Graph& myGraph) {
  tft.setCursor(0, 42);
  //the graph display will show time series data and can include the following:
  //1. CO2
  //2. Humidity
  //3. Temp
  //4. vOC
  //5. battery usage over time
  //6. pulse over time
  myGraph.draw(tft);
}

void Display::clearScreen() {
  //  tft.fillScreen(ILI9341_BLACK);
  //instead of filling the entire screen with black,
  //we can draw a rect over the top portion of the screen and not have to redraw the buttons
  tft.fillRect(0, 41, screenWidth, 198, ILI9341_BLACK);
  wasScreenCleared = true;
}
