#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "mqtt.h"
#include "var.h"

char nodeServer[] = IP_SERVER;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
char json[JSONBUFFER];

//********************************
// callback
// Description:
//********************************
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
	for (int i = 0; i < length; i++)
		json[i] = (char)payload[i];
	if (strcmp(topic, TOPIC_TEAM) == 0)
		decodeTopicTeam(json, WiFi.macAddress());
	if (strcmp(topic, TOPIC_PARTY) == 0)
		decodeTopicParty(json);
	if (strcmp(topic, TOPIC_GAME) == 0)
		decodeTopicGame(json);
	if (strcmp(topic, TOPIC_START) == 0)
		decodeTopicStart(json);
}

//********************************
// reconnect
// Description:
//********************************
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
			Serial.print(mqttClient.state());
#endif
			mqttClient.connect(WiFi.macAddress().c_str());
			delay(ATTENPTING);
		}
#ifdef INFO
		Serial.println("");
		Serial.println("MQTT connected");
#endif
		mqttClient.subscribe(TOPIC_PARTY);
		mqttClient.subscribe(TOPIC_TEAM);
		mqttClient.subscribe(TOPIC_GAME);
		mqttClient.subscribe(TOPIC_START);
	}
}

//********************************
// setup
// Description:
//********************************
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
	wifiManager.setDebugOutput(false);
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

	//ESP8266 Configuration
	pinMode(GPIO_0, OUTPUT);
	digitalWrite(GPIO_0, HIGH);
	pinMode(GPIO_2, INPUT);
	digitalWrite(GPIO_2, LOW);
}

//********************************
// sendGame
// Description:
//********************************
void sendGame(String id, String target)
{
	StaticJsonBuffer<MESSAGEBUFFER> JSONbuffer;
	JsonObject &JSONencoder = JSONbuffer.createObject();

	JSONencoder["id"] = id;
	JSONencoder["target"] = target;

	char JSONmessageBuffer[MESSAGEBUFFER];
	JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
#ifdef DEBUG
	Serial.println("Sending message to MQTT topic");
	Serial.println(JSONmessageBuffer);
#endif
	if (mqttClient.publish(TOPIC_GAME, JSONmessageBuffer) == true) {
#ifdef DEBUG
		Serial.println("Success sending message");
#endif
	}
	else {
#ifdef DEBUG
		Serial.println("Error sending message");
#endif
	}
}

//********************************
// loop
// Description:
//********************************
void loop()
{
	reconnect();

	if (mqttClient.connected())
		mqttClient.loop();
	// Check if data available on serial port
	if (Serial.available())
	{
		String data = Serial.readString();
		if (data.startsWith(CMD_KILL, 0)) {
			String id = data.substring(1, data.indexOf(';'));
			Serial.println(id);
			String target = data.substring(data.indexOf(';') + 1, data.length());
			Serial.println(target);
#ifdef DEBUG
			Serial.print("Id: ");
			Serial.println(id);
			Serial.print("Target: ");
			Serial.println(target);
#endif
			//Send to MQTT
			sendGame(id, target);
		}
	}
}