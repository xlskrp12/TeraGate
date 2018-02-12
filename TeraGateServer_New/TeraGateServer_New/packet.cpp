#include"packet.h"

PACKET* PACKET::instance = nullptr;

void PACKET::Init()
{
	int radius = 0;
	int radian = 0;

	for (int i = 0; i < NUM_OF_OBJECT; i++)
	{
		worldData[i].previousSize = 0;
		worldData[i].packetSize = 0;
		ZeroMemory(worldData[i].packetBuf, sizeof(worldData[i].packetBuf));
	}

	for (auto i = 0; i < MAX_USER; i++)
	{
		worldData[i].connected = false;
		worldData[i].recvOverlap.operation = OP_RECV;

		worldData[i].obj.setHP(PLAYER_HP);
		worldData[i].obj.setMaxHP(PLAYER_HP);


		radius = 2800;
		if (i % 2 == 0)
		{
			radian = DEGREE_TO_RADIAN(210);
			worldData[i].obj.setPos(radius*cos(radian), 0.f, radius*sin(radian));
			worldData[i].obj.setRotY(0.f);


		}
		else
		{
			radian = DEGREE_TO_RADIAN(-30);
			worldData[i].obj.setPos(radius*cos(radian), 0.f, radius*sin(radian));
			worldData[i].obj.setRotY(0.f);
		}

		worldData[i].recvOverlap.wsabuf.buf =
			reinterpret_cast<CHAR *>(worldData[i].recvOverlap.iocpBuffer);

		worldData[i].recvOverlap.wsabuf.len =
			sizeof(worldData[i].recvOverlap.iocpBuffer);

		cout << i << " 번 초기화 완료" << endl;
	}

	for (auto i = MAX_USER; i < NUM_OF_NPC_AND_PLAYER; i++)
	{
		worldData[i].connected = false;
	}

	for (auto i = NUM_OF_NPC_AND_PLAYER; i < NUM_OF_NPC_AND_PLAYER + NUM_OF_TOWER; i++)
	{
		worldData[i].connected = false;
	}

	for (auto i = NUM_OF_NPC_AND_PLAYER + NUM_OF_TOWER; i < NUM_OF_OBJECT; i++)
	{
		worldData[i].connected = false;
	}
}

void PACKET::SendPacket(int id, unsigned char *packet)
{
	OverlappedEx *over = new OverlappedEx;
	memset(over, 0, sizeof(OverlappedEx));

	over->operation = OP_SEND;
	over->wsabuf.buf = reinterpret_cast<CHAR *>(over->iocpBuffer);
	over->wsabuf.len = packet[0];
	memcpy(over->iocpBuffer, packet, packet[0]);


	int retval = WSASend(worldData[id].s, &over->wsabuf, 1, NULL, 0,
		&over->originalOverlapped, NULL);
	if (0 != retval)
	{
		int errNo = WSAGetLastError();
		errDisplay("SendPacket:WSASend", errNo);
		cout << id << "번 에서 오류" << endl;
		while (true);
	}
}

void PACKET::SendPutPlayerPacket(int id, int object)
{
	sc_packet_put_player packet;
	//memset(&packet,0, sizeof(packet));
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_PUT_PLAYER;

	packet.x = worldData[object].obj.getPosX();
	packet.y = worldData[object].obj.getPosY();
	packet.z = worldData[object].obj.getPosZ();

	cout << object << endl;
	SendPacket(id, reinterpret_cast<unsigned char *>(&packet));
}

void PACKET::SendRemovePlayerPacket(int id, int object)
{
	sc_packet_remove_player packet;
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_REMOVE_PLAYER;

	SendPacket(id, reinterpret_cast<unsigned char *>(&packet));
}

void PACKET::SendAttackPacket(int id)
{

}

void PACKET::LoginPlayer(int id)
{
	cout << id << " 번 player 로그인" << endl;

	worldData[id].packetSize = 0;
	worldData[id].previousSize = 0;

	memset(&worldData[id].recvOverlap.originalOverlapped, 0,
		sizeof(worldData[id].recvOverlap.originalOverlapped));

	worldData[id].recvOverlap.operation = OP_RECV;
	worldData[id].recvOverlap.wsabuf.buf =
		reinterpret_cast<CHAR *>(worldData[id].recvOverlap.iocpBuffer);

	worldData[id].recvOverlap.wsabuf.len =
		sizeof(worldData[id].recvOverlap.iocpBuffer);

	worldData[id].vlLock.lock();
	worldData[id].viewList.clear();
	worldData[id].vlLock.unlock();

	worldData[id].vlLock.lock();
	sc_packet_put_player enterPacket;
	enterPacket.id = id;
	enterPacket.size = sizeof(enterPacket);
	enterPacket.type = SC_PUT_PLAYER;
	enterPacket.x = GetWorldDataPosX(id);
	enterPacket.y = GetWorldDataPosY(id);
	enterPacket.z = GetWorldDataPosZ(id);
	enterPacket.roty = GetWorldDataRotY(id);

	worldData[id].vlLock.unlock();

	SendPacket(id, reinterpret_cast<unsigned char *>(&enterPacket));

	for (auto i = 0; i < MAX_USER; ++i)
	{
		if (false == worldData[i].connected)
			continue;
		if (i == id)
			continue;

		worldData[i].vlLock.lock();
		worldData[i].viewList.insert(id);
		worldData[i].vlLock.unlock();

		SendPacket(i, reinterpret_cast<unsigned char *>(&enterPacket));
	}

	worldData[id].connected = true;

	cout << id << "connected in login" << endl;
}
void PACKET::LogoutPlayer(int id)
{

}

bool PACKET::GetWorldDataConnected(int id)
{
	if (false == worldData[id].connected)
		return false;
	else
		return true;
}

void PACKET::SetWorldDataConnected(int id, bool exist)
{
	if (true == exist)
		worldData[id].connected = true;
	else
		worldData[id].connected = false;
}

void PACKET::SetWorldDataSocket(int id, SOCKET s)
{
	worldData[id].s = s;
	memset(&worldData[id].recvOverlap.originalOverlapped, 0,
		sizeof(worldData[id].recvOverlap.originalOverlapped));
}

int PACKET::GetWorldDataSocket(int id)
{
	return worldData[id].s;
}

void PACKET::SetWSARecv(SOCKET newClient, int id)
{
	DWORD flags = 0;


	retval = WSARecv(newClient, &worldData[id].recvOverlap.wsabuf, 1,
		NULL, &flags, &worldData[id].recvOverlap.originalOverlapped, NULL);

	cout << retval << endl;
}

int PACKET::GetWSARecvRetval()
{
	return retval;
}

WSABUF PACKET::GetWolrdDataWsabuf(int id)
{
	return worldData[id].recvOverlap.wsabuf;
}

OVERLAPPED PACKET::GetWorldDataOriginalOverlapped(int id)
{
	return worldData[id].recvOverlap.originalOverlapped;
}

unsigned char *PACKET::GetIocpBuffer(int id)
{
	return worldData[id].recvOverlap.iocpBuffer;
}

int PACKET::GetPacketSize(int id)
{
	return worldData[id].packetSize;
}

void PACKET::SetPacketSize(int id, int a)
{
	worldData[id].packetSize = a;
}

void PACKET::SetPacketSize(int id, unsigned char *bufPtr)
{
	worldData[id].packetSize = bufPtr[0];
}

void PACKET::SetPreviousSize(int id, int a)
{
	worldData[id].previousSize = a;
}

void PACKET::SetPreviousSizePlus(int id, int remained)
{
	worldData[id].previousSize += remained;
}

int PACKET::GetRequired(int id)
{
	return worldData[id].packetSize - worldData[id].previousSize;
}

void PACKET::SetPacketBufPlusSize(int id, unsigned char *bufPtr, int required)
{
	memcpy(worldData[id].packetBuf + worldData[id].previousSize,
		bufPtr, required);
}

unsigned char *PACKET::GetPacketBuf(int id)
{
	return worldData[id].packetBuf;
}

void PACKET::SocketClose(int id)
{
	closesocket(worldData[id].s);
}

// vlLock set
void PACKET::SetvlLock_Lock(int id)
{
	worldData[id].vlLock.lock();
}
void PACKET::SetvlLock_Unlock(int id)
{
	worldData[id].vlLock.unlock();
}

// viewList set
int PACKET::GetViewList_count(int id, int i)
{
	return worldData[id].viewList.count(i);
}

void PACKET::SetViewList_insert(int id, int i)
{
	worldData[id].viewList.insert(i);
}

void PACKET::SetViewList_erase(int id, int i)
{
	worldData[id].viewList.erase(i);
}

unordered_set<int> PACKET::GetViewList(int id)
{
	return worldData[id].viewList;
}

// 위치, rot세팅용
void PACKET::SetWorldDataPos(int id, D3DXVECTOR3 pos)
{
	worldData[id].obj.setPos(pos);
}
void PACKET::SetWorldDataPosX(int id, float posx)
{
	worldData[id].obj.setPosX(posx);
}
void PACKET::SetWorldDataPosY(int id, float posy)
{
	worldData[id].obj.setPosY(posy);
}
void PACKET::SetWorldDataPosZ(int id, float posz)
{
	worldData[id].obj.setPosZ(posz);
}

void PACKET::SetWorldDataRot(int id, D3DXVECTOR3 rot)
{
	worldData[id].obj.setRot(rot);
}
void PACKET::SetWorldDataRotX(int id, float rotx)
{
	worldData[id].obj.setRotX(rotx);
}
void PACKET::SetWorldDataRotY(int id, float roty)
{
	worldData[id].obj.setRotY(roty);
}
void PACKET::SetWorldDataRotZ(int id, float rotz)
{
	worldData[id].obj.setRotZ(rotz);
}

// 위치, rot확인용
D3DXVECTOR3 PACKET::GetWorldDataPos(int id)
{
	return worldData[id].obj.getPos();
}

float PACKET::GetWorldDataPosX(int id) 
{
	return worldData[id].obj.getPosX();
}
float PACKET::GetWorldDataPosY(int id)
{
	return worldData[id].obj.getPosY();
}
float PACKET::GetWorldDataPosZ(int id)
{
	return worldData[id].obj.getPosZ();
}

D3DXVECTOR3 PACKET::GetWorldDataRot(int id)
{
	return worldData[id].obj.getRot();
}
float PACKET::GetWorldDataRotX(int id)
{
	return worldData[id].obj.getRotX();
}
float PACKET::GetWorldDataRotY(int id)
{
	return worldData[id].obj.getRotY();
}
float PACKET::GetWorldDataRotZ(int id)
{
	return worldData[id].obj.getRotZ();
}

// 이동속도 세팅, 확인용
void PACKET::SetWorldDataMovement(int id, float movement)
{
	worldData[id].obj.setMovement(movement);
}
float PACKET::GetWorldDataMovement(int id)
{
	return worldData[id].obj.getMovement();
}

// npc, object 확인용
const bool PACKET::isNPC(const int id)
{
	if (id < MAX_USER)
		return false;
	else if (id > NUM_OF_NPC_AND_PLAYER)
		return false;
	return true;
}
const bool PACKET::isObject(const int id)
{
	if (id > NUM_OF_NPC_AND_PLAYER)
		return true;
	return false;
}
/*const bool PACKET::isActive(const int id)	// 타워나 돌등은 움직이지 않을테니까 확인용
{

}*/