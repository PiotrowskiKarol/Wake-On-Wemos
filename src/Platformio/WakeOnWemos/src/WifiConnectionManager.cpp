#include "WifiConnectionManager.h"

WifiConnectionManager::WifiConnectionManager() {
    // Disable WiFi power saving mode for more stable connection
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
}

void WifiConnectionManager::setCredentials(const String& ssid, const String& password) {
    _ssid = ssid;
    _password = password;
}

void WifiConnectionManager::begin() {
    // Check if credentials are set
    if (_ssid.isEmpty() || _password.isEmpty()) {
        Serial.println("WiFi credentials not set!");
        return;
    }

    // Configure WiFi station mode
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid.c_str(), _password.c_str());

    // Wait for connection with timeout
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");

        // Timeout after 10 seconds
        if (millis() - startAttemptTime > 10000) {
            Serial.println("\nWiFi connection failed!");
            break;
        }
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    }
}

void WifiConnectionManager::loop() {
    // Check connection status
    if (WiFi.status() != WL_CONNECTED) {
        // Attempt reconnection at intervals
        unsigned long currentMillis = millis();
        if (currentMillis - _lastReconnectAttempt >= _reconnectInterval) {
            _lastReconnectAttempt = currentMillis;
            reconnect();
        }
    }
}

void WifiConnectionManager::reconnect() {
    Serial.println("WiFi disconnected. Attempting to reconnect...");
    
    // Attempt to reconnect
    WiFi.reconnect();

    // Wait a short time to establish connection
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");

        // Timeout after 5 seconds
        if (millis() - startAttemptTime > 10000) {
            Serial.println("\nReconnection failed!");
            break;
        }
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi reconnected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.print("Not good status: ");
        Serial.println(WiFi.status());
    }
}

bool WifiConnectionManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String WifiConnectionManager::getIPAddress() {
    if (isConnected()) {
        return WiFi.localIP().toString();
    }
    return "Not Connected";
}