#ifndef SCD41_h
#define SCD41_h

#include <Arduino.h>
#include <SensirionI2CScd4x.h>
#include <Wire.h>
#include <TFT_eSPI.h>

class SCD41 {
  public:
    SCD41();

    void init(TwoWire Wire);
    void draw(TFT_eSPI tft);
    void measure();
  private:
    SensirionI2CScd4x scd4x;
    uint16_t co2;
    float temperature;
    float humidity;
};

#endif
