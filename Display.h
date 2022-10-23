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
    int timeToTurnOffDisplay = 60000;//turn off screen after 45 seconds
  public:
    Adafruit_ILI9341 tft;
    bool backLightOn = true;
    
    Display();
    Graph* myGraph;
    ESP32Time* rtc;
    Sensors* s;
    Settings* mySettings;
    void init(ESP32Time& _rtc, Sensors& _s, Settings& _mySettings, Graph& _myGraph);
    void draw();
    void drawHomeScreen();
    void drawGraphScreen();
    void drawSettingsScreen();
    void turnOnBacklight();
    void turnOffBacklight();
    bool checkForButtonClicks();
    void drawButtons();
    void clearScreen();
    void blackScreen();
    void turnOffBacklightAfterSomeTime();
};

#endif
