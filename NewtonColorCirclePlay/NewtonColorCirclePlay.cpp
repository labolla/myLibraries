/*
  NewColorCirclePlay.cpp - library to play a color in relation with a sound
  Created by LaBolla, February 06 2016
  https://github.com/labolla
  Released into the public domain.

  ref: https://technochicblog.wordpress.com/2012/10/22/the-real-relationship-between-sound-and-color/

*/

#include "Arduino.h"
#include "NewtonColorCirclePlay.h"
#include "pitches.h"

#define DEBUG_SERIAL  1

#define TURNED_OFF 255
#define TURNED_ON 0

#define FADE_STEP 10  // 10ms

// ref http://www.devguru.com/features/colors:   0xRRGGBB
#define GREEN_YELLOW  0xAAFF00UL   // B
#define GREEN         0x00FF00UL   // C
#define GREEN_BLUE    0x00AAAAUL   // C#
#define BLUE          0x0055DDUL   // D
#define BLUE_VIOLET   0x0000FFUL   // D#
#define VIOLET        0x3300CCUL   // E
#define VIOLET_RED    0x6600AAUL   // F
#define RED           0x990077UL   // F#
#define RED_ORANGE    0xFF0000UL   // G
#define ORANGE        0xFF1100UL   // G#
#define ORANGE_YELLOW 0xCC3300UL   // A
#define YELLOW        0xAA9900UL   // A#

#define RED_MASK    0xFF0000UL
#define GREEN_MASK  0x00FF00UL
#define BLUE_MASK   0x0000FFUL
#define RED_SHIFT   16
#define GREEN_SHIFT  8
#define BLUE_SHIFT   0

NewtonColorCirclePlay::NewtonColorCirclePlay(int red, int green, int blue, int fadingRate, common_RGB_t common)
{
  // configure pin
  _redPin = red;
  _bluePin = blue;
  _greenPin = green;

  pinMode(_redPin, OUTPUT);
  pinMode(_bluePin, OUTPUT);
  pinMode(_greenPin, OUTPUT);

  // configure common type for RGB led
  _common_rgb_type = common;

  // configure fadingRate once for all (% value)
  _fadingRate = fadingRate;

  _redValue = TURNED_ON;
  _greenValue = TURNED_ON;
  _blueValue = TURNED_ON;

  analogWrite(_redPin,_redValue);
  analogWrite(_greenPin,_greenValue);
  analogWrite(_bluePin,_blueValue);
}

void NewtonColorCirclePlay::Display(int tone, int duration)
{
  unsigned long hex_rgb = 0;
  signed int r_old = (signed int) _redValue;
  signed int g_old = (signed int) _greenValue;
  signed int b_old = (signed int) _blueValue;

//  Serial.print("NewtonColorCirclePlay::Display : ");
//  Serial.print(tone);

  // map sound into hex value for RGB led
  switch (tone) {
  case NOTE_B0:
  case NOTE_B1:
  case NOTE_B2:
  case NOTE_B3:
  case NOTE_B4:
  case NOTE_B5:
  case NOTE_B6:
    hex_rgb = GREEN_YELLOW;
#if DEBUG_SERIAL
    Serial.print("B \t");
#endif // DEBUG_SERIAL
    break;

  case NOTE_C1:
  case NOTE_C2:
  case NOTE_C3:
  case NOTE_C4:
  case NOTE_C5:
  case NOTE_C6:
  case NOTE_C7:
  case NOTE_C8:
    hex_rgb = GREEN;
#if DEBUG_SERIAL
    Serial.print("C \t");
#endif // DEBUG_SERIAL
    break;

  case NOTE_CS1:
  case NOTE_CS2:
  case NOTE_CS3:
  case NOTE_CS4:
  case NOTE_CS5:
  case NOTE_CS6:
  case NOTE_CS7:
  case NOTE_CS8:
    hex_rgb = GREEN_BLUE;
#if DEBUG_SERIAL
    Serial.print("C#\t");
#endif // DEBUG_SERIAL
    break;

  case NOTE_D1:
  case NOTE_D2:
  case NOTE_D3:
  case NOTE_D4:
  case NOTE_D5:
  case NOTE_D6:
  case NOTE_D7:
  case NOTE_D8:
    hex_rgb = BLUE;
#if DEBUG_SERIAL
    Serial.print("D \t");
#endif // DEBUG_SERIAL
    break;

  case NOTE_DS1:
  case NOTE_DS2:
  case NOTE_DS3:
  case NOTE_DS4:
  case NOTE_DS5:
  case NOTE_DS6:
  case NOTE_DS7:
  case NOTE_DS8:
    hex_rgb = BLUE_VIOLET;
#if DEBUG_SERIAL
    Serial.print("D#\t");
#endif // DEBUG_SERIAL
    break;

  case NOTE_E1:
  case NOTE_E2:
  case NOTE_E3:
  case NOTE_E4:
  case NOTE_E5:
  case NOTE_E6:
  case NOTE_E7:
    hex_rgb = VIOLET;
#if DEBUG_SERIAL
    Serial.print("E \t");
#endif // DEBUG_SERIAL
    break;

  case NOTE_F1:
  case NOTE_F2:
  case NOTE_F3:
  case NOTE_F4:
  case NOTE_F5:
  case NOTE_F6:
  case NOTE_F7:
    hex_rgb = VIOLET_RED;
#if DEBUG_SERIAL
    Serial.print("F \t");
#endif // DEBUG_SERIAL
    break;

  case NOTE_FS1:
  case NOTE_FS2:
  case NOTE_FS3:
  case NOTE_FS4:
  case NOTE_FS5:
  case NOTE_FS6:
  case NOTE_FS7:
    hex_rgb = RED;
#if DEBUG_SERIAL
    Serial.print("F#\t");
#endif // DEBUG_SERIAL
    break;

  case NOTE_G1:
  case NOTE_G2:
  case NOTE_G3:
  case NOTE_G4:
  case NOTE_G5:
  case NOTE_G6:
  case NOTE_G7:
    hex_rgb = RED_ORANGE;
#if DEBUG_SERIAL
    Serial.print("G \t");
#endif // DEBUG_SERIAL
    break;

  case NOTE_GS1:
  case NOTE_GS2:
  case NOTE_GS3:
  case NOTE_GS4:
  case NOTE_GS5:
  case NOTE_GS6:
  case NOTE_GS7:
    hex_rgb = ORANGE;
#if DEBUG_SERIAL
    Serial.print("G#\t");
#endif // DEBUG_SERIAL
    break;

  case NOTE_A1:
  case NOTE_A2:
  case NOTE_A3:
  case NOTE_A4:
  case NOTE_A5:
  case NOTE_A6:
  case NOTE_A7:
    hex_rgb = ORANGE_YELLOW;
#if DEBUG_SERIAL
    Serial.print("A \t");
#endif // DEBUG_SERIAL
    break;

  case NOTE_AS1:
  case NOTE_AS2:
  case NOTE_AS3:
  case NOTE_AS4:
  case NOTE_AS5:
  case NOTE_AS6:
  case NOTE_AS7:
    hex_rgb = YELLOW;
#if DEBUG_SERIAL
    Serial.print("A# \t");
#endif // DEBUG_SERIAL
    break;

  default:
    // error: just ignore passed sound pitch and do nothing
#if DEBUG_SERIAL
    Serial.print("unrecognized pitch");
    Serial.println(tone);
#endif // DEBUG_SERIAL
    return;
  }

#if DEBUG_SERIAL
  Serial.print("hex_rgb : ");
  Serial.print(hex_rgb, HEX);
#endif // DEBUG_SERIAL

  if (_common_rgb_type == COMMON_ANODE)
  {
    _redValue   = 0xFF- ((hex_rgb & RED_MASK) >> RED_SHIFT);
    _greenValue = 0xFF- ((hex_rgb & GREEN_MASK) >> GREEN_SHIFT);
    _blueValue  = 0xFF- ((hex_rgb & BLUE_MASK) >> BLUE_SHIFT);
  }
  else if
  (_common_rgb_type == COMMON_CATHODE)
  {
    _redValue   = ((hex_rgb & RED_MASK) >> RED_SHIFT);
    _greenValue = ((hex_rgb & GREEN_MASK) >> GREEN_SHIFT);
    _blueValue  = ((hex_rgb & BLUE_MASK) >> BLUE_SHIFT);
  }
  else
  {
#if DEBUG_SERIAL
    Serial.print("Wrong common type for LED: ");
    Serial.println(_common_rgb_type);
#endif // DEBUG_SERIAL
    return;
  }
#if DEBUG_SERIAL
  Serial.print(" ");
  Serial.print(_redValue, HEX);
  Serial.print(" ");
  Serial.print(_greenValue, HEX);
  Serial.print(" ");
  Serial.println(_blueValue, HEX);
#endif // DEBUG_SERIAL

  //TODO: not support fading for the moment
  if (_fadingRate == 0)
  {// apply immediately the new colors
    analogWrite(_redPin,_redValue);
    analogWrite(_greenPin,_greenValue);
    analogWrite(_bluePin,_blueValue);
    delay(duration);
  }
  else
  {
    //compute fade duration based on configured fading rate
    int fade_duration = duration * _fadingRate / 100;
    //compute number of fading steps and delta for each color
    int steps = fade_duration / FADE_STEP;
    signed int r_step = (_redValue - r_old)/steps;
    signed int g_step = (_greenValue - r_old)/steps;
    signed int b_step = (_blueValue - r_old)/steps;

    for (int i=0; i < steps; i++)
    {
      analogWrite(_redPin,  r_old + (i*r_step));
      analogWrite(_greenPin,g_old + (i*g_step));
      analogWrite(_bluePin, b_old + (i*b_step));
      delay(FADE_STEP);
    }

    analogWrite(_redPin,_redValue);
    analogWrite(_greenPin,_greenValue);
    analogWrite(_bluePin,_blueValue);

    delay(duration - fade_duration);
  }
}

void NewtonColorCirclePlay::SetRGB(int r, int g, int b)
{
#if DEBUG_SERIAL
  Serial.print("hex_rgb : ");
  Serial.print(r, HEX);
  Serial.print(g, HEX);
  Serial.println(b, HEX);
#endif // DEBUG_SERIAL

#if DEBUG_SERIAL
  Serial.print(" ");
  Serial.print(0xFF-_redValue, HEX);
  Serial.print(" ");
  Serial.print(0xFF-_greenValue, HEX);
  Serial.print(" ");
  Serial.println(0xFF-_blueValue, HEX);
#endif // DEBUG_SERIAL

  if (_common_rgb_type == COMMON_ANODE)
  {
    _redValue   = 0xFF- r;
    _greenValue = 0xFF- g;
    _blueValue  = 0xFF- b;
  }
  else if
  (_common_rgb_type == COMMON_CATHODE)
  {
    _redValue   = r;
    _greenValue = g;
    _blueValue  = b;
  }
  else
  {
#if DEBUG_SERIAL
    Serial.print("Wrong common type for LED: ");
    Serial.println(_common_rgb_type);
#endif // DEBUG_SERIAL
    return;
  }

  analogWrite(_redPin,_redValue);
  analogWrite(_greenPin,_greenValue);
  analogWrite(_bluePin,_blueValue);
}

