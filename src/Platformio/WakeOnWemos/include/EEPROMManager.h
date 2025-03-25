#ifndef EEPROM_MANAGER_H
#define EEPROM_MANAGER_H

#include <Arduino.h>

class EEPROMManager {
public:
    EEPROMManager(size_t size = 256);
    
    // Write string to a specific address
    bool writeString(int addr, const String& str);
    
    // Read string from a specific address
    String readString(int addr);
    
    // Write a single byte (for magic number or flags)
    void writeByte(int addr, uint8_t value);
    
    // Read a single byte
    uint8_t readByte(int addr);
    
    // Commit changes to EEPROM
    void commit();

private:
    size_t _eepromSize;
};

#endif