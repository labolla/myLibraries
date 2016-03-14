/*
  NewColorCirclePlay.h - library to play a color in relation with a sound
  Created by LaBolla, February 06 2016
  https://github.com/labolla
  Released into the public domain.

  ref: https://technochicblog.wordpress.com/2012/10/22/the-real-relationship-between-sound-and-color/

*/

#ifndef NEWCOLORCIRCLEPLAY_H_INCLUDED
#define NEWCOLORCIRCLEPLAY_H_INCLUDED

#include "Arduino.h"

typedef enum
{
  COMMON_CATHODE,
  COMMON_ANODE,
} common_RGB_t;


class NewtonColorCirclePlay
{
  public:

  NewtonColorCirclePlay(int red, int green, int blue, int fadingRate, common_RGB_t common);

  void Display(int tone, int duration);
  void SetRGB(int r, int g, int b);

  private:

  // RGB led pin
  int _redPin;
  int _greenPin;
  int _bluePin;

  // common RGB type: ANODE type is when common is to 5V and led are ON when driving gpio is 0;
  //                  CATHODE one has the common connected to gnd and pin logic is the reverse
  common_RGB_t  _common_rgb_type;

  // range [0:100] if 0 means to quickly change color; otherwise percentage of passd duration to fade
  int _fadingRate;

  // store current RGB value - used in fading
  int _redValue;
  int _blueValue;
  int _greenValue;
};

#endif NEWCOLORCIRCLEPLAY_H_INCLUDED
