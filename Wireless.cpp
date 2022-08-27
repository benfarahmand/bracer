#include "Wireless.h"
#include "WiFi.h"

Wireless::Wireless() {
  ssid       = "";
  password   = "";
  //in case we need to get the time from ntp. for now we will get the time from GPS module.
  ntpServer = "pool.ntp.org";
  gmtOffset_sec = 0;
  daylightOffset_sec = 0;
}

////wifi stuff
//const char* ssid       = "";
//const char* password   = "";
////in case we need to get the time from ntp. for now we will get the time from GPS module.
//const char* ntpServer = "pool.ntp.org";
//const long  gmtOffset_sec = 0;
//const int   daylightOffset_sec = 0;

void Wireless::BTConnect() {

}

void Wireless::WifiConnect()
{
  //  tft.printf("Connecting to %s ", ssid);
  //  WiFi.begin(ssid, password);
  //  while (WiFi.status() != WL_CONNECTED) {
  //    delay(500);
  //    tft.print(".");
  //  }
  //  tft.println(" CONNECTED");

  //init and get the time
  //  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  //  struct tm timeinfo;
  //  if (getLocalTime(&timeinfo)) {
  //    rtc.setTimeStruct(timeinfo);
  //  }

  //disconnect WiFi as it's no longer needed
  //  WiFi.disconnect(true);
  //  WiFi.mode(WIFI_OFF);
  //  tft.fillScreen(ILI9341_BLACK);
  //  return tft;
}
