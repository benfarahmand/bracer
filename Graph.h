#ifndef GRAPH_H
#define GRAPH_H

#include <Arduino.h>
#include "Adafruit_ILI9341.h"
#include "Adafruit_GFX.h"
#include "Sensors.h"
// #include <memory>

#define arrayLength 20 //array length is 600 / storage interval

class Graph {
private:
  // int arrayLength = 20;
  // int arrayStorageCounter;
  // int storageInterval;
  bool redraw = false;
  int arrayStorageCounter = 0;
  int startingIndex = 0;
  int storageInterval = 30;                 //record the data once every 30 seconds
  // int arrayLength = 600 / storageInterval;  //i'm thinking  the graph should show the last 10 minutes of data. therefore, formula for # of indexes is (600 seconds / storage interval)    double CO2 [arrayLength];
  // std::unique_ptr<double[]> Humidity;
  uint16_t CO2[arrayLength] = {};
  int IAQ[arrayLength] = {};
  double Humidity[arrayLength] = {};
  double Temperature[arrayLength] = {};
  double Battery[arrayLength] = {};
  double Latitude[arrayLength] = {};
  double Longitude[arrayLength] = {};
  unsigned long Time[arrayLength] = {};  //this is epoch time
  unsigned long timer;              //timer to keep track of when to store values
  unsigned long startTime;
  //    float vOC []; //to do once i attached the sensor
  //    float Pulse []; //to do once i attach the sensor
public:
  Graph();
  Adafruit_ILI9341* tft;
  Sensors* mySensors;

  void init(Adafruit_ILI9341& _tft, Sensors& s ,unsigned long t);
  void draw();
  void setData(unsigned long upt, unsigned long epochT);//, uint16_t carbon_dioxide, double hum, double temp, double lati, double longi, double bat);
  void saveData();
  bool checkForButtonClicks(uint16_t& x, uint16_t& y);
  void scrollGraph();
  void displayValuesAtPoint();
};

#endif