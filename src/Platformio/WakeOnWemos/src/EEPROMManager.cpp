#include "EEPROMManager.h"
#include <EEPROM.h>

EEPROMManager::EEPROMManager(size_t size) : _eepromSize(size) {
    EEPROM.begin(_eepromSize);
}

bool EEPROMManager::writeString(int addr, const String& str) {
    // Validate address and string length
    if (addr < 0 || addr + str.length() >= _eepromSize) {
        return false;
    }

    // Write string to EEPROM
    for (int i = 0; i < str.length(); i++) {
        EEPROM.write(addr + i, str[i]);
    }
    
    // Null-terminate the string
    EEPROM.write(addr + str.length(), '\0');
    
    return true;
}

String EEPROMManager::readString(int addr) {
    char buffer[64];  // Adjust size as needed
    int i = 0;
    char c;
    
    // Read until null terminator or buffer is full
    while ((c = EEPROM.read(addr + i)) != '\0' && i < 63) {
        buffer[i] = c;
        i++;
    }
    buffer[i] = '\0';
    
    return String(buffer);
}

void EEPROMManager::writeByte(int addr, uint8_t value) {
    if (addr >= 0 && addr < _eepromSize) {
        EEPROM.write(addr, value);
    }
}

uint8_t EEPROMManager::readByte(int addr) {
    if (addr >= 0 && addr < _eepromSize) {
        return EEPROM.read(addr);
    }
    return 0;
}

void EEPROMManager::commit() {
    EEPROM.commit();
}