/*
  SoftPressSensor.h - library to use "Soft Pressure Sensor"
  Created by LaBolla, January 19 2016
  https://github.com/labolla
  Released into the public domain.

  "Soft Pressure Sensor" is an handy craft analog pressure sensor
  built with Velostat material rolled in 'M' and envelopped by
  a textile.
*/

#ifndef SoftPressSensor_h
#define SoftPressSensor_h

#include "Arduino.h"

#define NOT_CALIBRATED  0xFFFF

class SoftPressSensor
{
  public:
    SoftPressSensor(int pin);
    int read();
    int getRange();
  private:

    //VARIABLES
    // pressure sensor pin
    int _pin;

    // pressure value range (max/min) used to calibrate sensor and map value into a range.
    int _minVal;
    int _maxVal;
    int _absMaxVal;
    int _absMinVal;

    //"press sensor" analog value: current and moving averaged one
    int _soft_press;
    int _soft_press_ma;

    // press values: delta between MA soft press value and min value
    int _press_val;
    int _prev_press_val;   // to detect if changed or not (to avoid sensor stuck)

    // inactive (no pression detected) and blocking (no change in pression) counters
    int _inactive_cnt;
    int _is_blocking_cnt;

    // flag indicating ensor being calibrated (sensor requires at least a press action to have a valid press range)
    bool _press_sensor_calibrated;
};


#endif // SoftPressSensor_h
