This example, for Teensy 3.5 & 3.6, is modified from DataLogger from https://github.com/greiman/SdFat/blob/master/SdFat/examples/dataLogger/dataLogger.ino

# Requirements

* microSD card - formatted in FAT16 or FAT32, in the on-board microSD card holder
* SdFat library - https://github.com/greiman/SdFat-beta/tree/master/SdFat, installed in your Arduino IDE software.
    * Copy the directory ```SdFat``` onto ```<Arduino>\hardware\teensy\avr\libraries\```

Teensy 3.5 & 3.6 use SDIO peripheral to interface micorSD via the on-board microSD holder.

Chip select is connected to pin 10.
