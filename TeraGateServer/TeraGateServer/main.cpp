#include "packet.h"
#include "ErrorDisplay.h"
#include "timer.h"
#include "enum.h"

HANDLE hIocp;
bool isShutdown = false;

WorldData worldData[NUM_OF_OBJECT];

priority_queue<eventType, vector<eventType>, mycomp> evtQueue, evtDB;
mutex eqLock, dbLock;

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
	return id >= MAX_USER && id < NUM_OF_NPC_AND_PLAYER;
}
const bool IsActive(const int npc)
{
	return worldData[npc].obj.isActive;
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
		cout << id << endl;
		while (true);
	}
}

void SendPutPlayerPacket(int id, int object)
{
	sc_packet_put_player packet;
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_PUT_PLAYER;

	packet.playerNum = worldData[object].obj.playerNum;

	packet.x = worldData[object].obj.x;
	packet.y = worldData[object].obj.y;
	packet.z = worldData[object].obj.z;

	packet.roty = worldData[object].obj.roty;

	packet.hp = worldData[object].obj.HP;
	packet.maxHp = worldData[object].obj.maxHp;

	packet.teamId = worldData[object].obj.teamID;
	//if(object > MAX_USER)
	packet.gateId = worldData[object].obj.gateID;

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

void LoginPlayer(int id)
{
	cout << "login start" << endl;
	double radius = 2800;
	double radian = 0;
	if (0 == id || 2==id ||4==id)
	{
		radian = DEGREE_TO_RADIAN(210);
		worldData[id].obj.x = radius*cos(radian);
		worldData[id].obj.y = 0.f;
		worldData[id].obj.z = radius*sin(radian);
		worldData[id].obj.roty = 0;
		worldData[id].obj.HP = PLAYER_HP;
		worldData[id].obj.teamID = TEAM_L;
	}
	else
	{
		radian = DEGREE_TO_RADIAN(-30);
		worldData[id].obj.x = radius*cos(radian);
		worldData[id].obj.y = 0.f;
		worldData[id].obj.z = radius*sin(radian);
		worldData[id].obj.roty = 0;
		worldData[id].obj.HP = PLAYER_HP;
		worldData[id].obj.teamID = TEAM_R;
	}

	worldData[id].packetSize = 0;
	worldData[id].previousSize = 0;
	
	memset(&worldData[id].recvOverlap.originalOverlapped, 0,
		sizeof(worldData[id].recvOverlap.originalOverlapped));


	worldData[id].vlLock.lock();
	worldData[id].viewList.clear();
	worldData[id].vlLock.unlock();

	sc_packet_put_player enterPacket;
	enterPacket.id = id;
	enterPacket.size = sizeof(enterPacket);
	enterPacket.type = SC_PUT_PLAYER;
	enterPacket.x = worldData[id].obj.x;
	enterPacket.y = worldData[id].obj.y;
	enterPacket.z = worldData[id].obj.z;
	enterPacket.roty = worldData[id].obj.roty;

	enterPacket.playerNum = id;
	cout << id << endl;

	enterPacket.teamId = worldData[id].obj.teamID;
	if (14 > worldData[id].obj.gateID)
		enterPacket.gateId = worldData[id].obj.gateID;
	else
		enterPacket.gateId = GATE_TOTAL;

	enterPacket.hp = worldData[id].obj.HP;

	SendPacket(id, reinterpret_cast<unsigned char *>(&enterPacket));

	for (auto i = 0; i < MAX_USER; ++i)
	{
		if (false == worldData[i].connected)
			continue;
		if (i == id)
			continue;
		if (false == viewRange(i, id))
			continue;

		worldData[i].vlLock.lock();
		worldData[i].viewList.insert(id);
		worldData[i].vlLock.unlock();

		

		SendPacket(i, reinterpret_cast<unsigned char *>(&enterPacket));
	}

	for (auto i = 0; i < NUM_OF_OBJECT; ++i)
	{
		if (false == worldData[i].connected)
			continue;
		if (i == id)
			continue;
		if (false == viewRange(i, id))
			continue;

		if (IsNPC(i) && (!IsActive(i)))
		{
			worldData[i].obj.isActive = true;
			evtQueue.push(eventType{ i, GetTickCount() + 1000, OP_MOVE });
		}

		worldData[id].vlLock.lock();
		worldData[id].viewList.insert(i);
		worldData[id].vlLock.unlock();

		enterPacket.id = i;
		enterPacket.x = worldData[i].obj.x;
		enterPacket.y = worldData[i].obj.y;
		enterPacket.z = worldData[i].obj.z;

		enterPacket.roty = worldData[i].obj.roty;

		enterPacket.teamId = worldData[i].obj.teamID;
		enterPacket.gateId = worldData[i].obj.gateID;

		enterPacket.hp = worldData[i].obj.HP;

		SendPacket(id, reinterpret_cast<unsigned char *>(&enterPacket));
	}


	worldData[id].connected = true;
	sc_packet_pos firstPacket;
	firstPacket.id = id;
	firstPacket.x = worldData[id].obj.x;
	firstPacket.y = worldData[id].obj.y;
	firstPacket.z = worldData[id].obj.z;
	firstPacket.roty = worldData[id].obj.roty;

	firstPacket.type = SC_POS;
	firstPacket.size = sizeof(firstPacket);

	SendPacket(id, reinterpret_cast<unsigned char *>(&firstPacket));
}

void logoutPlayer(int id)
{
	worldData[id].connected = false;
}

/*bool collisionCheck(int id)
{
	D3DXVECTOR3 aPosBeforeUpdate;//자연스럽게 회전시키기 위해 업데이트 이전 a오브젝트 좌표를 받는다. 얘가 없으면 정면충돌할 때 오브젝트가 180도 반대로 회전하는 경우가 생겨 되게 부자연스럽게 보이는 경우가 있따.
	D3DXVECTOR3 aPos;
	D3DXVECTOR3 bPos;
	D3DXVECTOR3 aPosAfterUpdate; //업데이트 이후 좌표를 받는다.
	float distance;
	float sizeDistance;
	float collisionDistance;
	float rotation;

	aPosBeforeUpdate.x = worldData[id].obj.x;
	aPosBeforeUpdate.y = worldData[id].obj.y;
	aPosBeforeUpdate.z = worldData[id].obj.z;

	for (auto a = 0; a < sizeof(worldData); a++)
	{
		

	}
}*/

void DBThreadStart()
{
	while (1)
	{
		Sleep(1);
		dbLock.lock();
		while (false == evtDB.empty())
		{
			if (evtDB.top().wakeUpTime > GetTickCount())
				break;
			eventType ev = evtDB.top();
			evtDB.pop();
			dbLock.unlock();

			switch (ev.eventID)
			{
				case DB_SELECT_NAME:
				{
					LoginPlayer(ev.objectID);
				}
				break;

				default:
					break;
			}
			dbLock.lock();
		}
		dbLock.unlock();
	}
}

void TimerThread()
{
	while (true)
	{
		Sleep(1);

		eqLock.lock();
		while (false == evtQueue.empty())
		{
			if (evtQueue.top().wakeUpTime > GetTickCount())
				break;
			eventType ev = evtQueue.top();
			evtQueue.pop();
			eqLock.unlock();

			switch (ev.eventID)
			{
			case OP_MOVE:
			{
				OverlappedEx *over = new OverlappedEx;
				over->operation = ev.eventID;

				PostQueuedCompletionStatus(hIocp, 1, ev.objectID,
					&(over->originalOverlapped));
			}
			break;

			case EVENT_MOVE:
			{
				OverlappedEx *over = new OverlappedEx;
				over->operation = OP_MOVE;

				PostQueuedCompletionStatus(hIocp, 1, ev.objectID,
					reinterpret_cast<LPOVERLAPPED>(over));
			}
			break;

			default:
				break;
			}
			eqLock.lock();
		}
		eqLock.unlock();
	}
}

void Initialize()
{
	int radius = 0;
	int radian = 0;

	int count = 0;
	for (auto i = 0; i < MAX_USER; ++i)
	{
		worldData[i].connected = false;
		worldData[i].recvOverlap.operation = OP_RECV;

		worldData[i].obj.HP = PLAYER_HP;
		worldData[i].obj.maxHp = PLAYER_HP;

		worldData[i].obj.playerNum = i;

		if (0 == i || 2 == i || 4 == i)
		{
			radian = DEGREE_TO_RADIAN(210);
			worldData[i].obj.x = radius*cos(radian);
			worldData[i].obj.y = 0.f;
			worldData[i].obj.z = radius*sin(radian);
			worldData[i].obj.roty = 0.f;

			worldData[i].obj.teamID = TEAM_L;
		}
		else
		{
			radian = DEGREE_TO_RADIAN(-30);
			worldData[i].obj.x = radius*cos(radian);
			worldData[i].obj.y = 0.f;
			worldData[i].obj.z = radius*sin(radian);
			worldData[i].obj.roty = 0.f;

			worldData[i].obj.teamID = TEAM_R;
		}


		worldData[i].recvOverlap.wsabuf.buf =
			reinterpret_cast<CHAR *>(worldData[i].recvOverlap.iocpBuffer);

		worldData[i].recvOverlap.wsabuf.len =
			sizeof(worldData[i].recvOverlap.iocpBuffer);

		cout << (i) / 140.f * 100.f << "%  초기화 완료 , " << count << endl;
		count++;
	}

	
	for (auto i = MAX_USER; i < NUM_OF_NPC_AND_PLAYER; ++i)
	{
		worldData[i].connected = true;
		worldData[i].obj.isActive = false;
		worldData[i].obj.playerNum = 1000;

		//기본 몬스터
		// 마스터 몬스터용 조건 추가 필요
		worldData[i].obj.HP = MONSTER_HP_STANDARD;
		worldData[i].obj.maxHp = MONSTER_HP_STANDARD;

		//NPC_Player
		radius = 2800;
		if (i < MAX_USER + NUM_NPC_PLAYER)
		{
			radian = DEGREE_TO_RADIAN(90);
			worldData[i].obj.x = radius*cos(radian);
			worldData[i].obj.y = 0;
			worldData[i].obj.z = radius*sin(radian);
			worldData[i].obj.roty = RANDOM_M(0, 360);
			worldData[i].obj.teamID = TEAM_N;
			worldData[i].obj.gateID = GATE_NC;
		}
		//L팀
		//else if (i < 30 + NPC_START + NUM_NPC_PLAYER)
		else if (i < 10 + MAX_USER + NUM_NPC_PLAYER)
		{
			radian = DEGREE_TO_RADIAN(210);
			worldData[i].obj.x = radius*cos(radian);
			worldData[i].obj.y = 0;
			worldData[i].obj.z = radius*sin(radian);
			worldData[i].obj.roty = RANDOM_M(0, 360);
			worldData[i].obj.teamID = TEAM_L;
			worldData[i].obj.gateID = GATE_LC;
		}

		//if (i < 60 + NPC_START + NUM_NPC_PLAYER )
		else if (i < 20 + MAX_USER + NUM_NPC_PLAYER)
		{
			radian = DEGREE_TO_RADIAN(215);
			worldData[i].obj.x = radius*cos(radian);
			worldData[i].obj.y = 0;
			worldData[i].obj.z = radius*sin(radian);
			worldData[i].obj.roty = RANDOM_M(0, 360);
			worldData[i].obj.teamID = TEAM_L;
			worldData[i].obj.gateID = GATE_LR;
		}

		//if (i < 90 + NPC_START + NUM_NPC_PLAYER)
		else if (i < 30 + MAX_USER + NUM_NPC_PLAYER)
		{
			radian = DEGREE_TO_RADIAN(205);
			worldData[i].obj.x = radius*cos(radian);
			worldData[i].obj.y = 0;
			worldData[i].obj.z = radius*sin(radian);
			worldData[i].obj.roty = RANDOM_M(0, 360);
			worldData[i].obj.teamID = TEAM_L;
			worldData[i].obj.gateID = GATE_LN;
		}


		//R팀
		//if (i < 120 + NPC_START + NUM_NPC_PLAYER)
		else if (i < 40 + MAX_USER + NUM_NPC_PLAYER)
		{
			radian = DEGREE_TO_RADIAN(-30);
			worldData[i].obj.x = radius*cos(radian);
			worldData[i].obj.y = 0;
			worldData[i].obj.z = radius*sin(radian);
			worldData[i].obj.roty = RANDOM_M(0, 360);
			worldData[i].obj.teamID = TEAM_R;
			worldData[i].obj.gateID = GATE_RC;
		}

		//if (i < 150 + NPC_START + NUM_NPC_PLAYER )
		else if (i < 50 + MAX_USER + NUM_NPC_PLAYER)
		{
			radian = DEGREE_TO_RADIAN(-35);
			worldData[i].obj.x = radius*cos(radian);
			worldData[i].obj.y = 0;
			worldData[i].obj.z = radius*sin(radian);
			worldData[i].obj.roty = RANDOM_M(0, 360);
			worldData[i].obj.teamID = TEAM_R;
			worldData[i].obj.gateID = GATE_RL;
		}

		//if (i < 180 + NPC_START + NUM_NPC_PLAYER )
		else if (i < 60 + MAX_USER + NUM_NPC_PLAYER)
		{
			radian = DEGREE_TO_RADIAN(-25);
			worldData[i].obj.x = radius*cos(radian);
			worldData[i].obj.y = 0;
			worldData[i].obj.z = radius*sin(radian);
			worldData[i].obj.roty = RANDOM_M(0, 360);
			worldData[i].obj.teamID = TEAM_R;
			worldData[i].obj.gateID = GATE_RN;
		}


		//N팀
		//if (i < 210 + NPC_START + NUM_NPC_PLAYER )
		else if (i < 70 + MAX_USER + NUM_NPC_PLAYER)
		{
			radian = DEGREE_TO_RADIAN(90);
			worldData[i].obj.x = radius*cos(radian);
			worldData[i].obj.y = 0;
			worldData[i].obj.z = radius*sin(radian);
			worldData[i].obj.roty = RANDOM_M(0, 360);
			worldData[i].obj.teamID = TEAM_N;
			worldData[i].obj.gateID = GATE_NC;
		}

		//if (i < 240 + NPC_START + NUM_NPC_PLAYER )
		else if (i < 80 + MAX_USER + NUM_NPC_PLAYER)
		{
			radian = DEGREE_TO_RADIAN(95);
			worldData[i].obj.x = radius*cos(radian);
			worldData[i].obj.y = 0;
			worldData[i].obj.z = radius*sin(radian);
			worldData[i].obj.roty = RANDOM_M(0, 360);
			worldData[i].obj.teamID = TEAM_N;
			worldData[i].obj.gateID = GATE_NL;
		}

		//if (i < 270 + NPC_START + NUM_NPC_PLAYER )
		else if (i < 90 + MAX_USER + NUM_NPC_PLAYER)
		{
			radian = DEGREE_TO_RADIAN(85);
			worldData[i].obj.x = radius*cos(radian);
			worldData[i].obj.y = 0;
			worldData[i].obj.z = radius*sin(radian);
			worldData[i].obj.roty = RANDOM_M(0, 360);
			worldData[i].obj.teamID = TEAM_N;
			worldData[i].obj.gateID = GATE_NR;
		}
		cout << (i) / 140.f * 100.f << "%  초기화 완료 , " << count << endl;
		count++;
	}

	radius = 3000;
	for (auto i = NUM_OF_NPC_AND_PLAYER; i < NUM_OF_NPC_AND_PLAYER + NUM_OF_TOWER; ++i)
	{
		worldData[i].connected = true;
		worldData[i].obj.HP = NEXUS_HP;
		// NPC Team 
		if (100 == i)
		{
			worldData[i].obj.x = 0.f;
			worldData[i].obj.y = 0.f;
			worldData[i].obj.z = 0.f;
			worldData[i].obj.teamID = TEAM_N;
			worldData[i].obj.gateID = GATE_C;
		}
		else if (101 == i )	// 메인 게이트
		{
			worldData[i].obj.x = radius*cos(DEGREE_TO_RADIAN(90));
			worldData[i].obj.y = 0.f;
			worldData[i].obj.z = radius*sin(DEGREE_TO_RADIAN(90));
			worldData[i].obj.teamID = TEAM_N;
			worldData[i].obj.gateID = GATE_N;
		}
		else if (102 == i)
		{
			worldData[i].obj.x = radius*cos(DEGREE_TO_RADIAN(90)) / 2;
			worldData[i].obj.y = 0.f;
			worldData[i].obj.z = radius*sin(DEGREE_TO_RADIAN(90)) / 2;
			worldData[i].obj.teamID = TEAM_N;
			worldData[i].obj.gateID = GATE_NC;
		}
		else if (103 == i )
		{
			worldData[i].obj.x = radius*cos(DEGREE_TO_RADIAN(50));
			worldData[i].obj.y = 0.f;
			worldData[i].obj.z = radius*sin(DEGREE_TO_RADIAN(50));
			worldData[i].obj.teamID = TEAM_N;
			worldData[i].obj.gateID = GATE_NR;
		}
		else if (104 == i )
		{
			worldData[i].obj.x = radius*cos(DEGREE_TO_RADIAN(130));
			worldData[i].obj.y = 0.f;
			worldData[i].obj.z = radius*sin(DEGREE_TO_RADIAN(130));
			worldData[i].obj.teamID = TEAM_N;
			worldData[i].obj.gateID = GATE_NL;
		}


		//Left Team
		else if (105 == i )	// 메인 게이트
		{
			worldData[i].obj.x = radius*cos(DEGREE_TO_RADIAN(210));
			worldData[i].obj.y = 0.f;
			worldData[i].obj.z = radius*sin(DEGREE_TO_RADIAN(210));
			worldData[i].obj.teamID = TEAM_L;
			worldData[i].obj.gateID = GATE_L;
		}
		else if (106 == i )
		{
			worldData[i].obj.x = radius*cos(DEGREE_TO_RADIAN(210)) / 2;
			worldData[i].obj.y = 0.f;
			worldData[i].obj.z = radius*sin(DEGREE_TO_RADIAN(210)) / 2;
			worldData[i].obj.teamID = TEAM_L;
			worldData[i].obj.gateID = GATE_LC;
		}
		else if (107 == i )
		{
			worldData[i].obj.x = radius*cos(DEGREE_TO_RADIAN(170));
			worldData[i].obj.y = 0.f;
			worldData[i].obj.z = radius*sin(DEGREE_TO_RADIAN(170));
			worldData[i].obj.teamID = TEAM_L;
			worldData[i].obj.gateID = GATE_LN;
		}
		else if (108 == i )
		{
			worldData[i].obj.x = radius*cos(DEGREE_TO_RADIAN(250));
			worldData[i].obj.y = 0.f;
			worldData[i].obj.z = radius*sin(DEGREE_TO_RADIAN(250));
			worldData[i].obj.teamID = TEAM_L;
			worldData[i].obj.gateID = GATE_LR;
		}

		//Right Team
		else if (109 == i )
		{
			worldData[i].obj.x = radius*cos(DEGREE_TO_RADIAN(-30));
			worldData[i].obj.y = 0.f;
			worldData[i].obj.z = radius*sin(DEGREE_TO_RADIAN(-30));
			worldData[i].obj.teamID = TEAM_R;
			worldData[i].obj.gateID = GATE_R;
		}
		else if (110 == i )
		{
			worldData[i].obj.x = radius*cos(DEGREE_TO_RADIAN(-30)) / 2;
			worldData[i].obj.y = 0.f;
			worldData[i].obj.z = radius*sin(DEGREE_TO_RADIAN(-30)) / 2;
			worldData[i].obj.teamID = TEAM_R;
			worldData[i].obj.gateID = GATE_RC;
		}
		else if (111 == i )
		{
			worldData[i].obj.x = radius*cos(DEGREE_TO_RADIAN(10));
			worldData[i].obj.y = 0.f;
			worldData[i].obj.z = radius*sin(DEGREE_TO_RADIAN(10));
			worldData[i].obj.teamID = TEAM_R;
			worldData[i].obj.gateID = GATE_RN;
		}
		else if (112 == i )
		{
			worldData[i].obj.x = radius*cos(DEGREE_TO_RADIAN(-70));
			worldData[i].obj.y = 0.f;
			worldData[i].obj.z = radius*sin(DEGREE_TO_RADIAN(-70));
			worldData[i].obj.teamID = TEAM_R;
			worldData[i].obj.gateID = GATE_RL;
		}

		cout << (i) / 140.f * 100.f << "%  초기화 완료 , " << count << endl;
		count++;
	}


	// 바위 * 9
	for (auto i = NUM_OF_NPC_AND_PLAYER + NUM_OF_TOWER; i < 4 + NUM_OF_NPC_AND_PLAYER + NUM_OF_TOWER; i++)
	{
		radius = 1500;
		radian = DEGREE_TO_RADIAN(10 + i * 16);//10~50도 
		worldData[i].obj.x = radius*cos(radian);
		worldData[i].obj.y = RANDOM_M(-50, 0);
		worldData[i].obj.z = radius*sin(radian);
		worldData[i].obj.roty = RANDOM_M(0, 360);

		cout << (i) / 140.f * 100.f << "%  초기화 완료 , " << count << endl;
		count++;
	}
	for (auto i = 4 + NUM_OF_NPC_AND_PLAYER + NUM_OF_TOWER; i< 9 + NUM_OF_NPC_AND_PLAYER + NUM_OF_TOWER; i++)
	{
		radius = 1700;
		radian = DEGREE_TO_RADIAN(i * 15);//0~60도 
		worldData[i].obj.x = radius*cos(radian);
		worldData[i].obj.y = RANDOM_M(-50, 0);
		worldData[i].obj.z = radius*sin(radian);
		worldData[i].obj.roty = RANDOM_M(0, 360);

		cout << (i) / 140.f * 100.f << "%  초기화 완료 , " << count << endl;
		count++;
	}
	// 바위 * 9
	for (auto i = 9 + NUM_OF_NPC_AND_PLAYER + NUM_OF_TOWER; i< 13 + NUM_OF_NPC_AND_PLAYER + NUM_OF_TOWER; i++)
	{
		radius = 1500;
		radian = DEGREE_TO_RADIAN(-70 - i * 16);//-70~-110도 
		worldData[i].obj.x = radius*cos(radian);
		worldData[i].obj.y = RANDOM_M(-50, 0);
		worldData[i].obj.z = radius*sin(radian);
		worldData[i].obj.roty = RANDOM_M(0, 360);

		cout << (i) / 140.f * 100.f << "%  초기화 완료 , " << count << endl;
		count++;
	}
	for (auto i = 13 + NUM_OF_NPC_AND_PLAYER + NUM_OF_TOWER; i< 18 + NUM_OF_NPC_AND_PLAYER + NUM_OF_TOWER; i++)
	{
		radius = 1700;
		radian = DEGREE_TO_RADIAN(-60 - i * 15);//-60~-120도 
		worldData[i].obj.x = radius*cos(radian);
		worldData[i].obj.y = RANDOM_M(-50, 0);
		worldData[i].obj.z = radius*sin(radian);
		worldData[i].obj.roty = RANDOM_M(0, 360);

		cout << (i) / 140.f * 100.f << "%  초기화 완료 , " << count << endl;
		count++;
	}
	// 바위 * 9
	for (auto i = 18 + NUM_OF_NPC_AND_PLAYER + NUM_OF_TOWER; i< 22 + NUM_OF_NPC_AND_PLAYER + NUM_OF_TOWER; i++)
	{
		radius = 1500;
		radian = DEGREE_TO_RADIAN(130 + i * 16);//130~170도
		worldData[i].obj.x = radius*cos(radian);
		worldData[i].obj.y = RANDOM_M(-50, 0);
		worldData[i].obj.z = radius*sin(radian);
		worldData[i].obj.roty = RANDOM_M(0, 360);

		cout << (i) / 140.f * 100.f << "%  초기화 완료 , " << count << endl;
		count++;
	}
	for (auto i = 22 + NUM_OF_NPC_AND_PLAYER + NUM_OF_TOWER; i< 27 + NUM_OF_NPC_AND_PLAYER + NUM_OF_TOWER; i++)
	{
		radius = 1700;
		radian = DEGREE_TO_RADIAN(120 + i * 15);//-60~-120도 
		worldData[i].obj.x = radius*cos(radian);
		worldData[i].obj.y = RANDOM_M(-50, 0);
		worldData[i].obj.z = radius*sin(radian);
		worldData[i].obj.roty = RANDOM_M(0, 360);

		cout << (i) / 140.f * 100.f << "%  초기화 완료 , " << count << endl;
		count++;
	}

	

	_wsetlocale(LC_ALL, L"korean");

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
}


void ProcessPacket(int id, unsigned char buf[])
{
	float x = worldData[id].obj.x;
	float y = worldData[id].obj.y;
	float z = worldData[id].obj.z;

	float roty = worldData[id].obj.roty;

	float movement = 10.f;

	switch (buf[1])
	{
		case CS_LOGIN:
		{
			cs_packet_login *my_packet = reinterpret_cast<cs_packet_login *>(buf);

			evtDB.push(eventType{ id,GetTickCount(),DB_SELECT_NAME });
			cout << "CS_LOGIN 확인" << endl;
		}
		break;

		case CS_LOGOUT:
		{
			//cs_packet_login *my_packet = reinterpret_cast<cs_packet_login *>(buf);
			logoutPlayer(id);

		}
		break;

		case CS_RIGHT_UP:
		{
			roty = 45;
			x += movement * sin(DEGREE_TO_RADIAN(roty));
			z += movement * cos(DEGREE_TO_RADIAN(roty));
			cout << x << " , " << z << endl;
		}
		break;

		case CS_LEFT_UP:
		{
			roty = -45;
			x += movement * sin(DEGREE_TO_RADIAN(roty));
			z += movement * cos(DEGREE_TO_RADIAN(roty));
			cout << x << " , " << z << endl;
		}
		break;

		case CS_RIGHT_DOWN:
		{
			roty = 135;
			
			x += movement * sin(DEGREE_TO_RADIAN(roty));
			z += movement * cos(DEGREE_TO_RADIAN(roty));
			
			cout << x << " , " << z << endl;
		}
		break;

		case CS_LEFT_DOWN:
		{
			roty = -135;
			x += movement * sin(DEGREE_TO_RADIAN(roty));
			z += movement * cos(DEGREE_TO_RADIAN(roty));
			cout << x << " , " << z << endl;
		}
		break;

		case CS_UP:
		{
			roty = 0;
			x += movement * sin(DEGREE_TO_RADIAN(roty));
			z += movement * cos(DEGREE_TO_RADIAN(roty));
			cout << x << " , " << z << endl;
		}
		break;

		case CS_DOWN:
		{
			roty = -180;
			x += movement * sin(DEGREE_TO_RADIAN(roty));
			z += movement * cos(DEGREE_TO_RADIAN(roty));
			cout << x << " , " << z << endl;
		}
		break;

		case CS_LEFT:
		{
			roty = 90;
			x += movement * sin(DEGREE_TO_RADIAN(roty));
			z += movement * cos(DEGREE_TO_RADIAN(roty));
			cout << x << " , " << z << endl;
		}
		break;

		case CS_RIGHT:
		{
			roty = -90;
			x += movement * sin(DEGREE_TO_RADIAN(roty));
			z += movement * cos(DEGREE_TO_RADIAN(roty));
			cout << x << " , " << z << endl;
		}
		break;

		case CS_ATTACK:
		{

		}
			break;

		case CS_SKILL_E:
		{
			movement = 20.0f;
			x += 50 * movement * cos(DEGREE_TO_RADIAN(roty));
			z += 50 * movement * sin(DEGREE_TO_RADIAN(roty));
		}
			break;

		case CS_SHIFT_DOWN:
		{
			movement = 20.0f;
		}
		break;

		case CS_SHIFT_UP:
		{
			movement = 10.0f;
		}
		break;

		default:
			cout << "Unknown type packet received\n";
			while (true);
	}

	if (pow((int)MAP_SIZE, 2) <= pow(x, 2) + pow(z, 2))
	{
		x -= movement * sin(DEGREE_TO_RADIAN(roty));
		z -= movement * cos(DEGREE_TO_RADIAN(roty));
	}
	

	worldData[id].obj.x = x;
	worldData[id].obj.y = y;
	worldData[id].obj.z = z;

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
	for (auto i = 0; i < NUM_OF_OBJECT; ++i)
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

			if (true == IsNPC(i))
			{
				if (!IsActive(i))
				{
					worldData[i].obj.isActive = true;

					evtQueue.push(eventType{ i, GetTickCount() + 1000, OP_MOVE });
					//cout << "npc move" << endl;
				}
				continue;
			}

			worldData[i].vlLock.lock();
			if (0 == worldData[i].viewList.count(id))
			{
				worldData[i].viewList.insert(id);
				worldData[i].vlLock.unlock();

				//cout << "a" << endl;

				SendPutPlayerPacket(i, id);
			}
			else
			{
				worldData[i].vlLock.unlock();

				//cout << "b" << endl;
				SendPacket(i, reinterpret_cast<unsigned char *>(&movPacket));
			}
		}

		else
		{
			worldData[id].vlLock.unlock();
			/*movPacket.id = i;

			movPacket.x = worldData[i].obj.x;
			movPacket.y = worldData[i].obj.y;
			movPacket.z = worldData[i].obj.z;
			movPacket.roty = worldData[i].obj.roty;

			//cout << "c" << endl;
			SendPacket(id, reinterpret_cast<unsigned char *>(&movPacket));*/

			if (IsNPC(i))
				continue;

			worldData[i].vlLock.lock();
			if (1 == worldData[i].viewList.count(id))
			{
				worldData[i].vlLock.unlock();
				//cout << "e" << endl;
				SendPacket(i, reinterpret_cast<unsigned char *>(&movPacket));
			}
			else
			{
				worldData[i].viewList.insert(id);
				worldData[i].vlLock.unlock();

				//cout << "d" << endl;
				SendPutPlayerPacket(i, id);
				
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

	
	/*for (auto i = 0; i < MAX_USER; ++i)
	{
		if (false == worldData[i].connected)
			continue;
		if (i == id)
			continue;
		if (false == viewRange(i, id))
			continue;

		movPacket.id = id;
		movPacket.x = worldData[id].obj.x;
		movPacket.y = worldData[id].obj.y;
		movPacket.z = worldData[id].obj.z;

		//cout << "f" << endl;
		SendPacket(i, reinterpret_cast<unsigned char *>(&movPacket));
	}

	for (auto i = 0; i < MAX_USER; ++i)
	{
		if (false == worldData[i].connected)
			continue;
		if (i == id)
			continue;
		if (false == viewRange(i, id))
			continue;

		movPacket.id = i;
		movPacket.x = worldData[i].obj.x;
		movPacket.y = worldData[i].obj.y;
		movPacket.z = worldData[i].obj.z;

		//cout << "g" << endl;
		SendPacket(id, reinterpret_cast<unsigned char *>(&movPacket));
	}*/
}

void MoveNPC(int id)
{
	if (false == worldData[id].obj.isActive)
		return;

	D3DXVECTOR3 _des(0.f,0.f,0.f);

	float posx = worldData[id].obj.x;
	float posz = worldData[id].obj.z;
	float roty = worldData[id].obj.roty;

	float distance = 0.f;
	float movement = 20.0f;

	unordered_set<int> viewList;
	for (auto i = 0; i < MAX_USER; i++)
	{
		if (false == worldData[i].connected)
			continue;
		if (false == viewRange(id, i))
			continue;
		viewList.insert(i);
	}

	switch (worldData[id].obj.gateID)
	{
	case GATE_C:
		_des = GATE_C_POS;
		break;
	case GATE_L:
		_des = GATE_L_POS;
		break;
	case GATE_LN:
		_des = GATE_LN_POS;
		break;
	case GATE_LR:
		_des = GATE_LR_POS;
		break;
	case GATE_LC:
		_des = GATE_LC_POS;
		break;
	case GATE_R:
		_des = GATE_R_POS;
		break;
	case GATE_RN:
		_des = GATE_RN_POS;
		break;
	case GATE_RL:
		_des = GATE_RL_POS;
		break;
	case GATE_RC:
		_des = GATE_RC_POS;
		break;
	case GATE_N:
		_des = GATE_N_POS;
		break;
	case GATE_NL:
		_des = GATE_NL_POS;
		break;
	case GATE_NR:
		_des = GATE_NR_POS;
		break;
	case GATE_NC:
		_des = GATE_NC_POS;
		break;
	}// switch(_gateID)
	roty = RADIAN_TO_DEGREE(atan2(_des.z - posz, _des.x - posx));

	//move
	distance = sqrt(pow(_des.x - posx, 2) + pow(_des.z - posz, 2));
	if (distance <= 0.1)
	{
		posx = _des.x;
		posz = _des.z;
	}
	else
	{
		posx += movement * cos(DEGREE_TO_RADIAN(roty));
		posz += movement * sin(DEGREE_TO_RADIAN(roty));
	}

	//out of map
	if (pow(MAP_SIZE, 2) <= pow(posx, 2) + pow(posz, 2))
	{
		posx = MAP_SIZE * cos(atan2(posz, posx));
		posz = MAP_SIZE * sin(atan2(posz, posx));
		roty = RADIAN_TO_DEGREE(atan2(posz, posx));
	}


	worldData[id].obj.x = posx;
	worldData[id].obj.z = posz;
	worldData[id].obj.roty = roty;


	//if (105 == id)
		//cout <<id <<"번npc"<< worldData[id].obj.x << "  ,   " << worldData[id].obj.z << endl;

	unordered_set<int> newList;
	for (auto i = 0; i < MAX_USER; i++)
	{
		if (false == worldData[i].connected)
			continue;
		if (false == viewRange(id, i))
			continue;
		newList.insert(i);
	}

	for (auto i : viewList)
	{
		if (0 == newList.count(i))
		{
			worldData[i].vlLock.lock();
			worldData[i].viewList.erase(id);
			worldData[i].vlLock.unlock();

			sc_packet_remove_player packet;
			packet.id = id;
			packet.size = sizeof(packet);
			packet.type = SC_REMOVE_PLAYER;

			SendPacket(i, reinterpret_cast<unsigned char *>(&packet));
		}
		else
		{
			/*if (worldData[id].obj.hp == 0 || worldData[id].obj.hp > 60000)
			{
				worldData[i].vlLock.lock();
				worldData[i].viewList.erase(id);
				worldData[i].vlLock.unlock();

				sc_packet_remove_player packet;
				packet.id = id;
				packet.size = sizeof(packet);
				packet.type = SC_REMOVE_PLAYER;

				SendRemovePlayerPacket(i, id);
				worldData[id].connected = false;
				worldData[id].obj.isActive = false;

				evtQueue.push(eventType{ id,GetTickCount() + 30000,EVENT_MONSTER_REGEN });
			}*/

			sc_packet_pos packet;
			packet.id = id;
			packet.size = sizeof(packet);
			packet.type = SC_POS;
			packet.x = posx;
			packet.y = worldData[id].obj.y;
			packet.z = posz;

			packet.roty = roty;

			//packet.monsterType = worldData[id].obj.type;


			//if (AGRESSIVE == worldData[id].pl.type || BOSS == worldData[i].pl.type)
				//evtQueue.push(eventType{ id,GetTickCount() + 1000, EVENT_MOVE });
			//evtQueue.push(eventType{ id,GetTickCount() + 2000, EVENT_MONSTER_ATK });

			evtQueue.push(eventType{ id,GetTickCount() + 1000, EVENT_MOVE });

			SendPacket(i, reinterpret_cast<unsigned char *>(&packet));
		}
	}

	for (auto i : newList)
	{
		if (0 != viewList.count(i))
			continue;

		sc_packet_put_player packet;
		packet.id = id;
		packet.size = sizeof(packet);
		packet.type = SC_PUT_PLAYER;

		SendPacket(i, reinterpret_cast<unsigned char *>(&packet));
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
			MoveNPC(key);
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

		printf("\n클라이언트 접속\n");

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
		cout << newID << endl;

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
}

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