//************************************************************************************
// Include libraries
#include <Arduino.h>
#include "EEPROMwriter.h"
//************************************************************************************
// Global variables
uint32_t baudRate = 115200;
// Initialize instance
EEPROMwriter Ewriter;
//************************************************************************************
// Functions
// Setup serial connection
void setupSerial(uint32_t baudRate) {
    Serial.begin(baudRate);
    Serial.print("EEPROM programmer started at baud rate ");
    Serial.println(String(baudRate));
    Serial.flush();
}
// Write address and data to serial prompt
void writeToSerial(uint16_t address, uint8_t data) {
    Serial.print(address);
    Serial.print(",");
    Serial.println(data); // Prints data and \n character
    Serial.flush();
}
// Process serial command
void processInput(String input) {
    int16_t LFindex = input.indexOf('\n');
    if (LFindex > 0) { // Remove newline character
        input = input.substring(0, LFindex);
    }
    int16_t SEPindex = input.indexOf(","); // Check for comma seperator
    if (SEPindex > 0) { // Write command
        uint16_t address = (uint16_t) (input.substring(0, SEPindex)).toInt();
        uint8_t data = (uint8_t) (input.substring(SEPindex + 1)).toInt();
        Ewriter.writeEEPROM(address, data);
        Serial.println("OK");
    } else { // Read command
        uint16_t address = (uint16_t) input.toInt();
        uint8_t data = Ewriter.readEEPROM(address);
        writeToSerial(address, data);
    }
}
// Read a string from the serial port
void readStringFromSerial() {
    if (Serial.available() > 0) {
		String input = Serial.readStringUntil('\n');
		processInput(input);
    }
}
//************************************************************************************
// Setup
void setup() {
    setupSerial(baudRate);
}
//************************************************************************************
// Main loop
void loop() {
	readStringFromSerial();
}
//************************************************************************************