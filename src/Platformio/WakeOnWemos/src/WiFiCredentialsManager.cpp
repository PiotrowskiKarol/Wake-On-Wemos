#include "WiFiCredentialsManager.h"

WiFiCredentialsManager::WiFiCredentialsManager(
    int ssidAddr, 
    int passwordAddr, 
    int magicAddr
) : 
    _ssidAddr(ssidAddr), 
    _passwordAddr(passwordAddr), 
    _magicAddr(magicAddr) 
{}

bool WiFiCredentialsManager::saveCredentials(const String& ssid, const String& password) {
    // Validate input
    if (ssid.length() > 63 || password.length() > 63) {
        Serial.println("Error: SSID or Password too long");
        return false;
    }

    // Write SSID and Password
    _eepromManager.writeString(_ssidAddr, ssid);
    _eepromManager.writeString(_passwordAddr, password);

    // Write magic number to indicate valid credentials
    _eepromManager.writeByte(_magicAddr, MAGIC_NUMBER);
    
    // Commit changes
    _eepromManager.commit();
    
    Serial.println("WiFi credentials stored successfully");
    return true;
}

bool WiFiCredentialsManager::loadCredentials(String& ssid, String& password) {
    // Check if EEPROM has valid data
    if (_eepromManager.readByte(_magicAddr) != MAGIC_NUMBER) {
        Serial.println("No stored WiFi credentials found");
        return false;
    }

    // Read SSID and Password
    ssid = _eepromManager.readString(_ssidAddr);
    password = _eepromManager.readString(_passwordAddr);

    Serial.println("WiFi credentials retrieved:");
    Serial.print("SSID: "); Serial.println(ssid);
    return true;
}

bool WiFiCredentialsManager::hasCredentials() {
    return _eepromManager.readByte(_magicAddr) == MAGIC_NUMBER;
}

void WiFiCredentialsManager::clearCredentials() {
    // Clear magic number to invalidate stored credentials
    _eepromManager.writeByte(_magicAddr, 0);
    _eepromManager.commit();
    
    Serial.println("WiFi credentials cleared");
}

void WiFiCredentialsManager::configureWifi() {
    Serial.println("Waiting 5s for setting credentials");
    delay(5000);

    if (!Serial.available()) {
        Serial.println("No new credentials, skipping configuration");
        return;
    }

    String inputString = Serial.readString();
    inputString.trim();
    Serial.println("Setting new credentials");

    if (inputString.startsWith("SET:")) {
        int firstColon = inputString.indexOf(':', 4);
        if (firstColon != -1) {
            String ssid = inputString.substring(4, firstColon);
            String password = inputString.substring(firstColon + 1);
            
            if (saveCredentials(ssid, password)) {
                Serial.println("Credentials saved successfully");
            } else {
                Serial.println("Failed to save credentials");
            }
        } else {
            Serial.println("Invalid SET command format");
        }
    } else {
        Serial.println("Invalid command. Use format: SET:SSID:PASSWORD");
    }
}