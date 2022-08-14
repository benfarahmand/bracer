#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include "DigitalWatch.h"
#include "Adafruit_TSC2007.h"
#include "SCD41.h"
//#include "FreeRTOS.h"
//#include "task.h"

//#if CONFIG_FREERTOS_UNICORE
//#define ARDUINO_RUNNING_CORE 0
//#else
//#define ARDUINO_RUNNING_CORE 1
//#endif

#define TFT_GREY 0x5AEB

void taskDisplay(void *parameters);
void taskSensors(void *parameters);

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
Adafruit_TSC2007 touch;

DigitalWatch dw = DigitalWatch();
SCD41 scd41 = SCD41();

unsigned int colour = 0;
int count = 0;

void setup(void) {
    Serial.begin(115200);
  Wire.begin();
  scd41.init(Wire);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.setTextSize(1);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);

  if (!touch.begin()) {
    while (1) {
      delay(10);
    }
  }

  dw.init();

  xTaskCreate(
    taskDisplay
    ,  "taskDisplay"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL);

  xTaskCreate(
    taskSensors
    ,  "taskSensors"
    ,  1024  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL);

    vTaskStartScheduler();
}

void loop() {
  //  scd41.measure();
  //TOUCH SENSOR
  //  uint16_t x, y, z1, z2;
  //  double maxX = 0.078125, maxY = 0.05859375;
  //  if (touch.read_touch(&x, &y, &z1, &z2) && (z1 > 100)) {
  //    x = 320 - ((double)x) * maxX;
  //    y = 240 - ((double)y) * maxY;
  //
  //    //    Serial.print("Touch point: (");
  //    //    Serial.print(x); Serial.print(", ");
  //    //    Serial.print(y); Serial.print(", ");
  //    //    Serial.print(z1); Serial.print(" / ");
  //    //    Serial.print(z2); Serial.println(")");
  //
  //    tft.drawSpot(x, y, 25, TFT_WHITE);
  //  }
  //draw the things
//  dw.draw(tft); //digital watch
//  scd41.draw(tft); //co2, temp, humidity sensor


}

void taskDisplay(void *parameters) {
  (void) parameters;

  for (;;) {
    dw.draw(tft); //digital watch
    scd41.draw(tft); //co2, temp, humidity sensor
    vTaskDelay(1000 );
  }
}

void taskSensors(void *parameters) {
  (void) parameters;

  for (;;) {
//    scd41.measure();
  Serial.println(count++);
    vTaskDelay(1000 );
  }
}
