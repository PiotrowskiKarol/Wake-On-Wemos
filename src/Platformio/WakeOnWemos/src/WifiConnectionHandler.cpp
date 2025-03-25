#include "WiFiConnectionHandler.h"

// Initialize static members
WiFiConnectionHandler* WiFiConnectionHandler::_instance = nullptr;

WiFiConnectionHandler::WiFiConnectionHandler(WiFiCredentialsManager& credentialsManager)
    : _credentialsManager(credentialsManager) 
{
    _instance = this;
}

void WiFiConnectionHandler::begin() {
    // Set up WiFi event handlers
    WiFi.onStationModeGotIP(onWifiConnected);
    WiFi.onStationModeDisconnected(onWifiDisconnected);
}

void WiFiConnectionHandler::onWifiConnected(const WiFiEventStationModeGotIP &event) {
    if (!_instance) return;

    Serial.println("\nConnected to Wi-Fi successfully.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI());

    // Reset reconnect attempts on successful connection
    _instance->_reconnectAttempts = 0;
}

void WiFiConnectionHandler::onWifiDisconnected(const WiFiEventStationModeDisconnected &event) {
    if (!_instance) return;

    Serial.println("\nWiFi Disconnected.");
    Serial.print("Reason: ");
    Serial.println(event.reason);

    // Attempt reconnection
    _instance->reconnect();
}

bool WiFiConnectionHandler::connect() {
    String ssid, password;
    
    // Try to load stored credentials
    if (!_credentialsManager.loadCredentials(ssid, password)) {
        Serial.println("No WiFi credentials available");
        return false;
    }

    // Begin connection
    beginConnection(ssid, password);
    return true;
}

void WiFiConnectionHandler::beginConnection(const String& ssid, const String& password) {
    // Disconnect first to clear any previous connection state
    WiFi.disconnect();
    
    // Begin connection
    WiFi.begin(ssid.c_str(), password.c_str());
}

void WiFiConnectionHandler::reconnect() {
    // Check if enough time has passed since last reconnect attempt
    unsigned long currentTime = millis();
    
    // Prevent too frequent reconnection attempts
    if (currentTime - _lastReconnectAttempt < RECONNECT_INTERVAL) {
        return;
    }

    // Check if max reconnect attempts reached
    if (_reconnectAttempts >= MAX_RECONNECT_ATTEMPTS) {
        Serial.println("Max reconnection attempts reached. Stopping.");
        return;
    }

    // Increment reconnect attempts
    _reconnectAttempts++;

    Serial.print("Reconnection attempt ");
    Serial.print(_reconnectAttempts);
    Serial.println(" of " + String(MAX_RECONNECT_ATTEMPTS));

    // Attempt to reconnect
    connect();

    // Update last reconnect time
    _lastReconnectAttempt = currentTime;
}

bool WiFiConnectionHandler::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}