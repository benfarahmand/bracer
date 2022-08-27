#include "Sensors.h"
#include <SensirionI2CScd4x.h>
#include <Adafruit_GPS.h>
#include <Wire.h>


// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

//Sensors::Sensors() {}

Sensors::Sensors(): GPS(&Wire), scd4x() {};
//Sensors():scd4x(){};


// Connect to the GPS on the hardware I2C port
//Adafruit_GPS Sensors::GPS(&Wire);

//uint32_t timer = millis();
void Sensors::init() {
  isDataReady = false;
  co2 = 0;
  temperature = 0.0f;
  humidity = 0.0f;
  timer = millis();
  initGPS();
  initSCD41();
}

void Sensors::initGPS() {
  if (GPS.standby()) {
    GPS.wakeup();
  }
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(0x10);  // The I2C address to use is 0x10
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  //  GPS.sendCommand(PGCMD_ANTENNA);

  //  delay(1000);

  // Ask for firmware version
  //  GPS.println(PMTK_Q_RELEASE);
}

void Sensors::readGPS() {
  char c = GPS.read();
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())) { // this also sets the newNMEAreceived() flag to false
      return;
    }
  }

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
    //    Serial.print("\nTime: ");
    //    if (GPS.hour < 10) { Serial.print('0'); }
    //    Serial.print(GPS.hour, DEC); Serial.print(':');
    //    if (GPS.minute < 10) { Serial.print('0'); }
    //    Serial.print(GPS.minute, DEC); Serial.print(':');
    //    if (GPS.seconds < 10) { Serial.print('0'); }
    //    Serial.print(GPS.seconds, DEC); Serial.print('.');
    //    if (GPS.milliseconds < 10) {
    //      Serial.print("00");
    //    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
    //      Serial.print("0");
    //    }
    //    Serial.println(GPS.milliseconds);
    //    Serial.print("Date: ");
    //    Serial.print(GPS.day, DEC); Serial.print('/');
    //    Serial.print(GPS.month, DEC); Serial.print("/20");
    //    Serial.println(GPS.year, DEC);
    //    float mph = GPS.speed * 1.15078; //converting from knots to MPH
    //    tft.print("Fix: "); tft.print((int)GPS.fix);
    //    tft.print(" quality: "); tft.print((int)GPS.fixquality); tft.println("    ");
    //    //    if (GPS.fix) {
    //    tft.print("Lat: ");
    //    tft.print(GPS.latitude, 4); tft.print(GPS.lat); tft.println("   ");
    //    tft.print("Lon: ");
    //    tft.print(GPS.longitude, 4); tft.print(GPS.lon); tft.println("   ");
    //    tft.print("MPH: "); tft.print(mph); tft.println("   ");
    //    tft.print("Ang: "); tft.print(GPS.angle); tft.println("   ");
    //    tft.print("Alt: "); tft.print(GPS.altitude); tft.println("   ");
    //    tft.print("Sats: "); tft.print((int)GPS.satellites); tft.println("   ");
    //    }
  }
}

void Sensors::initSCD41() {
  //  uint16_t error;
  //  char errorMessage[256];

  scd4x.begin(Wire);

  // stop potentially previously started measurement
  scd4x.stopPeriodicMeasurement();
  //  error = scd4x.stopPeriodicMeasurement();
  //  if (error) {
  //    Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
  //    errorToString(error, errorMessage, 256);
  //    Serial.println(errorMessage);
  //  }rintSerialNumber(serial0, serial1, serial2);
  //  }

  // Start Measurement
  scd4x.startPeriodicMeasurement();
  //  error = scd4x.startPeriodicMeasurement();
  //  if (error) {
  //    Serial.print("Error trying to execute startPeriodicMeasurement(): ");
  //    errorToString(error, errorMessage, 256);
  //    Serial.println(errorMessage);
  //  }

  //  Serial.println("Waiting for first measurement... (5 sec)");
}

//currently turning off GPS doesn't allow it to come back online
void Sensors::turnOffSensors() {
  scd4x.stopPeriodicMeasurement(); // stop co2, temp, and humidity sensor. save some energy
  GPS.standby(); //put GPS on standby. save some energy.

}

uint16_t Sensors::getCO2(){
  return co2;
}

float Sensors::getTemp(){
  return temperature;
}

float Sensors::getHumidity(){
  return humidity;
}

void Sensors::readSCD41() {
  uint16_t error;
  char errorMessage[256];

  delay(100);

  // Read Measurement
//  uint16_t co2 = 0;
//  float temperature = 0.0f;
//  float humidity = 0.0f;
  isDataReady = false;
  error = scd4x.getDataReadyFlag(isDataReady);
  if (error) {
//    Serial.print("Error trying to execute readMeasurement(): ");
//    errorToString(error, errorMessage, 256);
//    Serial.println(errorMessage);
    return;
  }
  if (!isDataReady) {
    return;
  }
//  scd4x.readMeasurement(co2, temperature, humidity);
  error = scd4x.readMeasurement(co2, temperature, humidity);
  if (error) {
//    Serial.print("Error trying to execute readMeasurement(): ");
//    errorToString(error, errorMessage, 256);
//    Serial.println(errorMessage);
  } else if (co2 == 0) {
//    Serial.println("Invalid sample detected, skipping.");
  } else {
    temperature = temperature * 9.0f / 5.0f + 32.0f;
    //    Serial.print("Co2:");
    //    Serial.print(co2);
    //    Serial.print("\t");
    //    Serial.print("Temperature:");
    //    Serial.print(temperature);
    //    Serial.print("\t");
    //    Serial.print("Humidity:");
    //    Serial.println(humidity);
    //    tft.fillScreen(ILI9341_BLACK);
    //    tft.print("Co2:");
    //    tft.print(co2);
    //    tft.println("   ");
    //    tft.print("Temp:");
    //    tft.print(temperature);
    //    tft.println("   ");
    //    tft.print("Hum:");
    //    tft.print(humidity);
    //    tft.println("   ");
    //    tft.drawString("CO2: ", 5, 10, 2);
    //    tft.drawNumber(co2, 10 + tft.textWidth("CO2: "), 10, 2);
  }
}
