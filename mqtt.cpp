#include <ArduinoJson.h>
#include "mqtt.h"
#include "var.h"

//********************************
// decodeTopicTeam
// Description:
//********************************
void decodeTopicTeam(char  json[], String mac_adr)
{
	StaticJsonBuffer<JSONBUFFER> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(json);
	if (!root.success()) {
#ifdef DEBUG
		Serial.println("parse json failed");
#endif
		return;
	}
	const char* jacket = root["jacket"];
	const char* color_r = root["r"];
	const char* color_g = root["g"];
	const char* color_b = root["b"];
	const char* luminosity = root["lum"];
	const char* pseudo = root["pseudo"];
	const char* id = root["id"];
#ifdef DEBUG
	Serial.print("Jacket: ");
	Serial.println(jacket);
	Serial.print("ColorR: ");
	Serial.println(color_r);
	Serial.print("ColorG: ");
	Serial.println(color_g);
	Serial.print("ColorB: ");
	Serial.println(color_b);
	Serial.print("Lum: ");
	Serial.println(luminosity);
	Serial.print("Pseudo: ");
	Serial.println(pseudo);
	Serial.print("Id: ");
	Serial.println(id);
#endif
	//Check MAC Address
	if (strcmp(jacket, mac_adr.c_str()) == 0) {
		// Send to serial CMD: TEAM
		String str = CMD_TEAM;
		str.concat(";");
		str.concat(color_r);
		str.concat(";");
		str.concat(color_g);
		str.concat(";");
		str.concat(color_b);
		str.concat(";");
		str.concat(luminosity);
		str.concat(";");
		str.concat(id);
		str.concat(";");
		Serial.println(str);

		// Send to serial CMD: PSEUDO
		str = CMD_PSEUDO;
		str.concat(";");
		str.concat(pseudo);
		str.concat(";");
		Serial.println(str);
	}
	else {
#ifdef DEBUG	
		Serial.println("Not right MAC adr");
#endif
	}
}

//********************************
// decodeTopicParty
// Description:
//********************************
void decodeTopicParty(char json[])
{
	StaticJsonBuffer<JSONBUFFER> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(json);
	if (!root.success()) {
#ifdef DEBUG
		Serial.println("parse json failed");
#endif
		return;
	}
	const char* poptime = root["poptime"];
	const char* start = root["start"];
#ifdef DEBUG
	Serial.print("PopTime: ");
	Serial.println(poptime);
	Serial.print("Start: ");
	Serial.println(start);
#endif
	// Send to serial CMD: T
	String str = "";
	str.concat(CMD_PARTY);
	str.concat(";");
	str.concat(poptime);
	str.concat(";");
	str.concat(start);
	str.concat(";");
	Serial.println(str);
	}

//********************************
// decodeTopicGame
// Description:
//********************************
void decodeTopicGame(char json[])
{

}

//********************************
// decodeTopicStart
// Description:
//********************************
void decodeTopicStart(char json[])
{
	StaticJsonBuffer<JSONBUFFER> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(json);
	if (!root.success()) {
#ifdef DEBUG
		Serial.println("parse json failed");
#endif
		return;
	}
	const char* go = root["go"];
#ifdef DEBUG
	Serial.print("Go: ");
	Serial.println(go);
#endif
	// Update GPIO state
	if (strcmp(go, "1") == 0)
		digitalWrite(GPIO_0, LOW);
	if (strcmp(go, "0") == 0)
		digitalWrite(GPIO_0, HIGH);
	}

//********************************
// sendWiFiConnection
// Description:
//********************************
void sendWiFiConnection(String wificonnection)
{
	String str = CMD_WIFI_CONNECT;
	str.concat(";");
	str.concat(wificonnection);
	str.concat(";");
	Serial.println(str);
}

//********************************
// sendMQTTConnection
// Description:
//********************************
void sendMQTTConnection(String mqttconnection)
{
	String str = CMD_MQTT_CONNECT;
	str.concat(";");
	str.concat(mqttconnection);
	str.concat(";");
	Serial.println(str);
}
