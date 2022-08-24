#include "Display.h"
#include "Adafruit_TSC2007.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_MISO 19
#define TFT_MOSI 18
#define TFT_CLK 5
#define TFT_CS   16  // Chip select control pin
#define TFT_DC   17  // Data Command control pin
#define TFT_RST  21 // Reset pin (could connect to RST pin)
// #define TFT_LIGHT 


Display::Display(){
    Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

    //touch screen
    Adafruit_TSC2007 touch;
}