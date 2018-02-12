
//#include"Prev.h"
//#include"Renderer.h"
#include"SceneManager.h"

Server* Server::instance = nullptr;
void Server::err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL
	);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(&lpMsgBuf);
}
void Server::err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void Server::ProcessPacket(char *ptr)
{
	static bool first_time = true;
	switch (ptr[1])
	{
	case SC_PUT_PLAYER:
	{
		sc_packet_put_player *my_packet = reinterpret_cast<sc_packet_put_player *>(ptr);
		int id = my_packet->id;
		if (id < MAX_USER)
		{
			if (true == first_time)
			{
				first_time = false;
				g_myid = id;
			}
		}

		if (id == g_myid)	//pc
		{
			player.id = id;
			player.x = my_packet->x;
			player.y = my_packet->y;
			player.z = my_packet->z;
			player.roty = my_packet->roty;
			player.teamId = my_packet->teamId;
			//player.GateId = my_packet->gateId;
			player.exist = true;
			//
			//player.hp = my_packet->hp;
			//player.maxHp = my_packet->maxHp;
			//player.objectSize = my_packet->objectSize;

			//player.state = my_packet->state;
			//player.frame = my_packet->frame;

			std:: cout << "player" << " insert" << std::endl;
			std::cout << "my number is  " << id << std::endl;
		}
		else if (id < MAX_USER)		//other pc
		{
			other_pc[id].id = id;
			other_pc[id].x = my_packet->x;
			other_pc[id].y = my_packet->y;
			other_pc[id].z = my_packet->z;
			other_pc[id].roty = my_packet->roty;
			other_pc[id].teamId = my_packet->teamId;
			//other_pc[id].GateId = my_packet->gateId;
			other_pc[id].exist = true;

			//other_pc[id].hp = my_packet->hp;
			//other_pc[id].objectSize = my_packet->objectSize;
			std::cout << id << "insert" << std::endl;
		}
		else if(id < NUM_OF_NPC_AND_PLAYER)	//npc
		{
			//NPC[id - MAX_USER].id = id;
			/*NPC[id - MAX_USER].x = my_packet->x;
			NPC[id - MAX_USER].y = my_packet->y;
			NPC[id - MAX_USER].z = my_packet->z;
			NPC[id - MAX_USER].roty = my_packet->roty;
			NPC[id - MAX_USER].teamId = my_packet->teamId;
			NPC[id - MAX_USER].GateId = my_packet->gateId;
			NPC[id - MAX_USER].exist = true;

			NPC[id - MAX_USER].hp = my_packet->hp;
			NPC[id-MAX_USER].objectSize = my_packet->objectSize;
			std::cout << "NPC " << id - MAX_USER << std::endl;*/
		}
		else if (id < NUM_OF_NPC_AND_PLAYER + NUM_OF_TOWER)	//tower
		{
			//tower[id - NUM_OF_NPC_AND_PLAYER].id = id;
			/*tower[id - NUM_OF_NPC_AND_PLAYER].x = my_packet->x;
			tower[id - NUM_OF_NPC_AND_PLAYER].y = my_packet->y;
			tower[id - NUM_OF_NPC_AND_PLAYER].z = my_packet->z;
			tower[id - NUM_OF_NPC_AND_PLAYER].roty = my_packet->roty;
			tower[id - NUM_OF_NPC_AND_PLAYER].teamId = my_packet->teamId;
			tower[id - NUM_OF_NPC_AND_PLAYER].GateId = my_packet->gateId;
			tower[id - NUM_OF_NPC_AND_PLAYER].exist = true;

			tower[id - NUM_OF_NPC_AND_PLAYER].hp = my_packet->hp;
			tower[id - NUM_OF_NPC_AND_PLAYER].objectSize = my_packet->objectSize;
			std::cout << "Tower " << id - NUM_OF_NPC_AND_PLAYER << std::endl;*/
		}
		else
		{
			/*stone[id - NUM_OF_NPC_AND_PLAYER - NUM_OF_TOWER].x = my_packet->x;
			stone[id - NUM_OF_NPC_AND_PLAYER - NUM_OF_TOWER].y = my_packet->y;
			stone[id - NUM_OF_NPC_AND_PLAYER - NUM_OF_TOWER].z = my_packet->z;
			stone[id - NUM_OF_NPC_AND_PLAYER - NUM_OF_TOWER].roty = my_packet->roty;
			stone[id - NUM_OF_NPC_AND_PLAYER - NUM_OF_TOWER].teamId = my_packet->teamId;
			stone[id - NUM_OF_NPC_AND_PLAYER - NUM_OF_TOWER].GateId = my_packet->gateId;
			stone[id - NUM_OF_NPC_AND_PLAYER - NUM_OF_TOWER].exist = true;

			stone[id - NUM_OF_NPC_AND_PLAYER - NUM_OF_TOWER].hp = my_packet->hp;
			stone[id - NUM_OF_NPC_AND_PLAYER - NUM_OF_TOWER].objectSize = my_packet->objectSize;
			std::cout << "Stone " << id - NUM_OF_NPC_AND_PLAYER - NUM_OF_TOWER << std::endl;
		*/
		}
	}
	break;

	case SC_POS:
	{
		sc_packet_pos *my_packet = reinterpret_cast<sc_packet_pos *>(ptr);
		int other_id = my_packet->id;
		if (other_id == g_myid)		//pc
		{
			player.x = my_packet->x;
			player.y = my_packet->y;
			player.z = my_packet->z;
			player.roty = my_packet->roty;
			//player.state = my_packet->state;
			//player.frame = my_packet->frame;
			//std::cout << "player : "<< player.state << " , " << player.frame <<"move"<< std::endl;
		}
		else if (other_id < MAX_USER)		//other pc
		{
			other_pc[other_id].x = my_packet->x;
			other_pc[other_id].y = my_packet->y;
			other_pc[other_id].z = my_packet->z;
			other_pc[other_id].roty = my_packet->roty;
			//std::cout << "otherplayer" << other_id << other_id << "move" << std::endl;
		}
		else if (other_id < NUM_OF_NPC_AND_PLAYER)		//npc
		{
			NPC[other_id - MAX_USER].x = my_packet->x;
			NPC[other_id - MAX_USER].y = my_packet->y;
			NPC[other_id - MAX_USER].z = my_packet->z;
			NPC[other_id - MAX_USER].roty = my_packet->roty;
		}
		else if(other_id < NUM_OF_NPC_AND_PLAYER + NUM_OF_TOWER)
		{
			tower[other_id - NUM_OF_NPC_AND_PLAYER].x = my_packet->x;
			tower[other_id - NUM_OF_NPC_AND_PLAYER].y = my_packet->y;
			tower[other_id - NUM_OF_NPC_AND_PLAYER].z = my_packet->z;
			tower[other_id - NUM_OF_NPC_AND_PLAYER].roty = my_packet->roty;
		}
		else
		{
			stone[other_id - NUM_OF_NPC_AND_PLAYER - NUM_OF_TOWER].x = my_packet->x;
			stone[other_id - NUM_OF_NPC_AND_PLAYER - NUM_OF_TOWER].y = my_packet->y;
			stone[other_id - NUM_OF_NPC_AND_PLAYER - NUM_OF_TOWER].z = my_packet->z;
			stone[other_id - NUM_OF_NPC_AND_PLAYER - NUM_OF_TOWER].roty = my_packet->roty;
		}
		
	}
	break;

	case SC_REMOVE_PLAYER:
	{
		sc_packet_remove_player *my_packet = reinterpret_cast<sc_packet_remove_player *>(ptr);
		int other_id = my_packet->id;
		if (other_id == g_myid)		//pc
		{
			player.exist = false;
		}
		else if (other_id < MAX_USER)		//other pc
		{
			other_pc[other_id].exist = false;
		}
		else if(other_id < NUM_OF_NPC_AND_PLAYER)		//npc
		{
			NPC[other_id - MAX_USER].exist = false;
		}
		else if (other_id < NUM_OF_NPC_AND_PLAYER + NUM_OF_TOWER)
		{
			tower[other_id - NUM_OF_NPC_AND_PLAYER].exist = false;
		}
		else
		{
			stone[other_id - NUM_OF_NPC_AND_PLAYER - NUM_OF_TOWER].exist = false;
		}
	}
	break;

	/*case SC_ATTACK:
	{
		sc_packet_attack *my_packet = reinterpret_cast<sc_packet_attack *>(ptr);
		int other_id = my_packet->id;
		if (other_id == g_myid)		//pc
		{
			//player.exist = false;
		}
		else if (other_id < MAX_USER)		//other pc
		{
			//other_pc[other_id].exist = false;
		}
		else if (other_id < NUM_OF_NPC_AND_PLAYER)		//npc
		{
			//NPC[other_id - MAX_USER].exist = false;
		}
		else if (other_id < NUM_OF_NPC_AND_PLAYER + NUM_OF_TOWER)
		{
			//tower[other_id - NUM_OF_NPC_AND_PLAYER].exist = false;
			tower[other_id - NUM_OF_NPC_AND_PLAYER].hp = my_packet->hp;
		}
		else
		{
			//stone[other_id - NUM_OF_NPC_AND_PLAYER - NUM_OF_TOWER].exist = false;
		}
	}
	break;*/

	default:
	{
		printf("Unknown PACKET type[%d]\n", ptr[1]);
	}
	break;
	}
}

void Server::ReadPacket(SOCKET sock)
{
	DWORD iobyte, ioflag = 0;

	int retval = WSARecv(sock, &recv_wsabuf, 1, &iobyte, &ioflag, NULL, NULL);
	if (retval)
	{
		int errCode = WSAGetLastError();
		printf("Recv Error[%d]\n", errCode);
	}

	BYTE *ptr = reinterpret_cast<BYTE *>(recv_buffer);

	while (0 != iobyte)
	{
		if (0 == in_packet_size)
			in_packet_size = ptr[0];
		if (iobyte + saved_packet_size >= in_packet_size)
		{
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);

			ProcessPacket(packet_buffer);

			ptr += in_packet_size - saved_packet_size;
			iobyte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;
		}
		else
		{
			memcpy(packet_buffer + saved_packet_size, ptr, iobyte);

			saved_packet_size += iobyte;
			iobyte = 0;
		}
	}
}

void Server::Init()
{
	//g_myid = 0;
	ZeroMemory(&player, sizeof(player));
	ZeroMemory(&other_pc, sizeof(other_pc));
	ZeroMemory(&NPC, sizeof(NPC));
	ZeroMemory(&tower, sizeof(tower));
	ZeroMemory(&stone, sizeof(stone));
	int retval;

	WSADATA wsa;
	if(0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		exit(-1);

	g_mysocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(SOCKADDR_IN));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(MY_SERVER_PORT);

	retval = WSAConnect(g_mysocket, (SOCKADDR *)&serveraddr, sizeof(serveraddr), NULL, NULL, NULL, NULL);

	std::cout << std::endl;
	std::cout << "Connected" << std::endl;
	//std::cout << g_mysocket << std::endl;
}

void Server::Sync(HWND _hwnd)
{
	int retval;

	retval = WSAAsyncSelect(g_mysocket, _hwnd, WM_SOCKET, FD_CLOSE | FD_READ);
	if (retval == SOCKET_ERROR)
	{
		err_quit("WSAAsyncSelect()");
	}

	std::cout << "Sync" << std::endl;
	send_wsabuf.buf = send_buffer;
	send_wsabuf.len = BUF_SIZE;
	recv_wsabuf.buf = recv_buffer;
	recv_wsabuf.len = BUF_SIZE;
}


void Server::Login()
{
	cs_packet_login *packet = reinterpret_cast<cs_packet_login *>(send_buffer);
	packet->size = sizeof(packet);
	send_wsabuf.len = sizeof(packet);
	packet->type = CS_LOGIN;

	int retval;

	DWORD iobyte;
	retval = WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	if (retval)
	{
		int errCode = WSAGetLastError();
		printf("Error while sending packet [%d]", errCode);
	}
}

void Server::Logout()
{
	//--------------------------------------------------------------------------
	cs_packet_login *packet = reinterpret_cast<cs_packet_login *>(send_buffer);
	packet->size = sizeof(packet);
	send_wsabuf.len = sizeof(packet);
	packet->type = CS_LOGOUT;

	int retval;

	DWORD iobyte;
	retval = WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	if (retval)
	{
		int errCode = WSAGetLastError();
		printf("Error while sending packet [%d]", errCode);
	}
	//--------------------------------------------------------------------------
}


void Server::Release()
{
	WSACleanup();
}

Server::Server()
{
}


Server::~Server()
{
}
