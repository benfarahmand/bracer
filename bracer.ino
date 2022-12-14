#include <Arduino.h>
#include "SPI.h"
#include <Wire.h>
#include "ESP32Time.h"
#include "Display.h"
#include "Sensors.h"
#include "Wireless.h"
#include "Settings.h"

//for tracking how many times the ESP32 has woken up
RTC_DATA_ATTR int bootCount = 0;

//variables for battery readings
const int MAX_ANALOG_VAL = 4095; //for battery measurement
const float MAX_BATTERY_VOLTAGE = 4.2; // Max LiPoly voltage of a 3.7 battery is 4.2

//variables for sleep modes
//#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
//#define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */
//RTC_DATA_ATTR int bootCount = 0;
#define BUTTON_PIN_BITMASK 0x8000000000 // 2^39 in hex, GPI 39

ESP32Time rtc(4 * 3600);
Sensors mySensors;
Display myDisplay;
Settings mySettings;
Graph myGraph;
uint32_t timer;
int timeToUpdate = 1000; //update screen & sensors every 1 second
unsigned long startTime;


void setup() {
  Serial.begin(115200);
  Wire.begin();
  while (!Serial) {
    delay(100);
  }
  rtc.setTime(30, 24, 15, 21, 8, 2022);//temporary values. will update it later via GPS module
  startTime = rtc.getEpoch();
  mySensors.init();
  myDisplay.init();
  mySettings.init(myDisplay.tft, mySensors, rtc);
  myGraph.init(startTime);
  timer = millis();
  if(bootCount > 0){
    
  }
  Serial.println("setup");
  bootCount = bootCount + 1;
}


//To Do: draw performance, potentially save battery life in the process:
//we could improve loop performance by only reading sensors 
//and then checking if there's anything new to draw to screen before updating the screen
//will constantly need to check for button clicks
void loop() { 
  if (millis() - timer > timeToUpdate) {
    timer = millis();
    //read data
    mySensors.readSCD41();
    mySensors.readGPS();
    mySensors.readBME();

    if(myDisplay.backLightOn) {
      myDisplay.draw(rtc, mySensors, myGraph, mySettings, getBatteryString(), (String) mySettings.getUpTime(), mySensors.getGPSFix());
    }
    //if more than some seconds have passed, log the data
    //unsigned long upt, unsigned long epochT, double carbon_dioxide, double hum, double temp, double lati, double longi, double bat
    //before setting the data, we might need to wait for the GPS to have a fix on the latitude and longitude, or do we not worry about the gps fix?
    myGraph.setData(mySettings.getUpTime(), rtc.getEpoch(), mySensors.getCO2(), mySensors.getHumidity(), mySensors.getTemp(), mySensors.getLatitude(), mySensors.getLongitude(), getBatteryVoltage());
  }

  // delay(50);
  //check for button clicks
  if(myDisplay.checkForButtonClicks(mySettings, myGraph)){
    myDisplay.draw(rtc, mySensors, myGraph, mySettings, getBatteryString(), (String) mySettings.getUpTime(), mySensors.getGPSFix());
  };
  myDisplay.turnOffBacklightAfterSomeTime();
  //check if analog button press... what should we do with the analog press? maybe go to light sleep
  if(analogRead(A3)){
    lightSleep(myDisplay.tft);
  }
}

float getBatteryVoltage(){
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
  return voltageLevel;
}

String getBatteryString() {
  float voltageLevel = getBatteryVoltage();
  float batteryFraction = voltageLevel / MAX_BATTERY_VOLTAGE;

  return (String)"Bat:" + voltageLevel + ":" + (batteryFraction * 100) + "%   ";
}




void deepSleep(Adafruit_ILI9341 tft) {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);

  //put various sensors to sleep
  // tft.println("Shifting sensors to low power.");
  //  turnOffI2CSensors();

  // need to check if Bluetooth and Wifi is on and turn them off

  //  esp_sleep_enable_ext0_wakeup(GPIO_NUM_32, 1); //1 = High, 0 = Low

  //If you were to use ext1, you would use it like
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);
  tft.println("Going to deep sleep now");
  delay(1000);
  myDisplay.turnOffBacklight();
  esp_deep_sleep_start(); //loses all memory of things
}

void lightSleep(Adafruit_ILI9341 tft) {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);

  //put various sensors to sleep
  // tft.println("Shifting sensors to low power.");
  //  turnOffI2CSensors();
  
  // need to check if Bluetooth and Wifi is on and turn them off

  //  esp_sleep_enable_ext0_wakeup(GPIO_NUM_32, 1); //1 = High, 0 = Low

  //If you were to use ext1, you would use it like
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);
  tft.println("Going to light sleep now");
  delay(1000);
  myDisplay.turnOffBacklight();
  esp_light_sleep_start(); //stores memory
}
