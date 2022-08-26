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
  public:
    uint32_t timer;
    Sensors();
//    Sensors():GPS(&Wire),scd4x(){}
    void init();
    void initSCD41();
    void readSCD41();
    void initGPS();
    void readGPS();
    void turnOffSensors();
};

#endif
