#include "Sensors.h"
#include <SensirionI2CScd4x.h>
#include <Adafruit_GPS.h>
#include <Wire.h>

//scd41 code: https://github.com/Sensirion/arduino-i2c-scd4x/blob/master/src/SensirionI2CScd4x.cpp
//GPS code: https://github.com/adafruit/Adafruit_GPS/blob/master/src/Adafruit_GPS.cpp

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
  isLowPower = false;
  co2 = 0;
  temperature = 0.0f;
  humidity = 0.0f;
//  timer = 0;
//  timer = millis();
  initGPS();
  initSCD41();
}

//Datasheet for the GPS module I'm using: https://cdn-learn.adafruit.com/assets/assets/000/084/295/original/CD_PA1010D_Datasheet_v.03.pdf?1573833002
//this datasheet has commands for how to go into low power mode

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

bool Sensors::getGPSFixBool(){
  if((int)GPS.fix==0){
    return false;
  }
  else {
    return true;
  }  
}

String Sensors::getGPSFix(){
  if((int)GPS.fix==0){
    return "No";
  }
  else {
    return "Yes";
  }
}

void Sensors::readGPS() {
  char c = GPS.read();
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())) { // this also sets the newNMEAreceived() flag to false
      return;
    }
  }

  // approximately every 2 seconds or so, print out the current stats
//  if (millis() - timer > 2000) {
//    timer = millis(); // reset the timer
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
//  }
}

void Sensors::getGPSTime(){

}

double Sensors::getLatitude(){
  return GPS.latitude_fixed/10000000.0;
}

double Sensors::getLongitude(){
  return GPS.longitude_fixed/10000000.0;
}

void Sensors::initSCD41() {
  scd4x.begin(Wire);

  // stop potentially previously started measurement
  scd4x.stopPeriodicMeasurement();

  // Start Measurement
  if(!isLowPower) {
    scd4x.startPeriodicMeasurement();
  }
  else {
    scd4x.startLowPowerPeriodicMeasurement();
  }
}

void Sensors::highPowerMode(){
  isLowPower = false;
}

void Sensors::lowPowerMode(){
  //collect measurements once in a while, maybe once every minute
  isLowPower = true;
}

//currently turning off GPS doesn't allow it to come back online
void Sensors::turnOffSensors() {
  scd4x.stopPeriodicMeasurement(); // stop co2, temp, and humidity sensor. save some energy
  scd4x.powerDown(); //i wonder if this works
  GPS.standby(); //put GPS on standby. save some energy. this causes some issue...
}

void Sensors::turnOnSensors(){
  //if sensors have been turned off, then we need to turn them on before we reinit
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

float Sensors::getVOC(){
  return voc;
}

int Sensors::getRawVocAdc(){
  int rawADC = analogRead(vocPin);
  Serial.print("raw ADC: ");
  Serial.println(rawADC);
  return rawADC;
}

//https://github.com/DFRobot/DFRobot_MICS/blob/master/DFRobot_MICS.cpp
//datasheet: https://www.mouser.com/datasheet/2/18/1084_Datasheet-MiCS-5524-rev-8-1144838.pdf
//need to figure out how to convert from voltage reading to ppm
void Sensors::readVOC(){
  float rawADC = static_cast<float>(getRawVocAdc());
  float sensorResistance = ((4095.0 - rawADC) / rawADC);
  // Serial.print("Resistance: ");
  // Serial.println(sensorResistance);

  float r0_clean_air = 100; //value from the datasheet
  // float r0_max = 1500;

  float rs_r0 = log10( sensorResistance / r0_clean_air);

  //PPM: x = 10 ^ ((y - b) / m)
  
  float slope = -0.85;
  float b = 0.54;
  float ppm = pow(10,((rs_r0-b)/slope));
  
  // Serial.print("PPM: ");
  // Serial.println(ppm);  
  voc = ppm;
  // voc = 0;
}

void Sensors::readSCD41() {
  uint16_t error;
  char errorMessage[256];

  // delay(100);

  isDataReady = false;
  error = scd4x.getDataReadyFlag(isDataReady);
  if (error) {
    //you can print the error if need be
    return;
  }
  if (!isDataReady) {
    return;
  }
//  scd4x.readMeasurement(co2, temperature, humidity);
  error = scd4x.readMeasurement(co2, temperature, humidity);
  if (error) {
    //you can print the error if need be
  } else if (co2 == 0) {
    //skip if no co2 values, might be an issue with the reading
  } else {
    temperature = temperature * 9.0f / 5.0f + 32.0f;
  }
}
