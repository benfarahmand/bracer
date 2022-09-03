#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>

//settings options: high power mode and low power mode
  //1. high power mode can collect data frequently. 
  //2. low power mode can collect data once every 5 or so minutes. how can this work?
  //3. set display brightness. apparently, it can only be on or off. So we can't have a display brightness setting. but we could try to pass a spcific value to the TFT and see what happens.
  //4. set time, maybe a button can be pressed to grab time from the GPS module
  //5. log data, maybe this is a button instead or maybe it can have an auto logging mode too
  //6. turn on/off wifi for transfering data
  //7. turn on/off BT for notifications?
  //8. show total up time and amount of battery voltage used
  //9. Sync the time with GPS time

class Settings {
  private:
    
  public:
    
    Settings();
    void setHighPowerMode();
    void setLowPowerMode();
    void setTimeManually();
    void syncTimeWithGPSTime();
    void manuallyLogData();
    void autoLogDataAtInterval(int interval);
    void turnWifiOff();
    void turnWifiOn();
    void turnBluetoothOff();
    void turnBluetoothOn();
    void viewUpTime();

    
};

#endif
