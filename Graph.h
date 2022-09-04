#ifndef GRAPH_H
#define GRAPH_H

#include <Arduino.h>

class Graph {
  private:
  //thinking that the graph should show the last 10 minutes of data recorded at 30 second intervals
    float CO2 [20];
    float Humidity [20];
    float Temperature [20];
    float Battery [20];
    float Latitude [20];
    float Longitude [20];
    float Time [20];
//    float vOC []; //to do once i attached the sensor
//    float Pulse []; //to do once i attach the sensor
  public:
   
    Graph();
    void draw();
    void setData();
    
};

#endif
