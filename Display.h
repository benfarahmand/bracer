#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "Adafruit_ILI9341.h"
#include "Adafruit_TSC2007.h"
#include "ESP32Time.h"
#include "Sensors.h"
#include "Button.h"
#include "Settings.h"
#include "Graph.h"

class Display {
  private:
    Adafruit_TSC2007 touch;//touch screen
    Button settingsButton;
    Button homeButton;
    Button graphButton;
    bool wereButtonsDrawn = false;
    bool wasScreenCleared = false;
    bool backLightOn = true;
    int textSize = 2;
    int screen; //0 = home, 1 = settings
    //touch screen calibration values
    float minX, maxX;
    float minY, maxY;
    float screenWidth = 320; //pixel value
    float screenHeight = 240; //pixel value
    float xConvert, yConvert;
  public:
    Adafruit_ILI9341 tft;
    
    Display();
    void init();
    void draw(ESP32Time& rtc, Sensors& s, Graph& myGraph, Settings& mySettings, String batteryDetails, String upTime, String gpsFix);
    void drawHomeScreen(ESP32Time& rtc, Sensors& s, String batteryDetails, String upTime, String gpsFix);
    void drawGraphScreen(Graph& myGraph);
    void drawSettingsScreen(Settings& mySettings);
    void turnOnBacklight();
    void turnOffBacklight();
    void checkForButtonClicks(Settings& settingsScreen);
    void drawButtons();
    void clearScreen();
};

#endif
