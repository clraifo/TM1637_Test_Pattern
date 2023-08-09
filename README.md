#THIS IS ENTIRELY UNTESTED AS OF THIS WRITING

# TM1637_Test_Pattern

## TM1637 LED Module Tester

This Arduino project provides a comprehensive testing pattern for TM1637 4-digit 7-segment LED modules. It's designed to quickly assess the functionality of each segment, character representation, and overall display capabilities of the module.

### Features

- Actuates each segment individually
- Displays a subset of the lowercase English alphabet
- Cycles through numbers 0-9
- Blinks the colon between positions 2 and 3
- Blinks the entire module
- Resets the entire module to LOW
- Entire test pattern completes in approximately 10 seconds

### Hardware Requirements

- Arduino Uno R3 (or compatible board)
- TM1637 4-digit 7-segment LED module
- Jumper wires

### Wiring

1. Connect the VCC of the TM1637 module to the 5V of the Arduino
2. Connect the GND of the TM1637 to the GND of the Arduino
3. Connect the CLK of the TM1637 to digital pin 2 on the Arduino
4. Connect the DIO of the TM1637 to digital pin 3 on the Arduino

### Usage

1. Open the provided `.ino` file in the Arduino IDE
2. Ensure your Arduino is connected and the correct board and port are selected
3. Upload the sketch to your Arduino
4. Power on the Arduino and observe the TM1637 as it goes through the test pattern

### Troubleshooting

- If the TM1637 doesn't light up, check all connections, especially VCC and GND
- Ensure that the CLK and DIO connections are correct
- If only certain segments or characters don't display correctly, the TM1637 module might be faulty

### License

This project is open source and available under the MIT License.
