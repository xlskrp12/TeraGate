#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MAX_BUFF_SIZE	1000
#define MAX_PACKET_SIZE	1000

#define MAP_WIDTH		200
#define MAP_HEIGHT		200

#define MAX_USER		6

#define NPC_START		100
#define NUM_OF_NPC		3000

#define NUM_OF_OBFECT	2000

#define MY_SERVER_PORT	4000

#define MAX_STR_SIZE	100

// NPC 상태
#define	NPC_MOVE		0
#define	NPC_ATTACK		1

//클라->서버 
#define CS_MOUSE_MOVE	1
#define	CS_MOUSE_ATTACK	2
#define CS_KEY_Q		3
#define CS_KEY_W		4
#define CS_KEY_E		5

#define CS_LOGIN		6
#define CS_ATTACK		7

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
	WCHAR name[MAX_STR_SIZE];
};

struct cs_packet_attack
{
	BYTE size;
	BYTE type;
};

struct cs_packet_move
{
	BYTE size;
	BYTE type;
	int goal;
};

struct sc_packet_pos
{
	BYTE size;
	BYTE type;
	WORD id;
	WORD x;
	WORD y;
	WORD monsterType;
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
	WORD x;
	WORD y;

	WORD hp;
	WORD maxHp;
	WORD monsterType;
	WCHAR name[MAX_STR_SIZE];
};

struct sc_packet_remove_player {
	BYTE size;
	BYTE type;
	WORD id;
};

#pragma pack(pop)
#endif
