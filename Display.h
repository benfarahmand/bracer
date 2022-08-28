#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "Adafruit_ILI9341.h"
#include "Adafruit_TSC2007.h"
#include "ESP32Time.h"
#include "Sensors.h"
#include "Button.h"

class Display {
  private:
    Adafruit_ILI9341 tft;
    Adafruit_TSC2007 touch;//touch screen
    Button settingsButton;
    Button homeButton;
    bool wereButtonsDrawn = false;
    bool wasScreenCleared = false;
    bool backLightOn = true;
    int textSize = 2;
    int screen; //0 = home, 1 = settings
    //touch screen calibration values
    float minX, maxX;
    float minY, maxY;
    float screenWidth, screenHeight;
    float xConvert, yConvert;
  public:
    Display();
    void init();
    void draw(ESP32Time& rtc, Sensors& s, String batteryDetails);
    void drawHomeScreen(ESP32Time& rtc, Sensors& s, String batteryDetails);
    void drawSettingsScreen();
    void turnOnBacklight();
    void turnOffBacklight();
    void checkForButtonClicks();
    void drawButtons();
    void clearScreen();
};

#endif
