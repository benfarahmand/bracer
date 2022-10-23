#include "Settings.h"
#include "Adafruit_TSC2007.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Button.h"
#include "ESP32Time.h"
#include <Adafruit_GPS.h>
#include "Sensors.h"

Settings::Settings()
  : powerModeButton(), syncTimeWithGPSButton() {}

void Settings::init(Adafruit_ILI9341& tft, Sensors& s, ESP32Time& r) {
  powerModeButton.initButton(tft, 200, 40, 120, 40, ILI9341_WHITE, ILI9341_BLACK, ILI9341_WHITE, "Power Mode", 2);
  syncTimeWithGPSButton.initButton(tft, 200, 80, 120, 40, ILI9341_WHITE, ILI9341_BLACK, ILI9341_WHITE, "Sync Time", 2);
  mySensors = &s;
  rtc = &r;
  wereButtonsDrawn = false;
  powerModeHigh = true;
  startTime = r.getEpoch();
}

void Settings::draw(Adafruit_ILI9341& tft) {
  if (!wereButtonsDrawn) {
    powerModeButton.drawButton(tft);
    syncTimeWithGPSButton.drawButton(tft);
  }
}

void Settings::setHighPowerMode() {
  //for high power mode, can update everything as fast as possible
}

void Settings::setLowPowerMode() {
  //what should we do for low power mode? maybe update once every 30 seconds or once every minute
}

void Settings::syncTimeWithGPSTime() {
  // Serial.print("Fix: ");
  // Serial.println(mySensors->getGPSFixBool());
  // Serial.print("CO2: ");
  // Serial.println(mySensors->getCO2());
  if (mySensors->getGPSFixBool()) {
    //setTime(int sc, int mn, int hr, int dy, int mt, int yr, int ms = 0);
    //before syncing the time, need to adjust the start time value to compensate for the syncing of the time
    unsigned long tempUpTime = getUpTime();
    rtc->setTime(mySensors->GPS.seconds, mySensors->GPS.minute, (mySensors->GPS.hour) + 3, int(mySensors->GPS.day+'0'), int(mySensors->GPS.month+'0'), int(mySensors->GPS.year+'0'));  //update time values from GPS module
    startTime = rtc->getEpoch() - tempUpTime;
    // Serial.print("Date: ");
    // Serial.print(mySensors->GPS.month);
    // Serial.print("/");
    // Serial.print(mySensors->GPS.day);
    // Serial.print("/");
    // Serial.println(mySensors->GPS.year);
  }
}

void Settings::setTimeManually() {}

void Settings::manuallyLogData() {}

void Settings::autoLogDataAtInterval(int interval) {}  //this should probably be on the graph screen

void Settings::turnWifiOff() {}

void Settings::turnWifiOn() {}

void Settings::turnBluetoothOff() {}

void Settings::turnBluetoothOn() {}

unsigned long Settings::getUpTime() {
  unsigned long upTime = rtc->getEpoch() - startTime;
  return upTime;
}

bool Settings::checkForButtonClicks(uint16_t& x, uint16_t& y) {
  if (powerModeButton.contains(x, y)) {
    if (powerModeHigh) {
      setLowPowerMode();
      powerModeHigh = false;
    } else {
      setHighPowerMode();
      powerModeHigh = true;
    }
    return true;
  }
  if (syncTimeWithGPSButton.contains(x, y)) {
    Serial.println("sync time clicked");
    syncTimeWithGPSTime();
    return true;
  }
  return false;
}