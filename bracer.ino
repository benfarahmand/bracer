#include <Arduino.h>
#include "SPI.h"
#include <Wire.h>
#include "ESP32Time.h"
#include "Display.h"
#include "Sensors.h"
#include "Wireless.h"

//variables for battery readings
const int MAX_ANALOG_VAL = 4095; //for battery measurement
const float MAX_BATTERY_VOLTAGE = 4.2; // Max LiPoly voltage of a 3.7 battery is 4.2

//variables for deep sleep modes
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */
RTC_DATA_ATTR int bootCount = 0;

ESP32Time rtc(4 * 3600);

//void printUint16Hex(uint16_t value) {
//  Serial.print(value < 4096 ? "0" : "");
//  Serial.print(value < 256 ? "0" : "");
//  Serial.print(value < 16 ? "0" : "");
//  Serial.print(value, HEX);
//}
//
//void printSerialNumber(uint16_t serial0, uint16_t serial1, uint16_t serial2) {
//  Serial.print("Serial: 0x");
//  printUint16Hex(serial0);
//  printUint16Hex(serial1);
//  printUint16Hex(serial2);
//  Serial.println();
//}

//Adafruit_ILI9341 printLocalTime(Adafruit_ILI9341 tft)
//{
//  //  struct tm timeinfo;
//  //  getLocalTime(&timeinfo);
//  //  if(!getLocalTime(&timeinfo)){
//  //    tft.println("Failed to obtain time");
//  //    delay(500);
//  //    connectAndGetLocalTime(tft);
//  //    return tft;
//  //  }
//  tft.println(rtc.getDate());
//  tft.println(rtc.getTime());
//  return tft;
//}



//#define BUTTON_PIN_BITMASK 0x8000000000 // 2^39 in hex, GPI 39

//void deepSleep(Adafruit_ILI9341 tft) {
//  tft.fillScreen(ILI9341_BLACK);
//  tft.setCursor(0, 0);
//
//  //put various sensors to sleep
//  tft.println("Shifting sensors to low power.");
//  turnOffI2CSensors();
//
//  // need to check if Bluetooth and Wifi is on and turn them off
//  
//  //  esp_sleep_enable_ext0_wakeup(GPIO_NUM_32, 1); //1 = High, 0 = Low
//
//  //If you were to use ext1, you would use it like
//  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);
//  tft.println("Going to sleep now");
//  delay(1000);
//  tft.fillScreen(ILI9341_BLACK);
//  //  esp_deep_sleep_start(); //loses all memory of things
//  esp_light_sleep_start(); //stores memory
//}

void setup() {
  

  Serial.begin(115200);
  rtc.setTime(30, 24, 15, 21, 8, 2022);

  while (!Serial) {
    delay(100);
  }

  Wire.begin();
  


}

void loop() {
//  tft.setCursor(0, 0);
//  tft = printLocalTime(tft);
  



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

//  tft.println((String)"Bat_V:" + voltageLevel + " : " + (batteryFraction * 100) + "%   ");
//  delay(500);
//  uint16_t x, y, z1, z2;
//  if (touch.read_touch(&x, &y, &z1, &z2) && (z1 > 100)) {
//    deepSleep(tft);
//  }
}
