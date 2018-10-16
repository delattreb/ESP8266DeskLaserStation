#include <ESP8266WiFi.h>
//#include <DNSServer.h>
//#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include "var.h"

char nodeServer[] = IP_SERVER;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
const int CHAR = 48;
int32_t oldRSSI = 0;
static unsigned long previousMillis = 0;
unsigned long currentMillis;

//
// reconnect
//
void callback(char *topic, byte *payload, unsigned int length)
{
#ifdef INFO
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");
	for (int i = 0; i < length; i++)
		Serial.print((char)payload[i]);
	Serial.println();
#endif
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
			Serial.println("K");
			mqttClient.connect(WiFi.macAddress().c_str());
			delay(ATTENPTING);
		}
#ifdef INFO
		Serial.println("");
		Serial.println("Connected");
#endif
		Serial.println("O");
		mqttClient.subscribe(TOPIC_TEAM);
	}
}

//
// setup
//
void setup()
{
	Serial.begin(SERIALBAUDS);
	while (!Serial)
	{
		;
	}
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
	oldRSSI = WiFi.RSSI();
}

//
// loop
//
void loop()
{
	currentMillis = millis();
	reconnect();
	if (currentMillis - previousMillis >= DB_FREQUENCY)
	{
		if (oldRSSI != WiFi.RSSI())
		{
			Serial.println("S" + String(WiFi.RSSI()));
			oldRSSI = WiFi.RSSI();
		}
		previousMillis = currentMillis;
	}
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
	String strH = "iot:h";
	char attributest[100];
	char attributesh[100];
	temp.toCharArray(attributest, 100);
	hum.toCharArray(attributesh, 100);
	if (mqttClient.connected())
	{
#ifdef DEBUG
		Serial.println("Before send to MQTT broker:");
		Serial.println(temp);
		Serial.println(hum);
		Serial.println(sensor);
		Serial.println(attributest);
		Serial.println(attributesh);
#endif
		strT.concat(sensor);
		strH.concat(sensor);
		mqttClient.publish(strT.c_str(), attributest);
		mqttClient.publish(strH.c_str(), attributesh);
	}
}