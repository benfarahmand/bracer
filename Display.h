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
    int textSize = 2;
    int screen; //0 = home, 1 = settings
    //touch screen calibration values
    float minX, maxX;
    float minY, maxY;
    float screenWidth = 320; //pixel value
    float screenHeight = 240; //pixel value
    float xConvert, yConvert;
    uint32_t displayTurnOffTimer;
    int timeToTurnOffDisplay = 45000;//turn off screen after 45 seconds
  public:
    Adafruit_ILI9341 tft;
    bool backLightOn = true;
    
    Display();
    void init();
    void draw(ESP32Time& rtc, Sensors& s, Graph& myGraph, Settings& mySettings, String batteryDetails, String upTime, String gpsFix);
    void drawHomeScreen(ESP32Time& rtc, Sensors& s, String batteryDetails, String upTime, String gpsFix);
    void drawGraphScreen(Graph& myGraph);
    void drawSettingsScreen(Settings& mySettings, ESP32Time& rtc);
    void turnOnBacklight();
    void turnOffBacklight();
    bool checkForButtonClicks(Settings& settingsScreen, Graph& graphScreen);
    void drawButtons();
    void clearScreen();
    void blackScreen();
    void turnOffBacklightAfterSomeTime();
};

#endif
