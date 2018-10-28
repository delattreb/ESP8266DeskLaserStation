// VAR

#pragma region DEBUG
//#define INFO
//#define DEBUG
#pragma endregion

#pragma region CONFIG
#define SERIALBAUDS 9600
#define DB_FREQUENCY 3000
#define ATTENPTING 10000
#define JSONBUFFER 300
#define MESSAGEBUFFER 200
#define CMD_PREFIX "_"
#define CMD_TEAM "T" 
#define CMD_PARTY "P"
#define CMD_GAME "G"
#define CMD_KILL "K"
#pragma endregion

#pragma region TOPICS
#define TOPIC_TEAM "team"
#define TOPIC_GAME "game"
#define TOPIC_PARTY "party"
#define TOPIC_START "start"
#pragma endregion

#pragma region IP
// IP Start
#define IPLOWA 10
#define IPLOWB 0
#define IPLOWC 0
#define IPLOWD 1

// IP End
#define IPHIGHA 10
#define IPHIGHB 0
#define IPHIGHC 0
#define IPHIGHD 1
#pragma endregion

#pragma region MQTT
#define MQTTPORT 1884
#define IP_SERVER "mycube.dscloud.me"
#pragma endregion

#pragma region GPIO
#define GPIO_0 0
#define GPIO_2 2
#pragma endregion