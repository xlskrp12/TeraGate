#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MAX_BUFF_SIZE	4000
#define MAX_PACKET_SIZE	4000

#define CLIENT_WIDTH  1280//1960//1280
#define CLIENT_HEIGHT 720//1080//720
// 매크로로 계산할 때는 괄호를 꼭 챙겨주자..
#define DEGREE_TO_RADIAN(degree) ((degree) * 0.0174532925f)
#define RADIAN_TO_DEGREE(radian) ((radian) * 57.295779513f)
#define SRAND() srand((unsigned)time(NULL))                            //time.h
#define SRAND_SEED(seed) srand((unsigned)time(NULL)*(seed))            //time.h
#define RANDOM_S(STANDARD, SIZE) ((STANDARD)-((SIZE)/2)+rand()%(SIZE)) //time.h
#define RANDOM_M(MIN, MAX) ((MIN) + rand()%((MAX)+1-(MIN)))            //time.h

#define MAP_SIZE		3150

#define VIEW_RADIUS		5000

#define NUM_THREADS		4

#define MAX_USER		6
#define USER_START		40

//#define NPC_PLAYER_START	10
#define NUM_NPC_PLAYER		4

#define NPC_START		10
#define NUM_OF_NPC		94
#define NUM_OF_NPC_AND_PLAYER		100

//#define TOWER_START		1000
//#define NUM_OF_NEXUS	3
//#define NUM_OF_GATE		10
#define NUM_OF_TOWER	13
#define NUM_OF_STONE	27

#define NUM_OF_OBJECT	140

#define MY_SERVER_PORT	4000

#define MAX_STR_SIZE	100

// NPC 상태
#define	NPC_MOVE		0
#define	NPC_ATTACK		1

//클라->서버 
enum 
{
	CS_RIGHT_UP,				
	CS_LEFT_UP,				
	CS_RIGHT_DOWN,			
	CS_LEFT_DOWN,			
	CS_UP,					
	CS_DOWN	,				
	CS_LEFT,					
	CS_RIGHT,		
	
	CS_LOGIN,				
	CS_LOGOUT,				
	CS_ATTACK,				
	CS_SKILL_E,				
	
	CS_SHIFT_DOWN,			
	CS_SHIFT_UP				
};

//서버->클라
#define	SC_POS				1
#define SC_PUT_PLAYER		2
#define SC_REMOVE_PLAYER	3
#define SC_ATTACK			4



#pragma pack(push,1)

struct cs_packet_login
{
	BYTE size;
	BYTE type;
};

struct cs_packet_attack
{
	BYTE size;
	BYTE type;
};

struct cs_packet_right_up {
	BYTE size;
	BYTE type;
};

struct cs_packet_left_up {
	BYTE size;
	BYTE type;
};

struct cs_packet_right_down {
	BYTE size;
	BYTE type;
};

struct cs_packet_left_down {
	BYTE size;
	BYTE type;
};

struct cs_packet_up {
	BYTE size;
	BYTE type;
};

struct cs_packet_down {
	BYTE size;
	BYTE type;
};

struct cs_packet_left {
	BYTE size;
	BYTE type;
};

struct cs_packet_right {
	BYTE size;
	BYTE type;
};

struct sc_packet_pos
{
	BYTE size;
	BYTE type;
	WORD id;
	WORD monsterType;
	float x;
	float y;
	float z;
	
	float roty;
};

struct sc_packet_attack 
{
	BYTE size;
	BYTE type;
	WORD id;
	WORD hp;
};

struct sc_packet_put_player {
	BYTE size;
	BYTE type;
	WORD id;

	WORD playerNum;
	WORD hp;
	WORD maxHp;
	WORD monsterType;

	WORD teamId;
	WORD gateId;

	float x;
	float y;
	float z;

	float roty;
};

struct sc_packet_remove_player {
	BYTE size;
	BYTE type;
	WORD id;
};

#pragma pack(pop)
#endif
