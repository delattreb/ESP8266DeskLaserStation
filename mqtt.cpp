#include <ArduinoJson.h>
#include "mqtt.h"
#include "var.h"

//********************************
// decodeTopicTeam
// Description:
//********************************
void decodeTopicTeam(char  json[])
{
	StaticJsonBuffer<JSONBUFFER> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(json);
	if (!root.success()) {
#ifdef DEBUG
		Serial.println("parse json failed");
#endif
		return;
	}

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

//********************************
// decodeTopicParty
// Description:
//********************************
void decodeTopicParty(char  json[])
{

}

//********************************
// decodeTopicGame
// Description:
//********************************
void decodeTopicGame(char  json[])
{

}

//********************************
// sendGame
// Description:
//********************************
void sendGame()
{

}



