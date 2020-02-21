# 28pin_EEPROM_Programmer
 
Simple Arduino EEPROM reader and writer, build using a Arduino Nano V3 and two serial shift register (74HC164).

The Python script provides a simple interface for the Arduino project.

The Arduino interface:
	- To write, send string via COM port: "address,data\n", where address and data designates desired address (uint15) and data (uint8).
	- Example: "4982,243\n"
	- To read, send string via COM port: "address\n", where address designates desired address (uint15).
	- Example: "2001\n"