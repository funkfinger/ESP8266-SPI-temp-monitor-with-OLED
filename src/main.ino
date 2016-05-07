#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

void setup() {
  Serial.begin(115200);
  WiFiManager wifiManager;
  // wifiManager.resetSettings();
  wifiManager.setDebugOutput(false);
  wifiManager.autoConnect();
  // wifiManager.startConfigPortal("wifiManager");
  Serial.println("connected..."); //
  Serial.println(WiFi.localIP());
}



void loop() {
  // put your main code here, to run repeatedly:

}