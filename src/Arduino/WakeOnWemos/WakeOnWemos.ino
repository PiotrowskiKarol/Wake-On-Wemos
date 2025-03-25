#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#import "secret_var.h"

const int optoPowerPin = D2; //optocoupler pin for power button
const int optoResetPin = D1; //optocoupler pin for reset button
const int powerLedPin = A0; //optocoupler pin for power LED
const int buzzerPin = D8; //optocoupler pin for buzzer
const int serial = 115200; //Serial Baudrate
const int port = 80;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
ESP8266WebServer server(port);

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("WEMOS MAC addres: "); // will IP address on Serial Monitor
  Serial.println(WiFi.macAddress()); 
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  
  digitalWrite(buzzerPin, HIGH);
  delay(200);
  digitalWrite(buzzerPin, LOW);
  delay(200);
  digitalWrite(buzzerPin, HIGH);
  delay(200);
  digitalWrite(buzzerPin, LOW);
  delay(200);
  digitalWrite(buzzerPin, HIGH);
  delay(200);
  digitalWrite(buzzerPin, LOW);
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi sucessfully.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi, trying to connect...");
  Serial.println(event.reason);
  Serial.println();
  WiFi.disconnect();
  WiFi.begin(ssid, password);
}

void handleRequestOn(){
  digitalWrite(optoPowerPin, HIGH);
  digitalWrite(buzzerPin, HIGH);
  delay(200);
  digitalWrite(optoPowerPin, LOW);
  digitalWrite(buzzerPin, LOW);
  server.send(200, "application/json", "{ \"status\": \"OK\", \"message\": \"turned on\"}");
}

void handleRequestOff(){
  digitalWrite(optoPowerPin, HIGH);
  digitalWrite(buzzerPin, HIGH);
  delay(200);
  digitalWrite(optoPowerPin, LOW);
  digitalWrite(buzzerPin, LOW);
  server.send(200, "application/json", "{ \"status\": \"OK\", \"message\": \"turned off\"}");
}

void handleRequestForceOff(){
  digitalWrite(optoPowerPin, HIGH);
  digitalWrite(buzzerPin, HIGH);
  delay(600);
  digitalWrite(optoPowerPin, LOW);
  digitalWrite(buzzerPin, LOW);
  server.send(200, "application/json", "{ \"status\": \"OK\", \"message\": \"forced turned off\"}");
}

void handleRequestReset(){
  digitalWrite(optoResetPin, HIGH);
  digitalWrite(buzzerPin, HIGH);
  delay(200);
  digitalWrite(optoResetPin, LOW);
  digitalWrite(buzzerPin, LOW);
  server.send(200, "application/json", "{ \"status\": \"OK\", \"message\": \"restart\"}");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  pinMode(optoPowerPin, OUTPUT);
  pinMode(optoResetPin, OUTPUT);
  pinMode(powerLedPin, INPUT);

  //Register event handlers
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
   
  initWiFi();
  server.on("/on", handleRequestOn);
  server.on("/off", handleRequestOn);
  server.on("/forceOff", handleRequestForceOff);
  server.on("/reset", handleRequestReset);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  server.handleClient();
}