#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "var.h"

char nodeServer[] = IP_SERVER;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
const int CHAR = 48;
static unsigned long previousMillis = 0;
unsigned long currentMillis;

// 
// Decode JSON
// 
void decodeJSON(char *topic, char json[], unsigned int length)
{
	StaticJsonBuffer<JSONBUFFER> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(json);
	if (!root.success()) {
#ifdef DEBUG
		Serial.println("parse json failed");
#endif
		return;
	}

	// In other case, you can do root["time"].as<long>();
	const char* team = root["team"];
	const char* jacket = root["jacket"];
#ifdef DEBUG
	Serial.print("Team: ");
	Serial.println(team);
	Serial.print("Jacket: ");
	Serial.println(jacket);
#endif
	//long time = root["time"];
	//double latitude = root["data"][0];
	//double longitude = root["data"][1];

	//Check MAC Address
}

//
// callback
//
void callback(char *topic, byte *payload, unsigned int length)
{
#ifdef DEBUG
	Serial.print("Topic: ");
	Serial.println(topic);
	Serial.print("Payload: ");
	for (int i = 0; i < length; i++)
		Serial.print((char)payload[i]);
	Serial.println("");
#endif
	//Decode JSON
	char json[JSONBUFFER];
	for (int i = 0; i < length; i++)
		json[i] = (char)payload[i];
	decodeJSON(topic, json, length);
}

//
// reconnect
//
void reconnect()
{
	// Connect to MQTT
	if (!mqttClient.connected())
	{
#ifdef INFO
		Serial.println("Attempting MQTT connection...");
#endif
		while (!mqttClient.connected())
		{
#ifdef INFO
			Serial.print(".");
#endif
			mqttClient.connect(WiFi.macAddress().c_str());
			delay(ATTENPTING);
		}
#ifdef INFO
		Serial.println("");
		Serial.println("Connected");
#endif
		mqttClient.subscribe(TOPIC_PARTY);
		mqttClient.subscribe(TOPIC_TEAM);
		mqttClient.subscribe(TOPIC_GAME);
	}
}

//
// setup
//
void setup()
{
	Serial.begin(SERIALBAUDS);
	while (!Serial)  continue;

#ifdef INFO
	delay(1500);
	Serial.print("Core version: ");
	Serial.println(ESP.getCoreVersion());
	Serial.print("Sdk version: ");
	Serial.println(ESP.getSdkVersion());
	Serial.print("MAC Address: ");
	Serial.println(WiFi.macAddress());
#endif
	WiFiManager wifiManager;
	//Reset setting
	//wifiManager.resetSettings();

	wifiManager.setAPStaticIPConfig(IPAddress(IPLOWA, IPLOWB, IPLOWC, IPLOWD), IPAddress(IPHIGHA, IPHIGHB, IPHIGHC, IPHIGHD), IPAddress(255, 255, 255, 0));

	if (!wifiManager.autoConnect(WiFi.macAddress().c_str()))
	{
		Serial.println("Failed to connect");
		delay(1000);
		ESP.reset();
		delay(5000);
	}

	mqttClient.setServer(nodeServer, MQTTPORT);
	mqttClient.setCallback(callback);
	reconnect();
}

//
// loop
//
void loop()
{
	currentMillis = millis();
	reconnect();
	//if (currentMillis - previousMillis >= DB_FREQUENCY)
	//	previousMillis = currentMillis;
	if (mqttClient.connected())
		mqttClient.loop();
	// Check if data available
	if (Serial.available())
	{
		//data = Serial.readString();
		//if (data.startsWith("T", 0))
		//	sensor[int(data[1]) - CHAR - 1] = data.substring(3, data.length() - 1);
	}

	// Transmit Data
	//sendMQTT(data[1], sensor[int(data[1]) - CHAR - 1], sensor[int(data[1]) - CHAR]);
}

//
// sendMQTT
//
void sendMQTT(char sensor, String temp, String hum)
{
	// Send payload
	String strT = "iot:t";
	char attributest[100];
	temp.toCharArray(attributest, 100);
	if (mqttClient.connected())
	{
#ifdef DEBUG
		Serial.println("Before send to MQTT broker:");
		Serial.println(temp);
		Serial.println(hum);
		Serial.println(sensor);
		Serial.println(attributest);
#endif
		strT.concat(sensor);
		mqttClient.publish(strT.c_str(), attributest);
	}
}
