// Created By Francis Ilechukwu C. 19:56 12/08/2017
#include <ArduinoJson.h>
#include <WiFi.h>
#include <EEPROM.h>
const char* ssid     = "AEG-NET";      // Android AP SSID.
const char* password = "ieeenovanova"; // Android AP Password.
const uint32_t TxRx = 2;               // LED Pin for Transmission and Reception of packets.
const uint32_t signatureAddress = 0;
const uint32_t signatureLength = 7;
String jBuff;                          // Temporary Storage for re-use of JSON strings by the JSON Library.

WiFiServer server(1942);               // WiFi Server to listen on port 1942.

void setup() {
    Serial.begin(115200);    // For Serial outputing.
    pinMode(TxRx, OUTPUT);      // set the LED pin mode
    delay(10);
    EEPROM.begin(512);
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();
}

void loop() {
 WiFiClient client = server.available();   // listen for incoming clients
  Serial.println("here again");
  if (client) {                             // Android device has connected
    Serial.println("new client");     
    String currentLine = "";                // make a String to hold incoming data from the client (Android)
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client
        char c = client.read();             // read a byte, then
        currentLine += c;
        jBuff = currentLine;
        StaticJsonBuffer<500> jsonBuffer;
        JsonObject& root = jsonBuffer.parseObject(jBuff);
        Serial.println("Recieved" + currentLine);
        if (root.success()) {
          Serial.println("parseObject() suceeded");
          if (root["mode"] == "ping") {
            JsonObject& object = jsonBuffer.createObject();
            object["fpmid"] = "tgf46huj";
            object["mode"] = "ping";
            object["message"] = "hi";
            object.set<int>("battery", 70);
            object.set<int>("hallId", 1);
            char buff[256];
            object.printTo(buff, sizeof(buff));
            client.write(buff);
            Serial.println("Sent Packet");
          }
        }
      } else {
        //client.stop();
      }
    }
  }
    // close the connection:
    Serial.println("idle");
}
