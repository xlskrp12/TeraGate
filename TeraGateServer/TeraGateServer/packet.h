#ifndef PACKET_H
#define PACKET_H

#include <winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

#include"protocol.h"

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
#include<time.h>
#include<queue>
#include<chrono>

#include<D3D11.h>       // #include<D3DX11.h> d3d11, d3dx �������� ����?;
#include<D3DX11.h>      // D3DX11CompileFromFile()�Լ� ��밡��. D3D11�� �ȵ�.
#include<D3Dcompiler.h> // ���̴� ������ �Լ� ����
#include<D3DX10math.h>  // Direct3D Math �Լ� ����
#include<D3DX11async.h> // CreateVertexShaderFromFile() �Լ� ����ε� �� �ƿ� �ȵ���..
#include<d3dcommon.h>
#include<DXGI.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

#define OP_RECV		1
#define OP_SEND		2
#define OP_MOVE		3

#define PLAYER_HP	100

#define MONSTER_HP_STANDARD	50
#define MONSTER_HP_MASTER	100

#define NEXUS_HP 500

#define DEGREE_TO_RADIAN(degree) (degree * 0.0174532925f)
#define RADIAN_TO_DEGREE(radian) (radian * 57.295779513f)

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
	float		x;
	float		y;
	float		z;

	float		roty;

	int		id;
	int		HP;
	int		maxHp;
	int		type;
	int		skill;

	bool	isActive;
};

struct TowerData
{
	bool exist;
	float x;
	float y;
	float z;

	int HP;
	int maxHP;
};

struct WorldData {
	SOCKET s;
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