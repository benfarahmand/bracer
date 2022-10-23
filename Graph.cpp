#include "Graph.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_GFX.h"
#include "Sensors.h"

Graph::Graph() {}

void Graph::init(Adafruit_ILI9341& _tft, Sensors& s, unsigned long t) {
  startTime = t;
  tft = &_tft;
  mySensors = &s;
}

void Graph::draw() {
  //will be using the following function extensively: writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color)
  //need to find a list of colors

  //need to think about how to draw all the different graphs
  //the few potential graphs include:
  //battery over time
  //co2 over time
  //temperature over time
  //humidity over time
  //maybe co2, temp, and hum can be combined into one graph with different colored lines. need to think about how this will work...
  //maybe even have a graph that shows the last 10 minutes of GPS coordinates where  longitude = x, and latitude = y
  //if there are multiple graphs, we need a way to switch between them. maybe some buttons

  //eventually once the vOC and pulse sensors are also connected, will need to include these graphs too
  if (redraw) {
    int16_t x0 = 0;
    int16_t y0 = 220;
    int16_t graphWidth = 250;
    int16_t graphHeight = 150;
    float mult1 = graphWidth / arrayLength;
    float multCO2 = float(graphHeight) / 5000.0f;
    float multHum = float(graphHeight) / 100.0f;
    float multTemp = float(graphHeight) / 120.0f;
    float multIAQ = float(graphHeight) / 500.0f;

    tft->setTextSize(2);
    tft->setCursor(graphWidth + 2, y0 - graphHeight);
    tft->setTextColor(ILI9341_CYAN);
    tft->println("CO2");
    tft->setCursor(graphWidth + 2, y0 - graphHeight + 15);
    tft->setTextColor(ILI9341_YELLOW);
    tft->println("Hum");
    tft->setCursor(graphWidth + 2, y0 - graphHeight + 30);
    tft->setTextColor(ILI9341_RED);
    tft->println("Temp");
    tft->setCursor(graphWidth + 2, y0 - graphHeight + 45);
    tft->setTextColor(ILI9341_GREEN);
    tft->println("IAQ");

    tft->fillRect(x0 + 1, y0 - graphHeight, graphWidth - 2, graphHeight, ILI9341_BLACK);
    tft->drawLine(x0, y0, x0, y0 - graphHeight, ILI9341_WHITE);
    tft->drawLine(x0, y0, x0 + graphWidth, y0, ILI9341_WHITE);
    for (int i = 1; i < arrayLength; i++) {
      int x1 = int(float(i - 1) * mult1);
      if(i==arrayStorageCounter) {
        tft->drawLine(x1, y0, x1, y0 - graphHeight, ILI9341_LIGHTGREY);
        continue;
      }
      // int x1 = int(float(i - 1) * mult1);
      int x2 = int(float(i) * mult1);
      tft->drawLine(x1, int(y0 - float(CO2[i - 1]) * multCO2), x2, int(y0 - float(CO2[i]) * multCO2), ILI9341_CYAN);
      tft->drawLine(x1, int(y0 - float(Humidity[i - 1]) * multHum), x2, int(y0 - float(Humidity[i]) * multHum), ILI9341_YELLOW);
      tft->drawLine(x1, int(y0 - float(Temperature[i - 1]) * multTemp), x2, int(y0 - float(Temperature[i]) * multTemp), ILI9341_RED);
      tft->drawLine(x1, int(y0 - float(IAQ[i - 1]) * multIAQ), x2, int(y0 - float(IAQ[i]) * multIAQ), ILI9341_GREEN);

      // Serial.print("x:");
      // Serial.print(float(i-1) * mult1);
      // Serial.print(", ");
      // Serial.print(IAQ[i-1]);
      // Serial.print(":");
      // Serial.print(int(y0 - float(IAQ[i - 1]) * multIAQ));
      // Serial.print(" ");
    }
    // for (int i = 0; i < arrayLength; i++) {
    //   Serial.print(IAQ[i]);
    //   Serial.print(", ");
    // }
    // Serial.println("");
    redraw = false;
  }
  // Serial.println("");
}

// void Graph::setData(unsigned long upt, unsigned long epochT) {  //}, uint16_t carbon_dioxide, double hum, double temp, double lati, double longi, double bat) {
//   //logic description:
//   //if uptime is at specific interval, then log the uptime, then reset the log timer
//   if (upt - timer > storageInterval) {
//     timer = upt;
//     if (arrayStorageCounter == arrayLength) {
//       //once the array storage counter is equal to the array lenght,
//       //we will start to shift all the values in the array over by 1, and then store the new values at the end of the array
//       //this will allow the line graph to move over by one
//       for (int i = 1; i < arrayLength; i++) {
//         CO2[i - 1] = CO2[i];
//         IAQ[i - 1] = IAQ[i];
//         Humidity[i - 1] = Humidity[i];
//         Temperature[i - 1] = Temperature[i];
//         Battery[i - 1] = Battery[i];
//         Latitude[i - 1] = Latitude[i];
//         Longitude[i - 1] = Longitude[i];
//         Time[i - 1] = Time[i];
//       }
//     }
    

//     CO2[arrayStorageCounter] = mySensors->getCO2();
//     IAQ[arrayStorageCounter] = mySensors->getIAQScore();
//     Humidity[arrayStorageCounter] = mySensors->getHumidity();
//     Temperature[arrayStorageCounter] = mySensors->getTemp();
//     Battery[arrayStorageCounter] = mySensors->getBatteryVoltage();
//     Latitude[arrayStorageCounter] = mySensors->getLatitude();
//     Longitude[arrayStorageCounter] = mySensors->getLongitude();
//     Time[arrayStorageCounter] = epochT;

//     if (arrayStorageCounter < arrayLength) {
//       arrayStorageCounter = arrayStorageCounter + 1;
//     }
//     redraw = true;
//   }
// }

//trying to implement a circular buffer so we don't have to keep shifting the array over
void Graph::setData(unsigned long upt, unsigned long epochT) {  //}, uint16_t carbon_dioxide, double hum, double temp, double lati, double longi, double bat) {
  //logic description:
  //if uptime is at specific interval, then log the uptime, then reset the log timer
  if (upt - timer > storageInterval) {
    timer = upt;
    if (arrayStorageCounter == arrayLength) {
      arrayStorageCounter = 0;
      // startingIndex = startingIndex + 1;
    }
    

    CO2[arrayStorageCounter] = mySensors->getCO2();
    IAQ[arrayStorageCounter] = mySensors->getIAQScore();
    Humidity[arrayStorageCounter] = mySensors->getHumidity();
    Temperature[arrayStorageCounter] = mySensors->getTemp();
    Battery[arrayStorageCounter] = mySensors->getBatteryVoltage();
    Latitude[arrayStorageCounter] = mySensors->getLatitude();
    Longitude[arrayStorageCounter] = mySensors->getLongitude();
    Time[arrayStorageCounter] = epochT;

    if (arrayStorageCounter < arrayLength) {
      arrayStorageCounter = arrayStorageCounter + 1;
    }
    redraw = true;
  }
}

//a function to save the data to the sd card
void Graph::saveData() {}

bool Graph::checkForButtonClicks(uint16_t& x, uint16_t& y) {
  return false;
}

//a function to help scroll the graph left or right
void Graph::scrollGraph() {
}

//if touching down on a spot, display the values at that point and also show time / how much time ago
void Graph::displayValuesAtPoint(){  
}