#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

class Button {
  private:
    int16_t x;
    int16_t y;
    uint16_t w;
    uint16_t h;
    uint16_t outline;
    uint16_t fill;
    uint16_t textcolor;
    String text;
    uint8_t textsize;
    uint16_t tWidth;
    uint16_t tHeight;
  public:
    Button();
    void initButton(Adafruit_ILI9341 &tft, int16_t _x, int16_t _y, uint16_t _w, uint16_t _h, uint16_t _outline, uint16_t _fill, uint16_t _textcolor, String _text, uint8_t _textsize);
    void drawButton(Adafruit_ILI9341 &tft);
    bool contains(int16_t _x, int16_t _y);

};

#endif
