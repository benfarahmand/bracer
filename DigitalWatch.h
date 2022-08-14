#ifndef DIGITALWATCH_h
#define DIGITALWATCH_h

#include <Arduino.h>
#include <TFT_eSPI.h>

class DigitalWatch {
  public:
    DigitalWatch();
    uint32_t targetTime;
    uint8_t hh, mm, ss;

    byte omm, oss;
    byte xcolon, xsecs;

    void init();
    uint8_t conv2d(const char* p);
    void draw(TFT_eSPI tft);
};

#endif
