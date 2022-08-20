#include <Arduino.h>
#include <SensirionI2CScd4x.h>
#include <Wire.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
//#include "DigitalWatch.h"
#include <WiFi.h>
#include "ESP32Time.h"
#include <Adafruit_GPS.h>

#define TFT_MISO 19
#define TFT_MOSI 18
#define TFT_CLK 5
#define TFT_CS   16  // Chip select control pin
#define TFT_DC   17  // Data Command control pin
#define TFT_RST  21 // Reset pin (could connect to RST pin)

//wifi stuff
const char* ssid       = "";
const char* password   = "";
//in case we need to get the time from ntp
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 0;

//variables for battery readings
const int MAX_ANALOG_VAL = 4095; //for battery measurement
const float MAX_BATTERY_VOLTAGE = 4.2; // Max LiPoly voltage of a 3.7 battery is 4.2

//variables for deep sleep modes
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */
RTC_DATA_ATTR int bootCount = 0;

//co2 sensor code
SensirionI2CScd4x scd4x;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
ESP32Time rtc(-4*3600); 

void printUint16Hex(uint16_t value) {
  Serial.print(value < 4096 ? "0" : "");
  Serial.print(value < 256 ? "0" : "");
  Serial.print(value < 16 ? "0" : "");
  Serial.print(value, HEX);
}

void printSerialNumber(uint16_t serial0, uint16_t serial1, uint16_t serial2) {
  Serial.print("Serial: 0x");
  printUint16Hex(serial0);
  printUint16Hex(serial1);
  printUint16Hex(serial2);
  Serial.println();
}

Adafruit_ILI9341 printLocalTime(Adafruit_ILI9341 tft)
{
//  struct tm timeinfo;
//  getLocalTime(&timeinfo);
//  if(!getLocalTime(&timeinfo)){
//    tft.println("Failed to obtain time");
//    delay(500);
//    connectAndGetLocalTime(tft);
//    return tft;
//  }
  tft.println(rtc.getDate());
  tft.println(rtc.getTime());
  return tft;
}

Adafruit_ILI9341 connectAndGetLocalTime(Adafruit_ILI9341 tft)
{
  tft.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      tft.print(".");
  }
  tft.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)){
    rtc.setTimeStruct(timeinfo); 
  }

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  tft.fillScreen(ILI9341_BLACK);
  return tft;
}

// Connect to the GPS on the hardware I2C port
Adafruit_GPS GPS(&Wire);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

uint32_t timer = millis();

void GPSSetup(){
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

Adafruit_ILI9341 GPSParse(Adafruit_ILI9341 tft){
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
//    Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return tft; // we can fail to parse a sentence in which case we should just wait for another
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
    tft.print("Fix: "); tft.print((int)GPS.fix);
    tft.print(" quality: "); tft.print((int)GPS.fixquality); tft.println("    ");
    if (GPS.fix) {
      tft.print("Location: ");
      tft.print(GPS.latitude, 4); tft.print(GPS.lat);
      tft.print(", ");
      tft.print(GPS.longitude, 4); tft.println(GPS.lon);
      tft.print("Speed (knots): "); tft.println(GPS.speed);
      tft.print("Angle: "); tft.println(GPS.angle);
      tft.print("Altitude: "); tft.println(GPS.altitude);
      tft.print("Satellites: "); tft.println((int)GPS.satellites);
    } 
  }
  return tft;
}

void setup() {
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE,ILI9341_BLACK);  
  tft.setTextSize(2.5);
  
  Serial.begin(115200);
  rtc.setTime(30, 24, 15, 17, 1, 2021);
  
  while (!Serial) {
    delay(100);
  }

  Wire.begin();
  uint16_t error;
  char errorMessage[256];

  scd4x.begin(Wire);

  // stop potentially previously started measurement
  error = scd4x.stopPeriodicMeasurement();
  if (error) {
    Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  }

  uint16_t serial0;
  uint16_t serial1;
  uint16_t serial2;
  error = scd4x.getSerialNumber(serial0, serial1, serial2);
  if (error) {
    Serial.print("Error trying to execute getSerialNumber(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  } else {
    printSerialNumber(serial0, serial1, serial2);
  }

  // Start Measurement
  error = scd4x.startPeriodicMeasurement();
  if (error) {
    Serial.print("Error trying to execute startPeriodicMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  }

  Serial.println("Waiting for first measurement... (5 sec)");

  

//  dw.init();

 
  //connect to WiFi
  tft = connectAndGetLocalTime(tft);

  GPSSetup();
  
  tft.fillScreen(ILI9341_BLACK);
}

void loop() {
  tft.setCursor(0, 0);
  tft = printLocalTime(tft);
  // put your main code here, to run repeatedly:
  uint16_t error;
  char errorMessage[256];

  delay(100);

  // Read Measurement
  uint16_t co2 = 0;
  float temperature = 0.0f;
  float humidity = 0.0f;
  bool isDataReady = false;
  error = scd4x.getDataReadyFlag(isDataReady);
  if (error) {
    Serial.print("Error trying to execute readMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
    return;
  }
  if (!isDataReady) {
    return;
  }
  error = scd4x.readMeasurement(co2, temperature, humidity);
  if (error) {
    Serial.print("Error trying to execute readMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  } else if (co2 == 0) {
    Serial.println("Invalid sample detected, skipping.");
  } else {
//    Serial.print("Co2:");
//    Serial.print(co2);
//    Serial.print("\t");
//    Serial.print("Temperature:");
//    Serial.print(temperature);
//    Serial.print("\t");
//    Serial.print("Humidity:");
//    Serial.println(humidity);
//    tft.fillScreen(ILI9341_BLACK);
    tft.print("Co2:");
    tft.println(co2);
    tft.print("Temperature:");
    tft.println(temperature);
    tft.print("Humidity:");
    tft.println(humidity);
//    tft.drawString("CO2: ", 5, 10, 2);
//    tft.drawNumber(co2, 10 + tft.textWidth("CO2: "), 10, 2);
  }

//  dw.draw(tft);
  tft = GPSParse(tft);

   // A13 pin is not exposed on Huzzah32 board because it's tied to
  // measuring voltage level of battery. Note: you must
  // multiply the analogRead value by 2x to get the true battery
  // level. See: 
  // https://learn.adafruit.com/adafruit-huzzah32-esp32-feather/esp32-faq
  int rawValue = analogRead(A13);

  // Reference voltage on ESP32 is 1.1V
  // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html#adc-calibration
  // See also: https://bit.ly/2zFzfMT
  float voltageLevel = (rawValue / 4095.0) * 2 * 1.1 * 3.3; // calculate voltage level
  float batteryFraction = voltageLevel / MAX_BATTERY_VOLTAGE;
  
  tft.println((String)"Bat_V:" + voltageLevel + " : " + (batteryFraction * 100) + "%  ");
  delay(500);
}
