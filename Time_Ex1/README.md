Note: Standard Arduino Time library no longer works for Arduino IDE software 1.6.1.
Instead the standard recommends to use Time library from Teensy 3.x
See http://playground.arduino.cc/Code/time

To use Teensy Time (aka TimeLib) library, just change 

```
#include <Time.h>
```

to

```
#include <TimeLib.h>
```

This example is for Teensy 3.x.
SetTime() is used to set the time of the RTC peripheral.  On restart, the time always starts from this time.

## Requirements

- This example uses TimeLib library comes with Teensyduino library
- Run Serial monitor window to see date time string