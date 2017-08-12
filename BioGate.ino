#include <ArduinoJson.h>
#include <WiFi.h>

const char* ssid     = "AEG-NET";
const char* password = "ieeenovanova";

String jBuff;

WiFiServer server(1942);

void setup()
{
    Serial.begin(115200);
    pinMode(2, OUTPUT);      // set the LED pin mode

    delay(10);

    // We start by connecting to a WiFi network

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

int value = 0;

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients
  Serial.println("here again");
  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        Serial.println("available");
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
