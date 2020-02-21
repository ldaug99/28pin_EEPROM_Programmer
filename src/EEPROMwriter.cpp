#include <Arduino.h>
#include "EEPROMwriter.h"
//************************************************************************************
// Public functions
//************************************************************************************
// Setup pinout
EEPROMwriter::EEPROMwriter() {
    setupHW();
}
// Setup pinout, non default paramaters
EEPROMwriter::EEPROMwriter(uint8_t io0, uint8_t io1, uint8_t io2, uint8_t io3, uint8_t io4, uint8_t io5, uint8_t io6, uint8_t io7, uint8_t Eread, uint8_t Ewrite, uint8_t Eenable, uint8_t soutpin, uint8_t sclockpin) : EEPROM_pin({io0, io1, io2, io3, io4, io5, io6, io7}), EEPROMread(Eread), EEPROMwrite(Ewrite), EEPROMenable(Eenable), shiftOutPin(soutpin), shiftClockPin(sclockpin) {
    setupHW();
}
// Write to EEPROM
void EEPROMwriter::writeEEPROM(uint16_t address, uint8_t data) {
    shiftOutAddress(address);
    setEEPROM(OUTPUT);
    setData(data);
    pulseWrite();
    setEEPROM(INPUT);
}
// Read from EEPROM
uint8_t EEPROMwriter::readEEPROM(uint16_t address) {
    shiftOutAddress(address);
    setEEPROM(INPUT);
    return getData();
}
//************************************************************************************
// Private functions
//************************************************************************************
// Setup EEPROM and 74HC164
void EEPROMwriter::setupHW() {
    setupEEPROM();
    setupShift();
}
// Setup EEPROM read and write pins
void EEPROMwriter::setupEEPROM() {
    pinMode(ER, OUTPUT);
    digitalWrite(ER, LOW);
    digitalWrite(EW, HIGH);
    pinMode(EW, OUTPUT);
    digitalWrite(CE, LOW);
    pinMode(CE, OUTPUT);
    setEEPROM(INPUT);
}
// Setup 74HC164 out and clock pin
void EEPROMwriter::setupShift() {
    digitalWrite(SO, LOW);
    pinMode(SO, OUTPUT);
    digitalWrite(SC, LOW);
    pinMode(SC, OUTPUT);
    shiftOut(SO, SC, MSBFIRST, 0);
    shiftOut(SO, SC, MSBFIRST, 0);
}
// Set data pins mode for EEPROM, input or output (read or write data)
void EEPROMwriter::setEEPROM(uint8_t mode) {
    if (EEPROM_mode != mode) {
        EEPROM_mode = mode;
        if (mode == OUTPUT) {
            digitalWrite(ER, HIGH);
        } else {
            digitalWrite(ER, LOW);
        }
        for (uint8_t i = 0; i < EEPROM_pins; i++) {
            pinMode(EEPROM_pin[i], mode);
        }
    }
    delayMicroseconds(pinDelay);
}
// Shift out address to 74HC164
void EEPROMwriter::shiftOutAddress(uint16_t address) {
    byte lb_addr = (byte) address;
    byte hb_addr = (byte) (address >> 8);
    shiftOut(SO, SC, MSBFIRST, hb_addr);
    shiftOut(SO, SC, MSBFIRST, lb_addr);
}
// Get data
uint8_t EEPROMwriter::getData() {
    uint8_t data = 0;
    for (uint8_t i = 0; i < EEPROM_pins; i ++) {
        data = data + (digitalRead(EEPROM_pin[i]) << i);
    }
    return data;
}
// Set data
void EEPROMwriter::setData(uint8_t data) {
    for (uint8_t i = 0; i < EEPROM_pins; i ++) {
        digitalWrite(EEPROM_pin[i], bitRead(data, i));
    }
    delayMicroseconds(pinDelay);
}
// Pulse write
void EEPROMwriter::pulseWrite() {
    digitalWrite(EW, LOW);
    delayMicroseconds(pulseTime); // Wrtie pulse should be 100 ns, function works accurately down to 3 micro seconds
    digitalWrite(EW, HIGH);
    delayMicroseconds(pinDelay);
}
//************************************************************************************