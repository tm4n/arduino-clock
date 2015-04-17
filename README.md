arduino-clock
=============

Setable two-button clock with arduino uno, ds1302 clock chip and tm1637 display controller.
It also keeps and displays the date. Copyright to the included libraries belongs to their owners. The tm1637 library has been modified to allow other glyphs to be displayed.

"temp_clock" code also has a dht22 sensor and can display the temperature and humidity. I removed that code from "clock" because the sensor readings were not reliable in the casing I build for the clock.

"stop_clock" adds a stopwatch, but viewing date will no longer be available. It needs a slightly modified ds1302 library, to prevent a name conflict with Time, since it also uses the arduino time library that can be found in the library manager.

Schematics will be added. I have build this clock into an small stained glass. Buttons and power connector are drilled into the plastic top. It uses a standalone arduino, which consists of the atmega328p microcontroller (loaded with the arduino bootloader) and a simple 7805 power supply circuit. Tutorials for a standalone arduino can be easily foud online.

Pictures:

![Clock front picture](https://cloud.githubusercontent.com/assets/3649289/7209828/86884f20-e54c-11e4-9202-b816b574b5b2.jpg)
![Clock side picture](https://cloud.githubusercontent.com/assets/3649289/7209829/868bff6c-e54c-11e4-89f7-7bba995ef376.jpg)
![Clock on breadboard picture](https://cloud.githubusercontent.com/assets/3649289/7209827/86829170-e54c-11e4-8053-dde4591e71d2.jpg)
