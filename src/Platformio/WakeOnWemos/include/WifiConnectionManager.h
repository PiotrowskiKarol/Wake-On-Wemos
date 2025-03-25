#ifndef WIFI_CONNECTION_MANAGER_H
#define WIFI_CONNECTION_MANAGER_H

#include <ESP8266WiFi.h>

class WifiConnectionManager {
private:
    String _ssid;
    String _password;
    unsigned long _lastReconnectAttempt = 0;
    const unsigned long _reconnectInterval = 10000; // 10 seconds between reconnect attempts
    
    void reconnect();

public:
    WifiConnectionManager();
    
    // Set WiFi credentials
    void setCredentials(const String& ssid, const String& password);
    
    // Initialize WiFi connection
    void begin();
    
    // Check and maintain WiFi connection
    void loop();
    
    // Check if WiFi is connected
    bool isConnected();
    
    // Get current IP address
    String getIPAddress();
};

#endif // WIFI_CONNECTION_MANAGER_H