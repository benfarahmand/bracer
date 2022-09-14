#include "Graph.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_GFX.h"

Graph::Graph() {}

void Graph::init(unsigned long t) {
  arrayStorageCounter = 0;
  storageInterval = 30; //record the data once every 30 seconds
  arrayLength = 600 / storageInterval; //i'm thinking  the graph should show the last 10 minutes of data. therefore, formula for # of indexes is (600 seconds / storage interval)
  startTime = t;
  CO2 [arrayLength];
  Humidity [arrayLength];
  Temperature [arrayLength];
  Battery [arrayLength];
  Latitude [arrayLength];
  Longitude [arrayLength];
  Time [arrayLength]; //this is epoch time
}

void Graph::draw(Adafruit_ILI9341 &tft) {
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
}

void Graph::setData(unsigned long upt, unsigned long epochT, double carbon_dioxide, double hum, double temp, double lati, double longi, double bat) {
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

    CO2[arrayStorageCounter] = carbon_dioxide;
    Humidity[arrayStorageCounter] = hum;
    Temperature[arrayStorageCounter] = temp;
    Battery[arrayStorageCounter] = bat;
    Latitude[arrayStorageCounter] = lati;
    Longitude[arrayStorageCounter] = longi;
    Time[arrayStorageCounter] = epochT;

    if ( arrayStorageCounter < arrayLength) {
      arrayStorageCounter = arrayStorageCounter + 1;
    }
  }
}

void Graph::saveData(){}
