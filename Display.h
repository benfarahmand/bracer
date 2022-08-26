#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "Adafruit_ILI9341.h"

class Display {
  private:
    Adafruit_ILI9341 tft;
  public:
    
    Display();
    Adafruit_ILI9341 init();
    Adafruit_ILI9341 homeScreen(Adafruit_ILI9341 tft);
    Adafruit_ILI9341 settingsScreen(Adafruit_ILI9341 tft);
};

#endif
