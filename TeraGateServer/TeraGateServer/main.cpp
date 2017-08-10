#include "packet.h"
#include "ErrorDisplay.h"

HANDLE hIocp;
bool isShutdown = false;

WorldData worldData[NUM_OF_NPC];
TowerData towerData[NUM_OF_NEXUS + NUM_OF_POWERSPOT];

bool viewRange(int a, int b)
{
	int dist = (worldData[a].obj.x - worldData[b].obj.x)
		* (worldData[a].obj.x - worldData[b].obj.x)
		+ (worldData[a].obj.z - worldData[b].obj.z)
		* (worldData[a].obj.z - worldData[b].obj.z);

	return dist <= VIEW_RADIUS * VIEW_RADIUS;
}


const bool IsNPC(const int id)
{
	return id >= NPC_START;
}




void Initialize()
{
	for (auto i = 0; i < MAX_USER; ++i)
	{
		worldData[i].connected = false;
		worldData[i].recvOverlap.operation = OP_RECV;

		worldData[i].obj.HP = PLAYER_HP;
		worldData[i].obj.maxHp = PLAYER_HP;

		worldData[i].obj.x = 0.f;
		worldData[i].obj.y = 0.f;
		worldData[i].obj.z = 0.f;
		
		worldData[i].recvOverlap.wsabuf.buf =
			reinterpret_cast<CHAR *>(worldData[i].recvOverlap.iocpBuffer);

		worldData[i].recvOverlap.wsabuf.len =
			sizeof(worldData[i].recvOverlap.iocpBuffer);
	}
	
	for (auto i = NPC_START; i < NUM_OF_NPC; ++i)
	{
		worldData[i].connected = true;
		worldData[i].obj.isActive = false;
		
		srand(time(NULL));
		worldData[i].obj.x = 80.f;
		worldData[i].obj.y = 0.f;
		worldData[i].obj.z = 0.f;

		worldData[i].obj.roty = 0.f;

		//기본 몬스터
		// 마스터 몬스터용 조건 추가 필요
		worldData[i].obj.HP = MONSTER_HP_STANDARD;
		worldData[i].obj.maxHp = MONSTER_HP_STANDARD;
	}

	for (auto i = TOWER_START; i < NUM_OF_OBJECT + TOWER_START; ++i)
	{
		towerData[i- TOWER_START].exist = true;
		towerData[i- TOWER_START].x = rand() % MAP_WIDTH*10;
		towerData[i- TOWER_START].z = rand() % MAP_HEIGHT*10;

		if (i < TOWER_START + NUM_OF_NEXUS)
		{
			towerData[i- TOWER_START].HP = NEXUS_HP;
			towerData[i- TOWER_START].maxHP = NEXUS_HP;
		}
	}

	_wsetlocale(LC_ALL, L"korean");

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
}

void SendPacket(int id, unsigned char *packet)
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
		while (true);
	}
}

void SendPutPlayerPacket(int id, int object)
{
	sc_packet_put_player packet;
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_PUT_PLAYER;

	packet.x = worldData[object].obj.x;
	packet.y = worldData[object].obj.y;
	packet.z = worldData[object].obj.z;

	packet.roty = worldData[object].obj.roty;

	packet.hp = worldData[object].obj.HP;
	packet.maxHp = worldData[object].obj.maxHp;

	//packet.monsterType = worldData[object].obj.type;

	SendPacket(id, reinterpret_cast<unsigned char *>(&packet));
}

void SendRemovePlayerPacket(int id, int object)
{
	sc_packet_remove_player packet;
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_REMOVE_PLAYER;

	SendPacket(id, reinterpret_cast<unsigned char *>(&packet));
}

void ProcessPacket(int id, unsigned char buf[])
{
	float x = worldData[id].obj.x;
	float y = worldData[id].obj.y;
	float z = worldData[id].obj.z;

	float roty = worldData[id].obj.roty;

	switch (buf[1])
	{
		case CS_LOGIN:
		{
		}
		break;

		case CS_MOUSE_MOVE:
			break;

		case CS_UP:
			roty = 0;
			x += 5.f * sin(DEGREE_TO_RADIAN(roty));
			z += 5.f * cos(DEGREE_TO_RADIAN(roty));
			cout << x << " , " << z << endl;
			break;

		case CS_DOWN:
			roty = -180;
			x += 5.f * sin(DEGREE_TO_RADIAN(roty));
			z += 5.f * cos(DEGREE_TO_RADIAN(roty));
			cout << x << " , " << z << endl;
			break;

		case CS_LEFT:
			roty = 90;
			x += 5.f * sin(DEGREE_TO_RADIAN(roty));
			z += 5.f * cos(DEGREE_TO_RADIAN(roty));
			cout << x << " , " << z << endl;
			break;

		case CS_RIGHT:
			roty = -90;
			x += 5.f * sin(DEGREE_TO_RADIAN(roty));
			z += 5.f * cos(DEGREE_TO_RADIAN(roty));
			cout << x << " , " << z << endl;
			break;

		case CS_ATTACK:
			break;

		case CS_SKILL:
			break;

		default:
			cout << "Unknown type packet received\n";
			while (true);
	}

	//cout << buf[1] << endl;

	/*if (y < 0)
		y = 0;
	if (y > MAP_HEIGHT*10 - 50)
		y = MAP_HEIGHT * 10 - 50;
	if (x < 0)
		x = 0;
	if (x > MAP_WIDTH * 10 - 50)
		x = MAP_WIDTH * 10 - 50;*/

	

	worldData[id].obj.x = x;
	worldData[id].obj.y = y;
	worldData[id].obj.z = z;

	worldData[id].obj.roty = roty;


	sc_packet_pos movPacket;
	movPacket.id = id;
	movPacket.size = sizeof(movPacket);
	movPacket.type = SC_POS;
	movPacket.x = x;
	movPacket.y = y;
	movPacket.z = z;
	movPacket.roty = roty;

	SendPacket(id, reinterpret_cast<unsigned char *>(&movPacket));
	cout << id << "mov" << endl;

	unordered_set<int> newList;
	for (auto i = 0; i < NUM_OF_NPC; ++i)
	{

		if (false == worldData[i].connected)
			continue;
		if (i == id)
			continue;
		if (false == viewRange(i, id))
			continue;

		newList.insert(i);
	}


	for (auto i : newList)
	{
		worldData[id].vlLock.lock();
		if (0 == worldData[id].viewList.count(i))
		{
			worldData[id].viewList.insert(i);
			worldData[id].vlLock.unlock();

			SendPutPlayerPacket(id, i);

			if (IsNPC(i))
			{
				continue;
			}

			worldData[i].vlLock.lock();
			if (0 == worldData[i].viewList.count(id))
			{
				worldData[i].viewList.insert(id);
				worldData[i].vlLock.unlock();

				SendPutPlayerPacket(i, id);
			}
			else
			{
				worldData[i].vlLock.unlock();

				SendPacket(i, reinterpret_cast<unsigned char *>(&movPacket));
			}
		}

		else
		{
			worldData[id].vlLock.unlock();
			movPacket.id = i;

			movPacket.x = worldData[i].obj.x;
			movPacket.y = worldData[i].obj.y;
			movPacket.z = worldData[i].obj.z;
			movPacket.roty = worldData[i].obj.roty;

			SendPacket(id, reinterpret_cast<unsigned char *>(&movPacket));

			if (IsNPC(i))
				continue;

			worldData[i].vlLock.lock();
			if (0 != worldData[i].viewList.count(id))
			{
				worldData[i].viewList.insert(id);
				worldData[i].vlLock.unlock();
				SendPutPlayerPacket(i, id);
			}
			else
			{
				worldData[i].vlLock.unlock();
				SendPacket(i, reinterpret_cast<unsigned char *>(&movPacket));
			}
		}
	}

	unordered_set<int> removeList;

	worldData[id].vlLock.lock();
	for (auto i : worldData[id].viewList)
	{
		if (0 != newList.count(i))
			continue;

		removeList.insert(i);
	}
	for (auto i : removeList)
		worldData[id].viewList.erase(i);

	worldData[id].vlLock.unlock();

	for (auto i : removeList) {
		SendRemovePlayerPacket(id, i);
		
	}


	for (auto i : removeList)
	{
		if (IsNPC(i))
			continue;

		worldData[i].vlLock.lock();
		if (0 != worldData[i].viewList.count(id))
		{
			worldData[i].viewList.erase(id);
			worldData[i].vlLock.unlock();

			SendRemovePlayerPacket(i, id);
		}
		else
			worldData[i].vlLock.unlock();
	}
}

void WorkerThreadStart()
{
	while (false == isShutdown)
	{
		DWORD ioSize;
		DWORD key;
		OverlappedEx *myOverlap;

		BOOL result = GetQueuedCompletionStatus(hIocp, &ioSize, &key,
			reinterpret_cast<LPOVERLAPPED *>(&myOverlap), INFINITE);

		if (0 == ioSize)
		{
			closesocket(worldData[key].s);

			sc_packet_remove_player disConnected;
			disConnected.id = key;
			disConnected.size = sizeof(disConnected);
			disConnected.type = SC_REMOVE_PLAYER;

			for (auto i = 0; i < MAX_USER; ++i)
			{
				if (false == worldData[i].connected)
					continue;
				if (key == i)
					continue;
				SendPacket(i, reinterpret_cast<unsigned char *>(&disConnected));
			}

			worldData[key].connected = false;
			cout << key << "removed" << endl;
		}

		if (OP_RECV == myOverlap->operation)
		{
			unsigned char *bufPtr = worldData[key].recvOverlap.iocpBuffer;
			int remained = ioSize;

			while (0 < remained)
			{
				if (0 == worldData[key].packetSize)
					worldData[key].packetSize = bufPtr[0];

				int required = worldData[key].packetSize - worldData[key].previousSize;

				if (remained >= required)
				{
					memcpy(worldData[key].packetBuf + worldData[key].previousSize,
						bufPtr, required);

					ProcessPacket(key, worldData[key].packetBuf);

					bufPtr += required;
					remained -= required;

					worldData[key].packetSize = 0;
					worldData[key].previousSize = 0;
				}
				else
				{
					memcpy(worldData[key].packetBuf + worldData[key].previousSize,
						bufPtr, remained);

					bufPtr += remained;
					worldData[key].previousSize += remained;
					remained = 0;
				}
			}

			DWORD flags = 0;
			WSARecv(worldData[key].s,
				&worldData[key].recvOverlap.wsabuf,
				1, NULL,
				&flags,
				&worldData[key].recvOverlap.originalOverlapped, NULL);
		}

		else if (OP_SEND == myOverlap->operation)
		{
			delete myOverlap;
		}

		else if (OP_MOVE == myOverlap->operation)
		{
			delete myOverlap;
		}

		else
		{
			cout << "Unknown IOCP Event\n";
			exit(-1);
		}
	}
}

void AcceptThreadStart()
{
	struct sockaddr_in listen_addr;

	SOCKET acceptSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL,
		0, WSA_FLAG_OVERLAPPED);

	memset(&listen_addr, 0, sizeof(listen_addr));

	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = htonl(ADDR_ANY);
	listen_addr.sin_port = htons(MY_SERVER_PORT);

	::bind(acceptSocket, reinterpret_cast<sockaddr *>(&listen_addr), sizeof(listen_addr));

	listen(acceptSocket, 10);

	while (false == isShutdown)
	{
		struct sockaddr_in client_addr;
		int addrSize = sizeof(client_addr);

		SOCKET newClient = WSAAccept(acceptSocket,
			reinterpret_cast<sockaddr *>(&client_addr), &addrSize,
			NULL, NULL);

		printf("\n클라이언트 접속");

		if (INVALID_SOCKET == newClient)
		{
			int errNo = WSAGetLastError();
			errDisplay("Accept::WSAAccept", errNo);
			while (true);
		}

		int newID = -1;
		for (auto i = 0; i < MAX_USER; ++i)
		{
			if (false == worldData[i].connected)
			{
				newID = i;
				break;
			}
		}

		if (-1 == newID)
		{
			cout << "MAX Concurrent User excceded\n";
			closesocket(newClient);
			continue;
		}

		worldData[newID].s = newClient;
		worldData[newID].obj.x = 0.f;
		worldData[newID].obj.y = 0.f;
		worldData[newID].obj.z = 0.f;
		worldData[newID].obj.roty = -180;
		worldData[newID].packetSize = 0;
		worldData[newID].previousSize = 0;
		memset(&worldData[newID].recvOverlap.originalOverlapped, 0,
			sizeof(worldData[newID].recvOverlap.originalOverlapped));

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(newClient),
			hIocp, newID, 0);

		worldData[newID].vlLock.lock();
		worldData[newID].viewList.clear();
		worldData[newID].vlLock.unlock();

		sc_packet_put_player enterPacket;
		enterPacket.id = newID;
		enterPacket.size = sizeof(enterPacket);
		enterPacket.type = SC_PUT_PLAYER;
		enterPacket.x = worldData[newID].obj.x;
		enterPacket.y = worldData[newID].obj.y;
		enterPacket.z = worldData[newID].obj.z;
		enterPacket.roty = worldData[newID].obj.roty;

		SendPacket(newID, reinterpret_cast<unsigned char *>(&enterPacket));
		//worldData[newID].exist = true;

		for (auto i = 0; i < MAX_USER; ++i)
		{
			if (false == worldData[i].connected)
				continue;
			if (i == newID)
				continue;
			if (false == viewRange(i, newID))
				continue;

			worldData[i].vlLock.lock();
			worldData[i].viewList.insert(newID);
			worldData[i].vlLock.unlock();

			SendPacket(i, reinterpret_cast<unsigned char *>(&enterPacket));
		}

		for (auto i = NPC_START; i < NUM_OF_NPC; ++i)
		{
			if (false == worldData[i].connected)
				continue;
			if (i == newID)
				continue;
			if (false == viewRange(i, newID))
				continue;

			worldData[newID].vlLock.lock();
			worldData[newID].viewList.insert(i);
			worldData[newID].vlLock.unlock();

			enterPacket.id = i;
			enterPacket.x = worldData[i].obj.x;
			enterPacket.y = worldData[i].obj.y;
			enterPacket.z = worldData[i].obj.z;

			enterPacket.roty = worldData[i].obj.roty;

			enterPacket.hp = worldData[i].obj.HP;

			SendPacket(newID, reinterpret_cast<unsigned char *>(&enterPacket));
		}

		for (auto i = 0; i < MAX_USER; ++i)
		{
			if (false == worldData[i].connected)
				continue;
			if (i == newID)
				continue;
			if (false == viewRange(i, newID))
				continue;

			worldData[newID].vlLock.lock();
			worldData[newID].viewList.insert(i);
			worldData[newID].vlLock.unlock();

			enterPacket.id = i;
			enterPacket.x = worldData[i].obj.x;
			enterPacket.y = worldData[i].obj.y;
			enterPacket.z = worldData[i].obj.z;
			SendPacket(newID, reinterpret_cast<unsigned char *>(&enterPacket));
		}

		worldData[newID].connected = true;


		DWORD flags = 0;

		int retval = WSARecv(newClient, &worldData[newID].recvOverlap.wsabuf, 1,
			NULL, &flags, &worldData[newID].recvOverlap.originalOverlapped, NULL);
		if (0 != retval)
		{
			int errNo = WSAGetLastError();
			if (WSA_IO_PENDING != errNo)
				errDisplay("Accept::WSARecv", errNo);
		}
	}
}

void cleanUp()
{
	WSACleanup();
}

int main()
{
	wcout.imbue(std::locale("kor"));

	vector<thread *> workerThread;

	Initialize();

	for (auto i = 0; i < NUM_THREADS; ++i)
	{
		workerThread.push_back(new thread{ WorkerThreadStart });
	}

	thread acceptThread{ AcceptThreadStart };
	
	while (false == isShutdown)
	{
		Sleep(1000);
	}

	for (auto th : workerThread)
	{
		th->join();
		delete th;
	}
	acceptThread.join();

	cleanUp();

	return 0;
}