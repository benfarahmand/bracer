#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <Wire.h>
#include <SensirionI2CScd4x.h>
#include <Adafruit_GPS.h>

class Sensors {
  private:
    bool isDataReady;
    uint16_t co2;
    float temperature;
    float humidity;
    float voc;
    int vocPin = 36;
    int lowPowerTimeDelay;
    bool isLowPower;
  public:
    Adafruit_GPS GPS;
    SensirionI2CScd4x scd4x;
    //    uint32_t timer;
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
    void readVOC();
    //add vOC sensor
    uint16_t getCO2();
    float getTemp();
    float getHumidity();
    float getVOC();
    int getRawVocAdc();

    //GPS Sensor
    void initGPS();
    void readGPS();
    String getGPSFix();
    bool getGPSFixBool();
    void getGPSTime();
    double getLatitude();
    double getLongitude();
    void syncTimeWithGPSTime();


    //Heart Beat Sensor
    //To Do
};

#endif
