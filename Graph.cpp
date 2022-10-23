#include "Graph.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_GFX.h"
#include "Sensors.h"

Graph::Graph() {}

// Graph::~Graph(){
//   delete[] Humidity;
// }

void Graph::init(Adafruit_ILI9341& _tft, Sensors& s, unsigned long t) {
  // arrayStorageCounter = 0;
  // storageInterval = 30; //record the data once every 30 seconds
  // arrayLength = 600 / storageInterval; //i'm thinking  the graph should show the last 10 minutes of data. therefore, formula for # of indexes is (600 seconds / storage interval)
  startTime = t;
  tft = &_tft;
  mySensors = &s;
  // CO2 [arrayLength];
  // Humidity[arrayLength];
  // Temperature [arrayLength];
  // Battery [arrayLength];
  // Latitude [arrayLength];
  // Longitude [arrayLength];
  // Time [arrayLength]; //this is epoch time
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
  int16_t x0 = 0;
  int16_t y0 = 220;
  int16_t graphWidth = 250;
  int16_t graphHeight = 150;
  float mult1 = graphWidth / arrayLength ;
  float mult2 = graphHeight / 5000 ;
  
  tft->setCursor(0,42);
  tft->drawLine(x0, y0, x0, y0-graphHeight, ILI9341_WHITE);
  tft->drawLine(x0, y0, x0+graphWidth, y0, ILI9341_WHITE);
  // tft->drawLine(50,50,100,100,ILI9341_WHITE);
  // for(int i = 1 ; i < arrayLength ; i++){
    // tft->drawLine(float(i-1) * mult1, y0 - float(CO2[i-1]) * mult2, float(i) * mult1, y0 - float(CO2[i]) * mult2, ILI9341_RED);
    // Serial.print("x:");
    // Serial.print(float(i-1) * mult1);
    // Serial.print(", ");    
    // Serial.print("y:");
    // Serial.print(y0 - CO2[i-1] * mult2);
    // Serial.print(" ");    
  // }
  // Serial.println("");
}

void Graph::setData(unsigned long upt, unsigned long epochT){//}, uint16_t carbon_dioxide, double hum, double temp, double lati, double longi, double bat) {
  //logic description:
  //if uptime is at specific interval, then log the uptime, then reset the log timer
  if (upt - timer > storageInterval) {
    timer = upt;
    if (arrayStorageCounter == arrayLength) {
      //once the array storage counter is equal to the array lenght,
      //we will start to shift all the values in the array over by 1, and then store the new values at the end of the array
      //this will allow the line graph to move over by one
      for (int i = 1 ; i < arrayLength ; i ++) {
        CO2[i - 1] = CO2[i];
        Humidity[i - 1] = Humidity[i];
        Temperature[i - 1] = Temperature[i];
        Battery[i - 1] = Battery[i];
        Latitude[i - 1] = Latitude[i];
        Longitude[i - 1] = Longitude[i];
        Time[i - 1] = Time[i];
      }
    }

    CO2[arrayStorageCounter] = mySensors->getCO2();
    Humidity[arrayStorageCounter] = mySensors->getHumidity();
    Temperature[arrayStorageCounter] = mySensors->getTemp();
    Battery[arrayStorageCounter] = mySensors->getBatteryVoltage();
    Latitude[arrayStorageCounter] = mySensors->getLatitude();
    Longitude[arrayStorageCounter] = mySensors->getLongitude();
    Time[arrayStorageCounter] = epochT;

    if ( arrayStorageCounter < arrayLength) {
      arrayStorageCounter = arrayStorageCounter + 1;
    }
  }
}

//a function to save the data to the sd card
void Graph::saveData(){}

bool Graph::checkForButtonClicks(uint16_t& x, uint16_t& y) {
  return false;
}  
