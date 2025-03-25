#ifndef WIFI_CONNECTION_HANDLER_H
#define WIFI_CONNECTION_HANDLER_H

#include <ESP8266WiFi.h>
#include "WiFiCredentialsManager.h"

class WiFiConnectionHandler {
public:
    WiFiConnectionHandler(WiFiCredentialsManager& credentialsManager);

    // Initialize WiFi connection events
    void begin();

    // Attempt to connect to WiFi
    bool connect();

    // Reconnect if disconnected
    void reconnect();

    // Check connection status
    bool isConnected();

private:
    // WiFi Event Handlers
    static void onWifiConnected(const WiFiEventStationModeGotIP &event);
    static void onWifiDisconnected(const WiFiEventStationModeDisconnected &event);

    // Static pointer to allow access in static methods
    static WiFiConnectionHandler* _instance;

    WiFiCredentialsManager& _credentialsManager;

    // Connection tracking
    unsigned long _lastReconnectAttempt = 0;
    const unsigned long RECONNECT_INTERVAL = 10000; // 10 seconds between reconnect attempts
    const int MAX_RECONNECT_ATTEMPTS = 5;
    int _reconnectAttempts = 0;

    // Internal connection method
    void beginConnection(const String& ssid, const String& password);
};

#endif