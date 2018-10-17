// mqtt.h

#ifndef _MQTT_h
#define _MQTT_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

// Topic managment
void decodeTopicTeam(char json[], String mac_adr);
void decodeTopicParty(char json[]);
void decodeTopicGame(char json[]);
void decodeTopicStart(char json[]);

#endif

