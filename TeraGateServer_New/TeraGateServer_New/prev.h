#pragma once
#ifndef PACKET_H
#define PACKET_H

#include<time.h>

#include <winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

#include"protocol.h"
#include"timer.h"
#include"enum.h"

#include"errorDisplay.h"

#include<stdlib.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
#include<vector>
#include<thread>
#include<algorithm>
#include<unordered_set>
#include<mutex>

#include<queue>
#include<chrono>

#include<D3D11.h>       // #include<D3DX11.h> d3d11, d3dx 차이점이 뭐지?;
#include<D3DX11.h>      // D3DX11CompileFromFile()함수 사용가능. D3D11은 안됨.
#include<D3Dcompiler.h> // 쉐이더 컴파일 함수 지원
#include<D3DX10math.h>  // Direct3D Math 함수 지원
#include<D3DX11async.h> // CreateVertexShaderFromFile() 함수 헤더인데 왜 아예 안되지..
#include<d3dcommon.h>
#include<DXGI.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

#define OP_RECV					1
#define OP_SEND					2
#define OP_MOVE					3
#define OP_EVENT_PLAYER_MOVE	4


#define EVENT_RESPAWN			10
#define EVENT_MOVE				11
#define EVENT_ATK				12
#define EVENT_MONSTER_ATK		13

#define PLAYER_HP				100

#define MONSTER_HP_STANDARD		50
#define MONSTER_HP_MASTER		100

#define NEXUS_HP				4000
#define NEXUS_MAXHP				4000

#define STONE_HP				50000
#define STONE_MAXHP				50000

#define DB_CONNECT				1
#define DB_SELECT_NAME			2
#define DB_UPDATE				3
#define DB_INSERT_ID			4

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



struct eventType {
	int objectID;
	unsigned int wakeUpTime;
	int eventID;
};

class mycomp
{
public:
	bool operator()(const eventType lhs, const eventType rhs) const
	{
		return (lhs.wakeUpTime > rhs.wakeUpTime);
	}
};



#endif