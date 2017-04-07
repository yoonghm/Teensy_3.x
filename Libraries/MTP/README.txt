The libraries provide MTP interface to onboard microSD card in a Teensy 3.5/3.6 board via a connected host PC.

This library requires the following changes
1.  SdFat libraries - https://github.com/greiman/SdFat-beta/tree/master/SdFat
2.  <Arduino_path>/hardware/teensy/avr/cores/teensy3/usb_desc.c
3.  <Arduino_path>/hardware/teensy/avr/cores/teensy3/usb_dev.c
4.  <Arduino_path>/hardware/boards.txt

See the history in https://forum.pjrc.com/threads/43050-MTP-Responder-Contribution?p=138772