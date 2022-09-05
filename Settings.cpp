#include "Settings.h"
#include "Adafruit_TSC2007.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Button.h"
#include "ESP32Time.h"
#include <Adafruit_GPS.h>

Settings::Settings(): powerModeButton(): syncTimeWithGPSButton() {}

void Settings::init(Adafruit_ILI9341& tft, Adafruit_GPS& g, ESP32Time& r) {
  powerModeButton.initButton(tft, 200, 40, 80, 40, ILI9341_WHITE, ILI9341_BLACK, ILI9341_WHITE, "Power Mode", 2);
  syncTimeWithGPSButton.initButton(tft, 200, 80, 80, 40, ILI9341_WHITE, ILI9341_BLACK, ILI9341_WHITE, "Sync Time", 2);
  wereButtonsDrawn = false;
  powerModeHigh = true;
  gps = g;
  rtc = r;
}

void Settings::draw(Adafruit_ILI9341 &tft) {
  if (!wereButtonsDrawn) {
    powerModeButton.drawButton(tft);
    syncTimeWithGPSButton.drawButton(tft);
  }
  checkForButtonClicks();
}

void Settings::setHighPowerMode() {
  //for high power mode, can update everything as fast as possible
}

void Settings::setLowPowerMode() {
  //what should we do for low power mode? maybe update once every 30 seconds or once every minute
}

void Settings::syncTimeWithGPSTime() {
  if(gps.fix){
    //setTime(int sc, int mn, int hr, int dy, int mt, int yr, int ms = 0);
    rtc.setTime(gps.seconds, gps.minute, gps.hour, gps.day, gps.month, gps.year);//update time values from GPS module    
  }
}

void Settings::setTimeManually() {}

void Settings::manuallyLogData() {}

void Settings::autoLogDataAtInterval(int interval) {} //this should probably be on the graph screen

void Settings::turnWifiOff() {}

void Settings::turnWifiOn() {}

void Settings::turnBluetoothOff() {}

void Settings::turnBluetoothOn() {}

void Settings::viewUpTime() {} //not sure if i need this

void Settings::checkForButtonClicks(int x, int y) {
  if (powerModeButton.contains(x, y)) {
    if (powerModeHigh) {
      setLowPowerMode();
      powerModeHigh = false;
    }
    else {
      setHighPowerMode();
      powerModeHigh = true;
    }
    return;
  }
  if (syncTimeWithGPSButton.contains(x, y)) {
    syncTimeWithGPSTime();
    return;
  }
}
