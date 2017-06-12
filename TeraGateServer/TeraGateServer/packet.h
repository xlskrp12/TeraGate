#ifndef PACKET_H
#define PACKET_H

#include <winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

#include"protocol.h"

#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#include<vector>
#include<thread>
#include<algorithm>
#include<unordered_set>
#include<mutex>
#include<time.h>
#include<queue>
#include<chrono>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

#define OP_RECV		1
#define OP_SEND		2
#define OP_MOVE		3

#define PLAYER_HP	100

#define MONSTER_HP_STANDARD	50
#define MONSTER_HP_MASTER	100

#define NEXUS_HP 500

using namespace std;
using namespace chrono;

struct OverlappedEx
{
	WSAOVERLAPPED originalOverlapped;
	int operation;
	int param1;
	WSABUF wsabuf;
	unsigned char iocpBuffer[MAX_BUFF_SIZE];
};


struct Object
{
	int		x;
	int		y;
	int		z;

	int		id;
	WCHAR	name[MAX_STR_SIZE];
	int		HP;
	int		maxHp;
	int		type;
	int		skill;

	bool	isActive;
};

struct TowerData
{
	bool exist;
	int x;
	int y;

	int HP;
	int maxHP;
};

struct WorldData {
	SOCKET s;
	bool exist;
	bool connected;

	unordered_set<int> viewList;
	mutex vlLock;

	Object obj;
	OverlappedEx recvOverlap;
	int packetSize;
	int previousSize;
	unsigned char packetBuf[MAX_PACKET_SIZE];
};

#endif