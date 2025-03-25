#ifndef WIFI_CREDENTIALS_MANAGER_H
#define WIFI_CREDENTIALS_MANAGER_H

#include <Arduino.h>
#include "EEPROMManager.h"

class WiFiCredentialsManager {
public:
    // Constructor with configurable EEPROM addresses
    WiFiCredentialsManager(
        int ssidAddr = 0, 
        int passwordAddr = 64, 
        int magicAddr = 200
    );

    // Store WiFi credentials in EEPROM
    bool saveCredentials(const String& ssid, const String& password);
    
    // Retrieve stored WiFi credentials
    bool loadCredentials(String& ssid, String& password);
    
    // Check if credentials are stored
    bool hasCredentials();
    
    // Clear stored credentials
    void clearCredentials();

    void configureWifi();

private:
    EEPROMManager _eepromManager;
    int _ssidAddr;
    int _passwordAddr;
    int _magicAddr;
    
    static const uint8_t MAGIC_NUMBER = 0xAA;
};

#endif