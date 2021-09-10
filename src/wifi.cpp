#include <Arduino.h>
#include <arduino-timer.h>
#include <WiFi.h>
#include <WiFiServer.h>
#include "wifi.h"
#include "secrets.h"

auto wifiTimer = timer_create_default();
WiFiServer server(4000);


void initWifi() {
    Serial.println(String("Init wifi"));

    WiFi.mode(WIFI_STA);
    WiFi.begin(SECRET_WIFI_SSID, SECRET_WIFI_SSID_PASSWORD);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
    Serial.print(String("Got IP: "));
    Serial.println(WiFi.localIP());
    server.begin();

    // update rotaryPosition
    wifiTimer.every(200, [](void*) -> bool { 
        if (WiFi.status() != WL_CONNECTED) {
            Serial.print(millis());
            Serial.println("Reconnecting to WiFi...");
            WiFi.disconnect();
            WiFi.reconnect();
        }
        return true; 
    });


    wifiTimer.every(50, [](void*) -> bool { 
        Serial.println("Waiting for client."); 

      WiFiClient client = server.available();   // Listen for incoming clients
      Serial.println("New Client."); 
      if (client) {
          while(client.connected()) {
              char c = client.read(); 
              Serial.println("Got data " + String(c));
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              client.stop();
          }
      }
      return true; 
    });
}
