// ensure this library description is only included once
#ifndef EEPROMwriter_h
#define EEPROMwriter_h
// Simple serial EEPROM reader/writer
// To shift register are used to set address
// Data is read using digital pins
// Global definitions
// EEPROM pinout
#define ED0 4 // EEPROM data pins, ED0 = I/O0 on EEPROM
#define ED1 5
#define ED2 6
#define ED3 7
#define ED4 8
#define ED5 9
#define ED6 10
#define ED7 11
#define EEPROM_pins 8
#define ER A5 // EEPROM read enable
#define EW A4 // EEPROM write enable
#define CE A3 // EEPROM chip enable
// Serial shift definitions
#define SO 3 // Shift out
#define SC 2 // Shift clock
#define pulseTime 5 // Write pulse should be 100 ns, function works accurately down to 3 micro seconds
#define pinDelay 3

// Include Arduino
#include <Arduino.h>
// Class
class EEPROMwriter {
    public:
        // Class initializer
        EEPROMwriter();
        EEPROMwriter(uint8_t io0, uint8_t io1, uint8_t io2, uint8_t io3, uint8_t io4, uint8_t io5, uint8_t io6, uint8_t io7, uint8_t Eread, uint8_t Ewrite, uint8_t Eenable, uint8_t soutpin, uint8_t sclockpin);

        uint8_t readEEPROM(uint16_t address);
        void writeEEPROM(uint16_t address, uint8_t data);

    private:
        const uint8_t EEPROM_pin[EEPROM_pins] = {ED0, ED1, ED2, ED3, ED4, ED5, ED6, ED7}; // EEPROM data pins
        uint8_t EEPROM_mode;
		const uint8_t EEPROMread = A5; // EEPROM read enable
		const uint8_t EEPROMwrite = A4; // EEPROM write enable
		const uint8_t EEPROMenable = A3; // EEPROM chip enable

		const uint8_t shiftOutPin = SO; // Shift out pin
		const uint8_t shiftClockPin = SC; // Clock pin

        void setupHW();
        void setupEEPROM();
        void setupShift();

        void setEEPROM(uint8_t mode);

        void setData(uint8_t data);
        uint8_t getData();
        void shiftOutAddress(uint16_t address);
        void pulseWrite();
};
#endif