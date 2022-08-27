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
//    char settingsLabel[9] = "Settings";
    bool wereButtonsDrawn = false;
    bool backLightOn = true;
    int textSize=2;
  public:
    Display();
    void init();
    void drawHomeScreen(ESP32Time rtc, Sensors s, String batteryDetails);
    void drawSettingsScreen();
    void turnOnBacklight();
    void turnOffBacklight();
    void checkForButtonClicks();
    void changeScreen();
    void drawButtons();
};

#endif
