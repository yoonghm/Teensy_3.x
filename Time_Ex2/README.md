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

This example for Teensy 3.x.

It uses Teensy3Clock - a global teensy3_clock_class object:

- Retrieve date and time from compile host into source code
- The time is periodically written to a flash memory within Teensy 3.x.
- If the RTC peripheral in Teensy 3.x is not powered, on power reset the time starts from the compile time.
- Otherwise the time continues from the time in the flash.

## Requirements

- This example uses TimeLib library comes with Teensyduino library
- Run Serial monitor window to see date time string