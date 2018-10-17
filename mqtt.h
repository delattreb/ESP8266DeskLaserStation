// mqtt.h

#ifndef _MQTT_h
#define _MQTT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// Topic managment
void decodeTopicTeam(char json[]);
void decodeTopicParty(char json[]);
void decodeTopicGame(char json[]);

// Send MQTT
void sendGame();

#endif

