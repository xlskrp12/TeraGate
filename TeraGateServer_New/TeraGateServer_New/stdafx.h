#pragma once
#include"object.h"

struct WorldData
{
	SOCKET s;
	bool connected;

	unordered_set<int> viewList;
	mutex vlLock;

	OBJECT obj;
	OverlappedEx recvOverlap;
	int packetSize;
	int previousSize;
	unsigned char packetBuf[MAX_PACKET_SIZE];
};
