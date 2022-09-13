#include "Graph.h"

Graph::Graph() {}

void Graph::init(unsigned long t) {
  int arrayLength = 20; //i'm thinking that the graph should show the last 10 minutes of data recorded at 30 second intervals
  startTime = t;
  CO2 [arrayLength];
  Humidity [arrayLength];
  Temperature [arrayLength];
  Battery [arrayLength];
  Latitude [arrayLength];
  Longitude [arrayLength];
  Time [arrayLength]; //this is epoch time
}

void Graph::draw() {}

void Graph::setData(unsigned long upt, unsigned long epochT, double carbon_dioxide, double hum, double temp, double lati, double longi, double bat) {
  //logic description:
  //if uptime is at specific interval, then log the uptime, then reset the log timer
  if (upt - timer > 30) { //greater than 30 seconds
    timer = upt;
    if (upt < 600) { //less then 10 minutes
      //condition for logging data when up time is less than 10 minutes
    } else {
      //condition for logging data when up time is more than 10 minutes
    }
  }
}

void Graph::logData() {
}
