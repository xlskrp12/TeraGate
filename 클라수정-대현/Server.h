#pragma once

#include"Prev.h"



#pragma comment(lib, "ws2_32")
#pragma comment(lib,"msimg32")
#pragma warning(disable:4996)

#define MAX_LOADSTRING			100

#define	BUF_SIZE				1024
#define	WM_SOCKET				WM_USER + 1

//HWND main_window_handle = NULL;


class Server
{
public:
	Server();
	~Server();

	void err_quit(char *msg);
	void err_display(char *msg);

	void ProcessPacket(char *ptr);
	void ReadPacket(SOCKET sock);

	void Init(HWND _hwnd);
	void Release();

	char SERVERIP[20];

private:
	SOCKET g_mysocket;
	WSABUF	send_wsabuf;
	char 	send_buffer[BUF_SIZE];
	WSABUF	recv_wsabuf;
	char	recv_buffer[BUF_SIZE];
	char	packet_buffer[BUF_SIZE];
	DWORD		in_packet_size = 0;
	int		saved_packet_size = 0;
	int		g_myid;

	int		g_left_x = 0;
	int     g_top_y = 0;

	bool loginState;

	

};

