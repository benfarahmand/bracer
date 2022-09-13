#ifndef GRAPH_H
#define GRAPH_H

#include <Arduino.h>

class Graph {
  private:
    double CO2 [];
    double Humidity [];
    double Temperature [];
    double Battery [];
    double Latitude [];
    double Longitude [];
    unsigned long Time []; //this is epoch time
    unsigned long timer; //timer to keep track of when to store values
    unsigned long startTime;
//    float vOC []; //to do once i attached the sensor
//    float Pulse []; //to do once i attach the sensor
  public:
    Graph();
    void init(unsigned long t);
    void draw();
    void setData(unsigned long upt, unsigned long epochT, double carbon_dioxide, double hum, double temp, double lati, double longi, double bat);
    void logData();
    
};

#endif
