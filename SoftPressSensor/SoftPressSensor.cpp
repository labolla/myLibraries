/*
  SoftPressSensor.h - library to use "Soft Pressure Sensor"
  Created by LaBolla, January 19 2016
  https://github.com/labolla
  Released into the public domain.

  "Soft Pressure Sensor" is an handy craft analog pressure sensor
  built with Velostat material rolled in 'M' and envelopped by
  a textile.
*/

#include "Arduino.h"
#include "SoftPressSensor.h"

#define _DEBUG_SOFT_PRESS_SENSOR

// empiric values; they may adjusted for each "Soft Pressure Sensor"
// MIN_MAX minimum delta "press sensor" is considered calibrated and starting tracking run-time "min/max"
#define PEAK_2_PEAK 20
// minumum analog pressed value to consider the soft button been pressed
#define ACTIVE_DELTA  20
// consecutive blocking condition to reset real-time min
#define BLOCKING_THRESHOLD 4
// consecutive 'inactive' (i.e. not enough pressure) condition to reset real-time min
#define INACTIVE_THRESHOLD 10

SoftPressSensor::SoftPressSensor(int pin)
{
#ifdef _DEBUG_SOFT_PRESS_SENSOR
  Serial.begin(9600);
#endif // _DEBUG_SOFT_PRESS_SENSOR
  pinMode(pin, INPUT_PULLUP);
  _pin = pin;

  _minVal = _absMinVal =2000;
  _maxVal = _absMaxVal= 0;

  _soft_press_ma = 0;

  _inactive_cnt=0;
  _is_blocking_cnt = 0;

  _prev_press_val = 0;
  _press_sensor_calibrated = false;
}

/*
   general API to read the pressure value
   return value: analog value between min/mx range or
   NOT_CALIBRATED indicating not enough pression done on soft Button

   note: range is changed dynamically but need to be a minimum large to operate.
*/

int SoftPressSensor::read(void)
{
  // read RAW value from pressure sensor
  _soft_press = analogRead(_pin);

  // start track absolute Max/Min
  _absMaxVal = max(_soft_press, _absMaxVal);
  _absMinVal = min(_soft_press, _absMinVal);

  // compute moving average on a 8 sample window (to avoid noisy spike)
  _soft_press_ma = (_soft_press_ma * 7 + _soft_press) / 8;

  // condition to have an acceptable starting calibrated min_max range (checked till calibrated)
  // from this point start to track run-time min/max to adjust in real-time the range
  if (_press_sensor_calibrated == false)
  {
    if (((_soft_press_ma - _absMinVal) > PEAK_2_PEAK/2) &&
        ((_absMaxVal - _soft_press_ma) > PEAK_2_PEAK/2))
    {
        _maxVal = max(_soft_press_ma, _maxVal);
        _minVal = min(_soft_press_ma, _minVal);
        _press_sensor_calibrated = true;
    }
    return NOT_CALIBRATED;
  }

  // sensor is calibrated keep tracking real-time max-min to adjust range
  _maxVal = max(_soft_press_ma, _maxVal);
  _minVal = min(_soft_press_ma, _minVal);
  // compute pression value as delta between current min and moving average value
  _press_val = _soft_press_ma - _minVal;

  #ifdef _DEBUG_SOFT_PRESS_SENSOR
  Serial.print(_soft_press);
  Serial.print('\t');
  Serial.print(_soft_press_ma);
  Serial.print('\t');
  Serial.print(_press_val);
  Serial.print('\t');
  Serial.print(_minVal);
  Serial.print('\t');
  Serial.println(_maxVal);
  #endif // _DEBUG_SOFT_PRESS_SENSOR

  // is it considered an real (active) pressure?
  if (_press_val > ACTIVE_DELTA)
  {
    // reset counter to detect not pressure
    _inactive_cnt = 0;

    // check blocking condition indicatig pressure sensor is not coming back to relaxed state
    // (in relaxed state min Val is read)
    if ((_press_val == _prev_press_val) &&  (_press_val > ACTIVE_DELTA*2))
    {
      _is_blocking_cnt++;
      if (_is_blocking_cnt == BLOCKING_THRESHOLD )
      {
        _minVal = _soft_press_ma;
      }
    }
  }
  else
  {
    //reset min if not pressure for a while
    _inactive_cnt++;
    if (_inactive_cnt == INACTIVE_THRESHOLD)
    {
      _inactive_cnt = 0;
      _minVal = _soft_press_ma;
    }
  }

  _prev_press_val = _press_val;

  return _press_val;
}

// API to get current range for Pressure Sensor
int SoftPressSensor::getRange(void)
{
  if (_maxVal > _minVal)
    return (_maxVal - _minVal);
  else
    return 0;
}





