#ifndef GRAPH_H
#define GRAPH_H

#include <Arduino.h>

class Graph {
  private:
  //thinking that the graph should show the last 10 minutes of data recorded at 30 second intervals
    double CO2 [20];
    double Humidity [20];
    double Temperature [20];
    double Battery [20];
    double Latitude [20];
    double Longitude [20];
    unsigned long Time [20]; //this is epoch time
    unsigned long timer; //timer to keep track of when to store values
//    float vOC []; //to do once i attached the sensor
//    float Pulse []; //to do once i attach the sensor
  public:
    Graph();
    void draw();
    void setData();
    
};

#endif
