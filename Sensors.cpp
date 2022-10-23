#include "Sensors.h"
#include <SensirionI2CScd4x.h>
#include <Adafruit_GPS.h>
#include <Wire.h>
#include <bsec.h>

//scd41 code: https://github.com/Sensirion/arduino-i2c-scd4x/blob/master/src/SensirionI2CScd4x.cpp
//GPS code: https://github.com/adafruit/Adafruit_GPS/blob/master/src/Adafruit_GPS.cpp

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

#define SEALEVELPRESSURE_HPA (1013.25)

//Sensors::Sensors() {}

Sensors::Sensors()
  : GPS(&Wire), scd4x(), bme(){};
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
  initBME();
}

//Datasheet for the GPS module I'm using: https://cdn-learn.adafruit.com/assets/assets/000/084/295/original/CD_PA1010D_Datasheet_v.03.pdf?1573833002
//this datasheet has commands for how to go into low power mode
void Sensors::initGPS() {
  //  if (GPS.standby()) {
  //    GPS.wakeup();
  //  }
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(0x10);  // The I2C address to use is 0x10
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);  // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  //  GPS.sendCommand(PGCMD_ANTENNA);

  //  delay(1000);

  // Ask for firmware version
  //  GPS.println(PMTK_Q_RELEASE);
}

/* NOTES FOR LOW POWER MODE:
Enter periodic mode by sending the following command: $PMTK225,Type, Run_time, Sleep_time,2nd_Run_time,2nd_Sleep_time*checksum
Where:
- Type = 1: Periodic backup mode, Type = 2 : Periodic standby mode
- Run time: Full Power period (ms)
- Sleep time: Standby period (ms)
- 2nd Run time: Full Power period (ms) for extended acquisition if GNSS acquisition fails during Run time.
- 2nd Sleep time: Standby period (ms) for extended sleep if GNSS acquisition fails during Runtime.
Example: $PMTK225,2,4000,15000,24000,90000*16: periodic mode with 4 second navigation and 15 second sleep.
 */
void Sensors::GPSLowPowerMode() {
  GPS.sendCommand("$PMTK225,2,1000,10000,24000,90000*16");  // 0.1HZ?
}

void Sensors::GPSHighPowerMode() {
  GPS.sendCommand("$PMTK225,0*2B");
}

void Sensors::GPSstandBy() {
  GPS.sendCommand("$PMTK161,0*28");
}

void Sensors::GPSWakeUp() {
  //to wake up, GPS will go into high power mode...
  //in the future we can remember the last state we were in and then wake up back to that state
  GPS.sendCommand("$PMTK225,0*2B");
}

bool Sensors::getGPSFixBool() {
  if ((int)GPS.fix == 0) {
    return false;
  } else {
    return true;
  }
}

String Sensors::getGPSFix() {
  if ((int)GPS.fix == 0) {
    return "No";
  } else {
    return "Yes";
  }
}

void Sensors::readGPS() {
  char c = GPS.read();
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())) {  // this also sets the newNMEAreceived() flag to false
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

void Sensors::getGPSTime() {
}

double Sensors::getLatitude() {
  return GPS.latitude_fixed / 10000000.0;
}

double Sensors::getLongitude() {
  return GPS.longitude_fixed / 10000000.0;
}

void Sensors::initSCD41() {
  scd4x.begin(Wire);

  // stop potentially previously started measurement
  scd4x.stopPeriodicMeasurement();

  // Start Measurement
  if (!isLowPower) {
    scd4x.startPeriodicMeasurement();
  } else {
    scd4x.startLowPowerPeriodicMeasurement();
  }
}

void Sensors::highPowerMode() {
  isLowPower = false;
  GPSHighPowerMode();
}

void Sensors::lowPowerMode() {
  //collect measurements once in a while, maybe once every minute
  isLowPower = true;
  GPSLowPowerMode();
}

void Sensors::SCD41LowPowerMode() {
}

void Sensors::SCD41HighPowerMode() {
}

//currently turning off GPS doesn't allow it to come back online
void Sensors::turnOffSensors() {
  scd4x.stopPeriodicMeasurement();  // stop co2, temp, and humidity sensor. save some energy
  scd4x.powerDown();                //i wonder if this works
  GPSstandBy();
  //  GPS.standby(); //put GPS on standby. save some energy. this causes some issue...
}

void Sensors::turnOnSensors() {
  //if sensors have been turned off, then we need to turn them on before we reinit
}

uint16_t Sensors::getCO2() {
  return co2;
}

float Sensors::getTemp() {
  return temperature;
}

float Sensors::getHumidity() {
  return humidity;
}

//https://github.com/G6EJD/BME680-Example/blob/master/ESP32_bme680_CC_demo_03.ino
//datasheet: https://cdn-shop.adafruit.com/product-files/3660/BME680.pdf
void Sensors::initBME() {
  bme.begin(BME680_I2C_ADDR_SECONDARY, Wire);
  // output = "\nBSEC library version " + String(bme.version.major) + "." + String(bme.version.minor) + "." + String(bme.version.major_bugfix) + "." + String(bme.version.minor_bugfix);
  // Serial.println(output);
  checkIaqSensorStatus();

  bsec_virtual_sensor_t sensorList[10] = {
    BSEC_OUTPUT_RAW_TEMPERATURE,
    BSEC_OUTPUT_RAW_PRESSURE,
    BSEC_OUTPUT_RAW_HUMIDITY,
    BSEC_OUTPUT_RAW_GAS,
    BSEC_OUTPUT_IAQ,
    BSEC_OUTPUT_STATIC_IAQ,
    BSEC_OUTPUT_CO2_EQUIVALENT,
    BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
  };

  bme.updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_LP);
  checkIaqSensorStatus();

  // Print the header
  // output = "Timestamp [ms], raw temperature [°C], pressure [hPa], raw relative humidity [%], gas [Ohm], IAQ, IAQ accuracy, temperature [°C], relative humidity [%], Static IAQ, CO2 equivalent, breath VOC equivalent";
  // Serial.println(output);
}

float Sensors::getVOC() {
  return bme.gasResistance / 1000.0;
}

float Sensors::getPressure() {
  return bme.pressure / 100.0;
}

//https://github.com/DFRobot/DFRobot_MICS/blob/master/DFRobot_MICS.cpp
//datasheet: https://www.mouser.com/datasheet/2/18/1084_Datasheet-MiCS-5524-rev-8-1144838.pdf
//need to figure out how to convert from voltage reading to ppm


void Sensors::readBME() {
  // unsigned long time_trigger = millis();
  if (bme.run()) { // If new data is available
    // output = String(time_trigger);
    // output += ", " + String(bme.rawTemperature);
    // output += ", " + String(bme.pressure);
    // output += ", " + String(bme.rawHumidity);
    // output += ", " + String(bme.gasResistance);
    // output += ", " + String(bme.iaq);
    // output += ", " + String(bme.iaqAccuracy);
    // output += ", " + String(bme.temperature);
    // output += ", " + String(bme.humidity);
    // output += ", " + String(bme.staticIaq);
    // output += ", " + String(bme.co2Equivalent);
    // output += ", " + String(bme.breathVocEquivalent);
    // Serial.println(output);
  } else {
    checkIaqSensorStatus();
  }
}

// Helper function definitions
void Sensors::checkIaqSensorStatus(void)
{
  if (bme.status != BSEC_OK) {
    if (bme.status < BSEC_OK) {
      output = "BSEC error code : " + String(bme.status);
      Serial.println(output);
      // for (;;)
      //   errLeds(); /* Halt in case of failure */
    } else {
      output = "BSEC warning code : " + String(bme.status);
      Serial.println(output);
    }
  }

  if (bme.bme680Status != BME680_OK) {
    if (bme.bme680Status < BME680_OK) {
      output = "BME680 error code : " + String(bme.bme680Status);
      Serial.println(output);
      // for (;;)
      //   errLeds(); /* Halt in case of failure */
    } else {
      output = "BME680 warning code : " + String(bme.bme680Status);
      Serial.println(output);
    }
  }
}

int Sensors::getIAQScore() {
  return bme.iaq;
}

String Sensors::getIAQString() {
  String IAQ_text = "";  //"air quality is ";
  int score = bme.iaq;
  if (score >= 301) IAQ_text += "Hazardous";
  else if (score >= 201 && score <= 300) IAQ_text += "Very Unhealthy";
  else if (score >= 176 && score <= 200) IAQ_text += "Unhealthy";
  else if (score >= 151 && score <= 175) IAQ_text += "Unhealthy 4 S/Gs";
  else if (score >= 51 && score <= 150) IAQ_text += "Moderate";
  else if (score >= 00 && score <= 50) IAQ_text += "Good";
  // Serial.print("IAQ Score = " + String(score) + ", ");
  return IAQ_text;
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