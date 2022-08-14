#include "SCD41.h"

//the following data sheet contains information about how to go into low power mode and also single shot measurements
//https://sensirion.com/media/documents/C4B87CE6/627C2DCD/CD_DS_SCD40_SCD41_Datasheet_D1.pdf


SCD41::SCD41() {
}

//To Do: print the error message to the screen, if there's an issue and remove the Serial dependency
void SCD41::init(TwoWire Wire) {
  scd4x.begin(Wire);

  // stop potentially previously started measurement
  scd4x.stopPeriodicMeasurement();

  // Start Measurement
  scd4x.startPeriodicMeasurement(); //high power
  //  scd4x.startLowPowerPeriodicMeasurement(); // low power
  //  scd4x.measureSingleShot() //lower power mode
  co2 = 0;
  temperature = 0.0f;
  humidity = 0.0f;

}

//for the measure function, i might have to create a timer that samples at specific intervals
void SCD41::measure() {
  uint16_t error;
  char errorMessage[256];

  delay(100);


  // Read Measurement
  bool isDataReady = false;
  error = scd4x.getDataReadyFlag(isDataReady);
  if (error) {
    return;
  }
  if (!isDataReady) {
    return;
  }
  error = scd4x.readMeasurement(co2, temperature, humidity);
  if (error) {
    //    Serial.print("Error trying to execute readMeasurement(): ");
    //    errorToString(error, errorMessage, 256);
    //    Serial.println(errorMessage);
  } else if (co2 == 0) {
    //    Serial.println("Invalid sample detected, skipping.");
  } else {
    temperature = temperature * 9 / 5 + 32;
  }
}

void SCD41::draw(TFT_eSPI tft) {

  //    String measurement = "CO2: " + co2 + " ppm Temperature: " + temperature + " Humidity: " + humidity;

  tft.drawString("CO2: ", 5, 10, 2);
  tft.drawNumber(co2, 10+tft.textWidth("CO2: "), 10, 2);
  //    tft.print("CO2: ");
  //    tft.print(co2);
  //    tft.print(" ppm");
  //    tft.print(" Temperature: ");
  //    tft.print(temperature);
  //    tft.print(" Humidity:");
  //    tft.print(humidity);

  //  }
}
