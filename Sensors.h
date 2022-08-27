#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <Wire.h>
#include <SensirionI2CScd4x.h>
#include <Adafruit_GPS.h>

class Sensors {
  private:
    Adafruit_GPS GPS;
    SensirionI2CScd4x scd4x;
    bool isDataReady;
    uint16_t co2;
    float temperature;
    float humidity;
  public:
    uint32_t timer;
    Sensors();
    void init();
    void initSCD41();
    void readSCD41();
    void initGPS();
    void readGPS();
    void turnOffSensors();
    uint16_t getCO2();
    float getTemp();
    float getHumidity();
};

#endif
