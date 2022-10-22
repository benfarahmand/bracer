#include "Sensors.h"
#include <SensirionI2CScd4x.h>
#include <Adafruit_GPS.h>
#include <Wire.h>
#include <Adafruit_BME680.h>

//scd41 code: https://github.com/Sensirion/arduino-i2c-scd4x/blob/master/src/SensirionI2CScd4x.cpp
//GPS code: https://github.com/adafruit/Adafruit_GPS/blob/master/src/Adafruit_GPS.cpp

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

#define SEALEVELPRESSURE_HPA (1013.25)

//Sensors::Sensors() {}

Sensors::Sensors(): GPS(&Wire), scd4x(), bme() {};
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
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
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
void Sensors::GPSLowPowerMode(){
  GPS.sendCommand("$PMTK225,2,1000,10000,24000,90000*16");// 0.1HZ?
}

void Sensors::GPSHighPowerMode(){
  GPS.sendCommand("$PMTK225,0*2B");
}

void Sensors::GPSstandBy(){
  GPS.sendCommand("$PMTK161,0*28");
}

void Sensors::GPSWakeUp(){
  //to wake up, GPS will go into high power mode... 
  //in the future we can remember the last state we were in and then wake up back to that state
  GPS.sendCommand("$PMTK225,0*2B");
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
  GPSHighPowerMode();
}

void Sensors::lowPowerMode(){
  //collect measurements once in a while, maybe once every minute
  isLowPower = true;
  GPSLowPowerMode();
}

void Sensors::SCD41LowPowerMode(){
  
}

void Sensors::SCD41HighPowerMode(){
  
}

//currently turning off GPS doesn't allow it to come back online
void Sensors::turnOffSensors() {
  scd4x.stopPeriodicMeasurement(); // stop co2, temp, and humidity sensor. save some energy
  scd4x.powerDown(); //i wonder if this works
  GPSstandBy();
//  GPS.standby(); //put GPS on standby. save some energy. this causes some issue...
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

//https://github.com/G6EJD/BME680-Example/blob/master/ESP32_bme680_CC_demo_03.ino
//datasheet: https://cdn-shop.adafruit.com/product-files/3660/BME680.pdf
void Sensors::initBME(){
  if (!bme.begin()) {
    // Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }
  
  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  // getGasReference();
}

float Sensors::getVOC(){
  return voc;
}

float Sensors::getPressure(){
  return bme.pressure / 100.0;
}

//https://github.com/DFRobot/DFRobot_MICS/blob/master/DFRobot_MICS.cpp
//datasheet: https://www.mouser.com/datasheet/2/18/1084_Datasheet-MiCS-5524-rev-8-1144838.pdf
//need to figure out how to convert from voltage reading to ppm
void Sensors::readBME(){
  if (! bme.performReading()) {
    // Serial.println("Failed to perform reading :(");
    return;
  }

  //calculate humidity score
  float current_humidity = bme.humidity;
  if (current_humidity >= 38 && current_humidity <= 42) // Humidity +/-5% around optimum
    humidity_score = 0.25 * 100;
  else
  { // Humidity is sub-optimal
    if (current_humidity < 38)
      humidity_score = 0.25 / hum_reference * current_humidity * 100;
    else
    {
      humidity_score = ((-0.25 / (100 - hum_reference) * current_humidity) + 0.416666) * 100;
    }
  }
  
  //calculate gas score
  // gas_score = (0.75 / (gas_upper_limit - gas_lower_limit) * gas_reference - (gas_lower_limit * (0.75 / (gas_upper_limit - gas_lower_limit)))) * 100.00;
  gas_score = (0.75 / (gas_upper_limit - gas_lower_limit) * bme.gas_resistance - (gas_lower_limit * (0.75 / (gas_upper_limit - gas_lower_limit)))) * 100.00;
  if (gas_score > 75) gas_score = 75; // Sometimes gas readings can go outside of expected scale maximum
  if (gas_score <  0) gas_score = 0;  // Sometimes gas readings can go outside of expected scale minimum

  //Combine results for the final IAQ index value (0-100% where 100% is good quality air)
  float air_quality_score = humidity_score + gas_score;
  Serial.println(" comprised of " + String(humidity_score) + "% Humidity and " + String(gas_score) + "% Gas");
  // if ((getgasreference_count++) % 5 == 0) getGasReference();
  Serial.println(CalculateIAQ(air_quality_score));
  
  voc = air_quality_score;
  // voc = bme.gas_resistance / 1000.0;
  
  // float rawADC = static_cast<float>(getRawVocAdc());
  // float sensorResistance = ((4095.0 - rawADC) / rawADC);

  // float r0_clean_air = 100; //value from the datasheet
  // // float r0_max = 1500;

  // float rs_r0 = log10( sensorResistance / r0_clean_air);

  // //PPM: x = 10 ^ ((y - b) / m)
  
  // float slope = -0.85;
  // float b = 0.54;
  // float ppm = pow(10,((rs_r0-b)/slope));
  
  // // Serial.print("PPM: ");
  // // Serial.println(ppm);  
  // voc = ppm;
}

String Sensors::calculateIAQ(int score) {
  String IAQ_text = "air quality is ";
  score = (100 - score) * 5;
  if      (score >= 301)                  IAQ_text += "Hazardous";
  else if (score >= 201 && score <= 300 ) IAQ_text += "Very Unhealthy";
  else if (score >= 176 && score <= 200 ) IAQ_text += "Unhealthy";
  else if (score >= 151 && score <= 175 ) IAQ_text += "Unhealthy for Sensitive Groups";
  else if (score >=  51 && score <= 150 ) IAQ_text += "Moderate";
  else if (score >=  00 && score <=  50 ) IAQ_text += "Good";
  Serial.print("IAQ Score = " + String(score) + ", ");
  return IAQ_text;
}

// void Sensors::getGasReference() {
//   // Now run the sensor for a burn-in period, then use combination of relative humidity and gas resistance to estimate indoor air quality as a percentage.
//   int readings = 10;
//   for (int i = 1; i <= readings; i++) { // read gas for 10 x 0.150mS = 1.5secs
//     gas_reference += bme.readGas();
//   }
//   gas_reference = gas_reference / readings;
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
