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

#include "Arduino.h"
#include "FadingPatternLed.h"

#define _DEBUG_FADING_PATTERN_LED

// LED pattern definition section: states, idle/min value for fade-in/out on /off value;
// states
#define LED_OFF 0
#define LED_ON 1
#define LED_FADE_IN 2
#define LED_FADE_OUT 3

// Max "excited" state (fast) paramters: duration and brightness
#define FADE_IN_FAST_TIME  100
#define LED_ON_FAST_TIME   100
#define FADE_OUT_FAST_TIME 100
#define LED_OFF_FAST_TIME  200
#define FAST_BRIGHT 255

// milli-seconds needed to ramp up/down
// from idle state to excited/fast one and vice-versa
#define UP_TIME 2000
#define DOWN_TIME 15000

bool quickrampOption = false;

// constructor - create LedFader
FadingPatternLed::FadingPatternLed(int pin, long fadeIn, long on, long fadeOut, long off, int max_bright)
{
  // configure pin
  _ledPin = pin;
  pinMode(_ledPin, OUTPUT);

  // init current value for dynamically changed pattern parameters and keep track of idle ones
  _OnTime = _led_on_time_idle = on;
  _OffTime = _led_off_time_idle = off;
  _fadeInTime = _fade_in_time_idle = fadeIn;
  _fadeOutTime = _fade_out_time_idle = fadeOut;
  _maxBright = _idle_bright = max_bright;

  _ledState = LED_OFF;
  // init randomly the curr off time
  //_prevTime = random((pin-RED_LED)*fadeInTime/3, (pin+1-RED_LED)*fadeInTime/3);
  _prevTime = random(0, _fadeInTime);

  exciting = false;

  // init step to update brightness and duration for exiciting and relaxig
  _bright_up_step= ceil((FAST_BRIGHT-_idle_bright)*SAMPLING_TIME/UP_TIME);
  _bright_dw_step= ceil((FAST_BRIGHT-_idle_bright)*SAMPLING_TIME/DOWN_TIME);

  _blink_up_on_step  = max(1,((long)(_led_on_time_idle  - LED_ON_FAST_TIME))*SAMPLING_TIME/UP_TIME);
  _blink_up_off_step = max(1,((long)(_led_off_time_idle - LED_OFF_FAST_TIME))*SAMPLING_TIME/UP_TIME);
  _blink_up_in_step  = max(1,((long)(_fade_in_time_idle - FADE_IN_FAST_TIME))*SAMPLING_TIME/UP_TIME);
  _blink_up_out_step = max(1,((long)(_fade_out_time_idle- FADE_OUT_FAST_TIME))*SAMPLING_TIME/UP_TIME);

  _blink_dw_on_step  = max(1,((long)(_led_on_time_idle  - LED_ON_FAST_TIME))*SAMPLING_TIME/DOWN_TIME);
  _blink_dw_off_step = max(1,((long)(_led_off_time_idle - LED_OFF_FAST_TIME))*SAMPLING_TIME/DOWN_TIME);
  _blink_dw_in_step  = max(1,((long)(_fade_in_time_idle - FADE_IN_FAST_TIME))*SAMPLING_TIME/DOWN_TIME);
  _blink_dw_out_step = max(1,((long)(_fade_out_time_idle- FADE_OUT_FAST_TIME))*SAMPLING_TIME/DOWN_TIME);

#ifdef _DEBUG_FADING_PATTERN_LED
  Serial.begin(9600);
  while (!Serial);
  Serial.print("Led initValue for pin ");
  Serial.println(_ledPin);
  Serial.print(_bright_up_step);
  Serial.print('\t');
  Serial.println(_bright_dw_step);

  Serial.print(_blink_dw_on_step);
  Serial.print('\t');
  Serial.print(_blink_dw_off_step);
  Serial.print('\t');
  Serial.print(_blink_dw_in_step);
  Serial.print('\t');
  Serial.println(_blink_dw_out_step);

  Serial.print(_blink_up_on_step);
  Serial.print('\t');
  Serial.print(_blink_up_off_step);
  Serial.print('\t');
  Serial.print(_blink_up_in_step);
  Serial.print('\t');
  Serial.println(_blink_up_out_step);
#endif // _DEBUG_FADING_PATTERN_LED
}

// update dynamically current pattern settings based on input "excited" or not
void FadingPatternLed::updatePattern()
{
  if (exciting)
  {
    //increment max bright to blink (with saturation)
    _maxBright = (((long)_maxBright + _bright_up_step) > FAST_BRIGHT ? FAST_BRIGHT : _maxBright + _bright_up_step);

    // decrease duration of each blinking state to speed up flashing;
    _OnTime = _OnTime - _blink_up_on_step;
    _OnTime = max(_OnTime, LED_ON_FAST_TIME);
    _OffTime = _OffTime - _blink_up_off_step;
    _OffTime = max(_OffTime, LED_OFF_FAST_TIME);
    _fadeInTime = _fadeInTime - _blink_up_in_step;
    _fadeInTime = max(_fadeInTime, FADE_IN_FAST_TIME);
    _fadeOutTime = _fadeOutTime - _blink_up_out_step;
    _fadeOutTime = max(_fadeOutTime, FADE_OUT_FAST_TIME);
  }
  else
  {
    //decrement max bright, not less then IDLE_BRIGHT
   _maxBright = (((long)_maxBright - _bright_dw_step) > _idle_bright ? _maxBright - _bright_dw_step : _idle_bright);

    // increase duration of each blinking state to move back to idle flashing
    _OnTime = _OnTime + _blink_dw_on_step;
    _OnTime = min(_OnTime, _led_on_time_idle);
    _OffTime = _OffTime + _blink_dw_off_step;
    _OffTime = min(_OffTime, _led_off_time_idle);
    _fadeInTime = _fadeInTime + _blink_dw_in_step;
    _fadeInTime = min(_fadeInTime, _fade_in_time_idle);
    _fadeOutTime = _fadeOutTime + _blink_dw_out_step;
    _fadeOutTime = min(_fadeOutTime, _fade_out_time_idle);
  }
}



//  void UpdateDisplay()
void FadingPatternLed::UpdateDisplay (unsigned long currTime)
{
  if ((exciting)&&(quickrampOption))
  {// move to excited state immediately- till released
    analogWrite(_ledPin,FAST_BRIGHT);
    _ledState=LED_ON;
    _prevTime=currTime;
    _OnTime = LED_ON_FAST_TIME;
    _OffTime = LED_OFF_FAST_TIME;
    _fadeInTime = FADE_IN_FAST_TIME;
    _fadeOutTime = FADE_OUT_FAST_TIME;
  }
  else
  {
    // check state machine, update status and prevTime, compute fade value if needed and update pin
    if ((_ledState==LED_OFF) && ((signed long) (currTime-_prevTime) >= _OffTime))
    {//OFF->FADE_IN
      _ledState = LED_FADE_IN;
      _prevTime = currTime;
    }
    else if (_ledState==LED_FADE_IN)
    {
      if (currTime-_prevTime >= _fadeInTime)
      {//FADE_IN->ON
        _ledState = LED_ON;
        _prevTime = currTime;
        analogWrite(_ledPin,_maxBright);
      }
      else
      {//just update fade value
        int fadeValue = (currTime-_prevTime)*_maxBright/_fadeInTime;
        // if fadeTime has changed we may have been gone above maxBright
        fadeValue = min(fadeValue, _maxBright);
        analogWrite(_ledPin,fadeValue);
      }
    }
    else if ((_ledState==LED_ON)&&(currTime-_prevTime >= _OnTime))
    {//ON->FADE_OUT
      _ledState=LED_FADE_OUT;
      _prevTime=currTime;
      int fadeValue = _maxBright;
      analogWrite(_ledPin,fadeValue);
    }
    else if (_ledState==LED_FADE_OUT)
    {
      if (currTime-_prevTime >= _fadeOutTime)
      {//FADE_OUT->OFF
        _ledState=LED_OFF;
        _prevTime=currTime;
        analogWrite(_ledPin,0);
      }
      else
      {//just update fade value
        int fadeValue = _maxBright - (currTime-_prevTime)*_maxBright/_fadeOutTime;
        // if fadeOutTime has changed we may have gone above maxBright in negative delta.
        fadeValue = max(fadeValue, 0);
        analogWrite(_ledPin,fadeValue);
      }
    }
  }
}
