#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <Wire.h>
#include <SensirionI2CScd4x.h>
#include <Adafruit_GPS.h>
#include <Adafruit_BME680.h>

class Sensors {
  private:
    bool isDataReady;
    uint16_t co2;
    float temperature;
    float humidity;
    float voc;
    // int vocPin = 36;
    int lowPowerTimeDelay;
    bool isLowPower;
    //variables for calculating indoor air quality
    float hum_weighting = 0.25; // so hum effect is 25% of the total air quality score
    float gas_weighting = 0.75; // so gas effect is 75% of the total air quality score

    int   humidity_score, gas_score;
    float gas_reference = 2500;
    float hum_reference = 40;
    int   getgasreference_count = 0;
    int   gas_lower_limit = 10000;  // Bad air quality limit
    int   gas_upper_limit = 300000; // Good air quality limit
  public:
    Adafruit_GPS GPS;
    SensirionI2CScd4x scd4x;
    Adafruit_BME680 bme; // I2C
    //    uint32_t timer;
    Sensors();

    //general functions
    void init();
    void highPowerMode();
    void lowPowerMode();
    void turnOffSensors(); //doesn't work
    void turnOnSensors(); //doesn't work

    //Atmospheric Sensors
    void initSCD41();
    void initBME();
    void readSCD41();
    void readBME();
    void getGasReference();
    String calculateIAQ(int score);
    uint16_t getCO2();
    float getTemp();
    float getHumidity();
    float getPressure();
    float getVOC();
    void SCD41LowPowerMode();
    void SCD41HighPowerMode();

    //GPS Sensor
    void initGPS();
    void readGPS();
    String getGPSFix();
    bool getGPSFixBool();
    void getGPSTime();
    double getLatitude();
    double getLongitude();
    void syncTimeWithGPSTime();
    void GPSWakeUp();
    void GPSstandBy();
    void GPSHighPowerMode();
    void GPSLowPowerMode();


    //Heart Beat Sensor
    //To Do
};

#endif
