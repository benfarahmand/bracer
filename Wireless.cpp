#include "Wireless.h"
#include "Wifi.h"

Wireless::Wireless(){}

//wifi stuff
const char* ssid       = "";
const char* password   = "";
//in case we need to get the time from ntp. for now we will get the time from GPS module.
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 0;

