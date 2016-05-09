#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>
#include <ThingSpeak.h>
#include <Wire.h>
#include <Adafruit_MCP9808.h>

Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

#include "settings.h"

const char* ssid = SETTINGS_SSID;
const char* password = SETTINGS_PASS;

volatile float c;

#define DHTPIN 2
#define DHTTYPE DHT11

WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready OTA");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  setupTempSensor();
  ThingSpeak.begin(client);
}

int counter;

void loop() {
  counter++;
  // 1200 * 100 - this should be about 2 min, but not sure how long ArduinoOTA.handle() takes...
  if (counter > 1200) { 
    readTemp();
    postValues();
    counter = 0;
  }
  ArduinoOTA.handle();
  delay(100);
}

void postValues(void) {
  if (!isnan(c)) {
    Serial.println("posting: c: " + convertFloatToString(c));
    ThingSpeak.setField(1,c);
    ThingSpeak.writeFields(SETTINGS_THINGSPEAK_CHANNEL, SETTINGS_THINGSPEAK_KEY);
  } 
  else {
    Serial.println("dht read error");
  }
}

void readTemp(void) {
  tempsensor.shutdown_wake(0);  // Don't remove this line! required before reading temp
  c = tempsensor.readTempC();
  float f = c * 9.0 / 5.0 + 32;
  Serial.print("Temp: "); Serial.print(c); Serial.print("*C\t"); 
  Serial.print(f); Serial.println("*F");
  delay(250);
  
  Serial.println("Shutdown MCP9808.... ");
  tempsensor.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere
  
  delay(2000);
}

void setupTempSensor(void) {
  uint8_t started = 0;
  while (!started) {
    started = tempsensor.begin();
    Serial.println("Couldn't find MCP9808!");
    delay(100);
  }
}

String convertFloatToString(float n) {
  char buf2[16];
  return dtostrf(n, 5, 2, buf2);
}






