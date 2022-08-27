#ifndef WIRELESS_H
#define WIRELESS_H

#include <Arduino.h>

class Wireless {
  public:
    //wifi stuff
    String ssid       = "";
    String password   = "";
    //in case we need to get the time from ntp. for now we will get the time from GPS module.
    String ntpServer = "pool.ntp.org";
    long  gmtOffset_sec = 0;
    int   daylightOffset_sec = 0;
    Wireless();
    void WifiConnect();
    void BTConnect();

};

#endif
