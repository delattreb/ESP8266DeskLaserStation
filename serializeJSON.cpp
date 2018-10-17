#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char *ssid = "yourNetworkName";
const char *password = "yourNetworkPassword";
const char *mqttServer = "m11.cloudmqtt.com";
const int mqttPort = 12948;
const char *mqttUser = "yourInstanceUsername";
const char *mqttPassword = "yourInstancePassword";

WiFiClient espClient;
PubSubClient client(espClient);

void loop()
{
    StaticJsonBuffer<300> JSONbuffer;
    JsonObject &JSONencoder = JSONbuffer.createObject();

    JSONencoder["device"] = "ESP32";
    JSONencoder["sensorType"] = "Temperature";
    JsonArray &values = JSONencoder.createNestedArray("values");

    values.add(20);
    values.add(21);
    values.add(23);

    char JSONmessageBuffer[100];
    JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println("Sending message to MQTT topic..");
    Serial.println(JSONmessageBuffer);

    if (client.publish("esp/test", JSONmessageBuffer) == true)
        Serial.println("Success sending message");
    else
        Serial.println("Error sending message");

    client.loop();
}