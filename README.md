# DIY multicopter lighting project

Over the past few weeks I have been developing a lighting setup for my quadcopter that is 100% custom and can perform many different lighting sequences. I also required it to be powered straight from a 3S lipo balance plug, controlled remotely using the aux channel on my transmitter and have a FTDI connector to update code.

For any one that is interested in making their own I hope this schematic, code, PCB design and photos will help out and save you hours of work.

At first I tested the code and design using an Arduino UNO and a breadboard, then when I was happy with how that performed I designed the schematic and PCB to use the ATmega328P microcontroller standalone to reduce the weight and size. One important thing to note is that I wanted to eliminate the external clock and use the 8 MHz internal clock, instructions on how to do that is here http://arduino.cc/en/Tutorial/ArduinoToBreadboard

### Build of materials:

* ATmega328P Microcontroller
* ULN2003AN Transistor package
* LM7805 Voltage Regulator
* 0.33uf Capacitor
* 0.1uf Capacitors (x5)
* 1k Resistor
* 10K Resistor
* Tact Switch
* 3S 4Pin JST-XH Socket
* Male header pins (optional)
