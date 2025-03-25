#include <ESP8266WiFi.h>
#import "secret_var.h"

// Replace with your network credentials
const int serial = 115200; //Serial Baudrate
const int port = 8085; //wemos port (defalult 80)

const int optoPowerPin = D2; //optocoupler pin for power button
const int optoResetPin = D1; //optocoupler pin for reset button
const int powerLedPin = A0; //optocoupler pin for power LED
const int buzzerPin = D8; //optocoupler pin for buzzer

unsigned int orderRequests = 0;
int powerLedValue = 0;
const char* wifiStatus;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
WiFiServer server(port);

bool oneDisc = false;

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  wifiStatus = "CONNECTED";
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  wifiStatus = "CONNECTED";
  Serial.println("Connected to Wi-Fi sucessfully.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  if(wifiStatus != "CONNECTING") {
    wifiStatus = "CONNECTING";
    WiFi.begin(ssid, password);
  }
  
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  if(wifiStatus != "CONNECTING") {
    wifiStatus = "DISCONNECTED";
    Serial.println("Disconnected from Wi-Fi");
    Serial.print("Disconnect reason: ");
    Serial.println(event.reason);
    Serial.println();
    WiFi.disconnect();
    reconnect();
  } 
  
}

void disconnectFromWifi() {
  WiFi.disconnect();
}

void displayHtmlPage(WiFiClient client, int powerLedValue) {
  String html ="<!DOCTYPE html> <html> <head> <link rel=\"preconnect\" href=\"https://fonts.googleapis.com\" /> <link rel=\"preconnect\" href=\"https://fonts.gstatic.com\" crossorigin /> <link href=\"https://fonts.googleapis.com/css2?family=Roboto:wghtx300&display=swap\" rel=\"stylesheet\" /> <meta name=\"viewport\" content=\"width=device-width\" ; initial-scale=\"1.0\" /> <style> html { height: 100%; } body { background: linear-gradient( 45deg, rgb(243, 244, 228) 20%, rgb(212, 219, 182) 80% ); color: red; display: flex; justify-content: center; margin-top: 40px; font-family: 'Roboto', sans-serif; font-weight: bold; } div.upper { font-size: 1.2rem; height: 30px; } div.middle { background-color: rgb(222, 219, 189); /* box-shadow: -6px 6px 20px 4px rgb(61, 61, 69); */ border-radius: 10px; margin: 0px 10px; height: 3px; } div.lower { padding: 5px; flex-direction: row; justify-content: flex-start; } div.lower-left { margin-left: 10px; flex-direction: column; background-color: rgb(222, 204, 139); box-shadow: -3px 3px 10px 0px rgb(173, 173, 162); border-radius: 8px; width: 68px; height: 68px; } div.lower-right { line-height: 13px; flex-direction: column; align-items: start; } div.main { background-color: rgb(253, 253, 253); border: 2px solid rgb(197, 171, 116); color: black; width: 300px; border-radius: 10px; overflow: hidden; box-shadow: -2px 2px 15px 1px rgb(160, 161, 157); } div.form { flex-direction: column; margin-top: 8px; margin-bottom: 6px; } a.status { padding-top: 14px; font-size: 24px; } a.label { position: relative; top: -12px; font-size: 0.85rem; color: white; font-weight: 200; } .center-flex { display: flex; align-items: center; justify-content: center; gap: 10px; } .button { cursor: pointer; font-weight: bold; background-color: rgb(222, 204, 139); padding: 0 20px; height: 24px; border-radius: 30px; border: none; box-shadow: -1px 2px 8px 0px rgb(137, 136, 129); } .select { width: 207px; text-align: center; border: 1px solid rgb(199, 195, 185); border-radius: 8px; height: 25px; } .input { width: 200px; text-align: center; border: 1px solid rgb(199, 195, 185); border-radius: 8px; height: 25px; } </style> <script> async function handleClick() { document.getElementById(\"response\").innerHTML = \"\"; const secret = document.getElementById(\"secret\").value; const order = document.getElementById(\"order\").value; const url = \"http://192.168.0.100:8085/request?secret=\" + secret + \"&order=\" + order; const response = await fetch(url); const json = await response.json(); document.getElementById(\"response\").innerHTML = \"Status: \" + json.status; console.log(response); } </script> </head> <body> <div class=\"main\"> <div class=\"upper center-flex\">WAKE-ON-WEMOS</div> <div class=\"middle center-flex\"></div> <div class=\"lower center-flex\"> <div class=\"lower-left center-flex\"> <a class=\"status\">#status#</a><a class=\"label\">status</a> </div> <div class=\"center-flex lower-right\"> <span>Alive: #d#d #h#h #m#m #s#s</span> <span>Request: #requests#</span> <span>Last DNS update: ?</span> </div> </div> <div class=\"middle center-flex\"></div> <div class=\"form center-flex\"> <select class=\"select\" name=\"Orders\" id=\"order\"> <option value=\"ON\">Turn ON</option> <option value=\"OFF\">Turn OFF</option> <option value=\"FORCE_OFF\">Turn OFF force</option> <option value=\"RESTART\">Restart</option> </select> <input id=\"secret\" class=\"input\" type=\"password\" placeholder=\"secret\" /> <button class=\"button\" onclick=\"handleClick()\">SEND COMMAND</button> <span id=\"response\"></span> </div> </div> </body> </html>";

  html.replace("#requests#", String(orderRequests));

  const String state = powerLedValue > 900 ? "ON" : "OFF";
  html.replace("#status#", state);

  unsigned long allSeconds=millis()/1000;
  int runDays= allSeconds/(3600*24); 
  int runHours= (allSeconds/3600)%24;
  int secsRemaining=allSeconds%3600;
  int runMinutes=secsRemaining/60;
  int runSeconds=secsRemaining%60;

  html.replace("#d#", String(runDays));
  html.replace("#h#", String(runHours));
  html.replace("#m#", String(runMinutes));
  html.replace("#s#", String(runSeconds));

  client.println("HTTP/1.1 200 OK"); //
  client.println("Content-Type: text/html");
  client.println("");
  client.println(html);
  delay(1);
  Serial.println("> End of request");
  Serial.println("");
}

void proccedRequest(WiFiClient client, String request) {

  if(request.indexOf("secret=" + secret) == -1)
  {
    client.println("HTTP/1.1 403 Forbidden"); //
    client.println("Content-Type: application/json");
    client.println("");
    client.println("{ \"status\": \"ERROR\", \"message\": \"wrong secret\"}");
  }
  else
  {
    orderRequests++;
    //Turn ON
    if(request.indexOf("order=ON") != -1) {
      digitalWrite(optoPowerPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      delay(200);
      digitalWrite(optoPowerPin, LOW);
      digitalWrite(buzzerPin, LOW);
    }
    else if(request.indexOf("order=OFF") != -1) {
      digitalWrite(optoPowerPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      delay(200);
      digitalWrite(optoPowerPin, LOW);
      digitalWrite(buzzerPin, LOW);
    }
    else if(request.indexOf("order=FORCE_OFF") != -1) {
      digitalWrite(optoPowerPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      delay(600);
      digitalWrite(optoPowerPin, LOW);
      digitalWrite(buzzerPin, LOW);
    }
    else if(request.indexOf("order=RESTART") != -1) {
      digitalWrite(optoResetPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      delay(200);
      digitalWrite(optoResetPin, LOW);
      digitalWrite(buzzerPin, LOW);
    }    

    client.println("HTTP/1.1 200 OK"); //
    client.println("Content-Type: application/json");
    client.println("");
    client.println("{ \"status\": \"OK\", \"message\": \"order completed\"}");
  }
}

void setup() {
  Serial.begin(serial); //Default Baudrate
  pinMode(buzzerPin, OUTPUT);
  pinMode(optoPowerPin, OUTPUT);
  pinMode(optoResetPin, OUTPUT);
  pinMode(powerLedPin, INPUT);

  //Register event handlers
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
   
  initWiFi();

  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
  Serial.print("IP Address of wemos: "); // will IP address on Serial Monitor
  Serial.println(WiFi.localIP());

  Serial.print("WEMOS MAC addres: "); // will IP address on Serial Monitor
  Serial.println(WiFi.macAddress());

  Serial.print("Access by entering IP: "); // Will print IP address in URL format
  Serial.print(WiFi.localIP());
  Serial.print(" on PORT: ");
  Serial.println(port);

  delay(300);

  server.begin();  // Starts the Server
  Serial.println("HTTP Server started");
}

void loop() {
  WiFiClient client = server.available();

  if(client) {
    delay(50);
    if(client.available()) {
        delay(50);
        
        String request = client.readStringUntil('\r');
        Serial.println(request);
        client.flush();

        if(request.indexOf("request") != -1)
        {
          proccedRequest(client, request);
        } else {
          powerLedValue = analogRead(powerLedPin);
          displayHtmlPage(client, powerLedValue);
        }

        delay(50);
    }
  }
}