#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MAX_BUFF_SIZE	4000
#define MAX_PACKET_SIZE	4000

//#define MAP_WIDTH		200
//#define MAP_HEIGHT		200

#define MAP_SIZE		2000

#define VIEW_RADIUS		20000

#define NUM_THREADS		4

#define MAX_USER		6

#define NPC_START		100
#define NUM_OF_NPC		220

#define TOWER_START		4000
#define NUM_OF_NEXUS		3
#define NUM_OF_POWERSPOT	10
#define NUM_OF_OBJECT	13

#define MY_SERVER_PORT	4000

#define MAX_STR_SIZE	100

// NPC 상태
#define	NPC_MOVE		0
#define	NPC_ATTACK		1

//클라->서버 
#define CS_MOUSE_MOVE			1
#define CS_RIGHT_UP				2
#define CS_LEFT_UP				3
#define CS_RIGHT_DOWN			4
#define CS_LEFT_DOWN			5
#define CS_UP					6
#define CS_DOWN					7
#define CS_LEFT					8
#define CS_RIGHT				9

#define CS_LOGIN				10
#define CS_ATTACK				11
#define CS_SKILL				12

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

	WORD hp;
	WORD maxHp;
	WORD monsterType;

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
