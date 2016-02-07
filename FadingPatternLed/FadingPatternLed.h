/*
  FadingPatternLed.h - library to use an RGB led playing dynamic fading pattern
  Created by LaBolla, February 06 2016
  https://github.com/labolla
  Released into the public domain.

  Class that let play never ending pattern for an RGB led wit a fade-in/out effect
  on all R-G-B colours.
  An input may be associated to increase brightness and speed on a specific color
  based on a desired logic
*/

#ifndef FADINGPATTERNLED_H_INCLUDED
#define FADINGPATTERNLED_H_INCLUDED

#include "Arduino.h"


/*A pattern is defined by 4 states:
 led_off/fade-in/ led_on    /fade-off
                ____________
               /            \
              /              \
             /                \
            /                  \
  _________/                    \______

  There is an idle state wit a relaxed definition of pattern that apply to all R-G-B.
  Based on an input a pattern for a selected led can be excited.
  Excited means that both brigtness and change increase (so duration in each state is reduced)
  There is an max excited state.

  Logic is that pattern start in idle and then
  - if button is pressed (or any desired input) increase max_brightness and reduce blinking interval (Exicted/fast state).
  - if not pressed move back to idle pattern

  NOTE: the fast/exicited led pattern paramters are hard-coded into source code while idle/relaxed ones
        are application specific and passed in the constructor

*/

// input sampling interval
#define SAMPLING_TIME  100

class FadingPatternLed
{
  public:
    // based on desired User Input the led pattern could exciting or not
    bool exciting;

    // Constructor and API: pin and 'idle' (aka relaxed) led pattern settings
    FadingPatternLed(int pin, long fadeIn, long on, long fadeOut, long off, int max_bright);

    // called to update led pattern based on excited or not state
    void updatePattern();

    // called to update the led based on current status and current timing
    // routine handle also the led pattern state transition
    void UpdateDisplay (unsigned long currTime);

  private:
    int  _ledPin;  // GPIO to drive led

    // dynamically changed pattern parameters (led state timings and max brightness)
    // need to be public to be accessed by extern when printing value
    long _OnTime;
    long _OffTime;
    long _fadeInTime;
    long _fadeOutTime;
    int  _maxBright;

    // current "ld pattern" state  (fade-in, on, fade-out, off)
    int _ledState;

    // keep track of timing to update led pattern state
    signed long _prevTime;

    long _led_on_time_idle;
    long _led_off_time_idle;
    long _fade_in_time_idle;
    long _fade_out_time_idle;
    int _idle_bright;

    // step to increase/decrease brightness every sampling interval
    int _bright_up_step;
    int _bright_dw_step;
    // step to increase/decrease duration in each led pattern state every sampling interval
    unsigned long _blink_up_on_step;
    unsigned long _blink_up_off_step;
    unsigned long _blink_up_in_step;
    unsigned long _blink_up_out_step;

    unsigned long _blink_dw_on_step;
    unsigned long _blink_dw_off_step;
    unsigned long _blink_dw_in_step;
    unsigned long _blink_dw_out_step;
};

#endif // FADINGPATTERNLED_H_INCLUDED
