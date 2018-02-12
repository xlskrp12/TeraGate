#include "stdafx.h"

HANDLE hIocp;
bool isShutdown = false;
//#define _PacketDef PACKET::GetInstance()

WorldData worldData[NUM_OF_OBJECT];


// npc, object 확인용
const bool isNPC(const int id)
{
	if (id < MAX_USER)
		return false;
	else if (id > NUM_OF_NPC_AND_PLAYER)
		return false;
	return true;
}
const bool isObject(const int id)
{
	if (id > NUM_OF_NPC_AND_PLAYER)
		return true;
	return false;
}


//패킷 보내기
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
		cout << id << "번 에서 오류" << endl;
		while (true);
	}
}

void SendPutPlayerPacket(int id, int object)
{
	sc_packet_put_player packet;
	//memset(&packet,0, sizeof(packet));
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_PUT_PLAYER;

	packet.x = worldData[object].obj.getPosX();
	packet.y = worldData[object].obj.getPosY();
	packet.z = worldData[object].obj.getPosZ();

	packet.roty = worldData[object].obj.getRotY();

	packet.teamId = worldData[object].obj.getTeamId();

	cout << object << endl;
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

void SendAttackPacket(int id)
{

}

void LoginPlayer(int id)
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
	enterPacket.x = worldData[id].obj.getPosX();
	enterPacket.y = worldData[id].obj.getPosY();
	enterPacket.z = worldData[id].obj.getPosZ();
	enterPacket.roty = worldData[id].obj.getRotY();

	enterPacket.teamId = worldData[id].obj.getTeamId();

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
void LogoutPlayer(int id)
{

}

void DBThreadStart()
{

}	// 로그인 관련 및 DB관련

void TimerThread()
{

}	//npc관련 및 pc공격과 충돌체크등의 처리를 맡음

void Initialize()
{
	//MinionTimerSet();
	//_PacketDef->Init();
	int radius = 0;
	int radian = 0;

	//for (int i = 0; i < NUM_OF_OBJECT; i++)
	//{
	//	worldData[i].previousSize = 0;
	//	worldData[i].packetSize = 0;
	//	ZeroMemory(worldData[i].packetBuf, sizeof(worldData[i].packetBuf));
	//}

	for (auto i = 0; i < MAX_USER; i++)
	{
		worldData[i].connected = false;
		worldData[i].recvOverlap.operation = OP_RECV;

		worldData[i].obj.setHP(PLAYER_HP);
		worldData[i].obj.setMaxHP(PLAYER_HP);

		worldData[i].obj.setMovement(30.0f);


		radius = 2800;
		if (i % 2 == 0)
		{
			radian = DEGREE_TO_RADIAN(210);
			worldData[i].obj.setPos(radius*cos(radian), 0.f, radius*sin(radian));
			worldData[i].obj.setRotY(0.f);
			worldData[i].obj.setTeam(TEAM_L);

		}
		else
		{
			radian = DEGREE_TO_RADIAN(-30);
			worldData[i].obj.setPos(radius*cos(radian), 0.f, radius*sin(radian));
			worldData[i].obj.setRotY(0.f);
			worldData[i].obj.setTeam(TEAM_R);
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

	_wsetlocale(LC_ALL, L"korean");

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);


	hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

}	// 초기화

void ProcessPacket(int id, unsigned char buf[])
{
	D3DXVECTOR3 _pos = worldData[id].obj.getPos();

	float roty = worldData[id].obj.getRotY();

	float movement = worldData[id].obj.getMovement();

	switch (buf[1])
	{
		case CS_LOGIN:
		{
			//_PacketDef->LoginPlayer(id);
			LoginPlayer(id);
		}
		break;

		case CS_RIGHT_UP:
		{
			roty = 45;
			_pos.x += movement * sin(DEGREE_TO_RADIAN(roty));
			_pos.z += movement * cos(DEGREE_TO_RADIAN(roty));

		}
		break;

		case CS_LEFT_UP:
		{
			roty = -45;
			_pos.x += movement * sin(DEGREE_TO_RADIAN(roty));
			_pos.z += movement * cos(DEGREE_TO_RADIAN(roty));
		}
		break;

		case CS_RIGHT_DOWN:
		{
			roty = 135;

			_pos.x += movement * sin(DEGREE_TO_RADIAN(roty));
			_pos.z += movement * cos(DEGREE_TO_RADIAN(roty));

		}
		break;

		case CS_LEFT_DOWN:
		{
			roty = -135;
			_pos.x += movement * sin(DEGREE_TO_RADIAN(roty));
			_pos.z += movement * cos(DEGREE_TO_RADIAN(roty));
		}
		break;

		case CS_UP:
		{
			roty = 0;
			_pos.x += movement * sin(DEGREE_TO_RADIAN(roty));
			_pos.z += movement * cos(DEGREE_TO_RADIAN(roty));
		}
		break;

		case CS_DOWN:
		{
			roty = -180;
			_pos.x += movement * sin(DEGREE_TO_RADIAN(roty));
			_pos.z += movement * cos(DEGREE_TO_RADIAN(roty));
		}
		break;

		case CS_LEFT:
		{
			roty =  90;
			_pos.x += movement * sin(DEGREE_TO_RADIAN(roty));
			_pos.z += movement * cos(DEGREE_TO_RADIAN(roty));
		}
		break;

		case CS_RIGHT:
		{
			roty =  -90;
			_pos.x += movement * sin(DEGREE_TO_RADIAN(roty));
			_pos.z += movement * cos(DEGREE_TO_RADIAN(roty));
		}
		break;

		case CS_FIRE:
		{

		}
		break;

		case CS_ATTACK:
		{
			
		}
		break;

		case CS_SKILL_E:
		{
			
		}
		break;

		case CS_SHIFT_DOWN:
		{
			
		}
		break;

		case CS_SHIFT_UP:
		{
			
		}
		break;

		default:
			cout << "Unknown type packet received\n";
			while (true);
	
	}

	cout << _pos.x << " , " << _pos.z << endl;
	// PC 이동 처리
	//_PacketDef->SetWorldDataRotY(id, roty);
	worldData[id].obj.setPos(_pos.x, _pos.y, _pos.z);

	if (CS_LEFT_UP == buf[1] || CS_RIGHT_DOWN == buf[1])
	{
		roty += 180;
	}

	if (CS_UP == buf[1] || CS_DOWN == buf[1])
	{
		roty += 90;
	}
	else if (CS_LEFT == buf[1] || CS_RIGHT == buf[1])
	{
		roty -= 90;
	}

	worldData[id].obj.setRotY(roty);

	sc_packet_pos movPacket;
	movPacket.id = id;
	movPacket.size = sizeof(movPacket);
	movPacket.type = SC_POS;
	movPacket.x = _pos.x;
	movPacket.y = _pos.y;
	movPacket.z = _pos.z;
	movPacket.roty = roty;

	SendPacket(id, reinterpret_cast<unsigned char *>(&movPacket));

	// 모든 클라이언트에 뿌려주기
	unordered_set<int> newList;
	for (auto i = 0; i < MAX_USER; ++i)
	{
		if (false == worldData[i].connected)
			continue;
		if (i == id)
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

	for (auto i : removeList)
	{
		SendRemovePlayerPacket(id, i);
	}

	for (auto i : removeList)
	{
		if (true == isNPC(i))
			continue;
		if (true == isObject(i))
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

}	// PC의 움직임 처리

void WorkerThreadStart()
{
	while (0 == isShutdown)
	{
		DWORD ioSize;
		DWORD key;
		OverlappedEx *myOverlap;

		BOOL result = GetQueuedCompletionStatus(hIocp, &ioSize, &key,
			reinterpret_cast<LPOVERLAPPED *>(&myOverlap), INFINITE);

		if (0 == ioSize)
		{
			//_PacketDef->SocketClose(key);

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
				SendPacket(i, reinterpret_cast<unsigned char*>(&disConnected));
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

					cout << remained << endl;

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
}	// 항상 처리되는 스레드

void AcceptThreadStart() 
{
	struct sockaddr_in listen_addr;

	SOCKET acceptSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 
		0, WSA_FLAG_OVERLAPPED);

	memset(&listen_addr, 0, sizeof(listen_addr));

	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = htonl(ADDR_ANY);
	listen_addr.sin_port = htons(MY_SERVER_PORT);

	::bind(acceptSocket, reinterpret_cast<sockaddr *>(&listen_addr), 
		sizeof(listen_addr));

	listen(acceptSocket, 10);

	while (false == isShutdown)
	{
		struct sockaddr_in client_addr;
		int addrSize = sizeof(client_addr);

		SOCKET newClient = WSAAccept(acceptSocket, 
			reinterpret_cast<sockaddr *>(&client_addr), &addrSize,
			NULL, NULL);

		printf("\n client access\n");

		if (INVALID_SOCKET == newClient)
		{
			int errNO = WSAGetLastError();
			errDisplay("Accept::WSAAccept", errNO);
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
			cout << "MAX concurrent User excceded\n";
			closesocket(newClient);
			cout << "close socket" << endl;
			continue;
		}

		cout << newID << endl;

		worldData[newID].s = newClient;

		cout << newID << " accept" << endl;


		CreateIoCompletionPort(reinterpret_cast<HANDLE>(newClient),
			hIocp, newID, 0);


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
	
}	// 서버에 클라를 연결할때 사용하는 스레드

void cleanUp()
{
	WSACleanup();
}

int main()
{
	wcout.imbue(std::locale("kor"));

	vector<thread *> workerThread;
	Timer timer;

	Initialize();

	for (auto i = 0; i < NUM_THREADS; ++i)
	{
		workerThread.push_back(new thread{ WorkerThreadStart });
	}

	thread timerThread{ TimerThread };

	thread acceptThread{ AcceptThreadStart };

	thread DBThread{ DBThreadStart };

	while (false == isShutdown)
	{
		if (timer.isOn()) {}
	}

	for (auto th : workerThread)
	{
		th->join();
		delete th;
	}

	timerThread.join();

	acceptThread.join();

	DBThread.join();

	cleanUp();

	return 0;
}