
//#include"Prev.h"
#include"Renderer.h"

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
		if (first_time)
		{
			first_time = false;
			g_myid = id;
		}

		if (id == g_myid)
		{
			player.x = my_packet->x;
			player.y = my_packet->y;
			player.z = my_packet->z;
			player.roty = my_packet->roty;
			player.exist = true;

			player.hp = my_packet->hp;
			player.maxHp = my_packet->maxHp;
			std:: cout << id << "insert" << std::endl;
		}
		else if (id < NPC_START)
		{
			otherPC[id].x = my_packet->x;
			otherPC[id].y = my_packet->y;
			otherPC[id].z = my_packet->z;
			otherPC[id].roty = my_packet->roty;
			otherPC[id].exist = true;

			otherPC[id].hp = my_packet->hp;
			otherPC[id].maxHp = my_packet->maxHp;
			std::cout << id << "insert" << std::endl;
		}
		else
		{
			NPC[id - NPC_START].x = my_packet->x;
			NPC[id - NPC_START].y = my_packet->y;
			NPC[id - NPC_START].z = my_packet->z;
			NPC[id - NPC_START].roty = my_packet->roty;
			NPC[id - NPC_START].exist = true;

			NPC[id - NPC_START].hp = my_packet->hp;
			NPC[id - NPC_START].maxHp = my_packet->maxHp;
		}
	}
	break;

	case SC_POS:
	{
		sc_packet_pos *my_packet = reinterpret_cast<sc_packet_pos *>(ptr);
		int other_id = my_packet->id;
		if (other_id == g_myid)
		{
			player.x = my_packet->x;
			player.y = my_packet->y;
			player.z = my_packet->z;
			player.roty = my_packet->roty;
			std::cout << player.x << " , " << player.z << std::endl;
		}
		else if (other_id < NPC_START)
		{
			otherPC[other_id].x = my_packet->x;
			otherPC[other_id].y = my_packet->y;
			otherPC[other_id].z = my_packet->z;
			otherPC[other_id].roty = my_packet->roty;
		}
		else
		{
			NPC[other_id - NPC_START].x = my_packet->x;
			NPC[other_id - NPC_START].y = my_packet->y;
			NPC[other_id - NPC_START].z = my_packet->z;
			NPC[other_id - NPC_START].roty = my_packet->roty;
			NPC[other_id - NPC_START].type = my_packet->monsterType;
		}
		
	}
	break;

	case SC_REMOVE_PLAYER:
	{
		sc_packet_remove_player *my_packet = reinterpret_cast<sc_packet_remove_player *>(ptr);
		int other_id = my_packet->id;
		if (other_id == g_myid)
		{
			player.exist = false;
		}
		else if (other_id < NPC_START)
		{
			otherPC[other_id].exist = false;
		}
		else
		{
			NPC[other_id - NPC_START].exist = false;
		}
		
	}
	break;

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

	

}

void Server::Sync(HWND _hwnd)
{
	int retval;

	retval = WSAAsyncSelect(g_mysocket, _hwnd, WM_SOCKET, FD_CLOSE | FD_READ);
	if (retval == SOCKET_ERROR)
	{
		err_quit("WSAAsyncSelect()");
	}

	send_wsabuf.buf = send_buffer;
	send_wsabuf.len = BUF_SIZE;
	recv_wsabuf.buf = recv_buffer;
	recv_wsabuf.len = BUF_SIZE;
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
