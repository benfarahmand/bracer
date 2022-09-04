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
    int lowPowerTimeDelay;
    bool isLowPower;
  public:
    uint32_t timer;
    Sensors();
    
    //general functions
    void init();
    void highPowerMode();
    void lowPowerMode();
    void turnOffSensors();
    void turnOnSensors();
    
    //Atmospheric Sensors
    void initSCD41();
    void readSCD41();
    //add vOC sensor
    uint16_t getCO2();
    float getTemp();
    float getHumidity();

    //GPS Sensor
    void initGPS();
    void readGPS();
    void getGPSTime();
    double getLatitude();
    double getLongitude();
    void syncTimeWithGPSTime();
    

    //Heart Beat Sensor
    //To Do
};

#endif
