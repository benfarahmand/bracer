#include "Button.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
//
//  /*! https://github.com/adafruit/Adafruit-GFX-Library/blob/master/Adafruit_GFX.cpp
//     @brief    Initialize button with our desired color/size/settings
//     @param    gfx     Pointer to our display so we can draw to it!
//     @param    x       The X coordinate of the center of the button
//     @param    y       The Y coordinate of the center of the button
//     @param    w       Width of the buttton
//     @param    h       Height of the buttton
//     @param    outline  Color of the outline (16-bit 5-6-5 standard)
//     @param    fill  Color of the button fill (16-bit 5-6-5 standard)
//     @param    textcolor  Color of the button label (16-bit 5-6-5 standard)
//     @param    label  Ascii string of the text inside the button
//     @param    textsize The font magnification of the label text
//  */
//  //initButton(Adafruit_GFX *gfx, int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t outline, uint16_t fill, uint16_t textcolor,char *label, uint8_t textsize)
Button::Button(){};

void Button::initButton(Adafruit_ILI9341 &tft, int16_t _x, int16_t _y, uint16_t _w, uint16_t _h, uint16_t _outline, uint16_t _fill, uint16_t _textcolor,String _text, uint8_t _textsize){
  x=_x;
  y=_y;
  w=_w;
  h=_h;
  outline=_outline;
  fill=_fill;
  textcolor=_textcolor;
  text=_text;
  textsize=_textsize;
  int16_t tX=0;
  int16_t tY=0;
  tWidth=60;
  tHeight=20;
  tft.setTextSize(textsize);
  tft.getTextBounds(text,0,0,&tX,&tY,&tWidth,&tHeight);
}

void Button::drawButton(Adafruit_ILI9341 &tft) {
  tft.drawRect(x, y, w, h, outline);
  tft.drawRect(x+1, y+1, w-2, h-2, fill);
  tft.setCursor(x+w/2-tWidth/2, y+h/2-tHeight/2);
  tft.setTextColor(textcolor, ILI9341_BLACK);
  tft.setTextSize(textsize);
  tft.print(text);
}

bool Button::contains(int16_t _x, int16_t _y) {
  return ((_x >= x) && (_x < (int16_t)(x + w)) && (_y >= y) && (_y < (int16_t)(y + h)));
}
