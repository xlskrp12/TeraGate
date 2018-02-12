#pragma once






#define MAX_LOADSTRING			100

#define	BUF_SIZE				1024
#define	WM_SOCKET				WM_USER + 1

static HWND main_window_handle = NULL;


struct S_Object {
	bool exist;
	int id;

	float x;
	float y;
	float z;

	float roty;

	//int playerNum;
	//float hp;
	//float maxHp;
	//
	int teamId;
	//int GateId;

	//float objectSize;
};



class Server
{
	
public:
	Server();
	~Server();

	static Server* instance;

	static Server* GetInstance()
	{
		if (nullptr == instance)
			return instance = new Server;

		return instance;
	}

	
	void err_quit(char *msg);
	void err_display(char *msg);

	void ProcessPacket(char *ptr);
	void ReadPacket(SOCKET sock);

	void Init();
	void Sync(HWND _hwnd);
	void Login();
	void Logout();
	void Release();

	char SERVERIP[20];
	SOCKET g_mysocket;

	int		g_myid=0;

	S_Object player;
	S_Object other_pc[MAX_USER];
	S_Object NPC[NUM_OF_NPC];
	S_Object tower[NUM_OF_TOWER];
	S_Object stone[NUM_OF_STONE];
	
	WSABUF	send_wsabuf;
	char 	send_buffer[BUF_SIZE];
	WSABUF	recv_wsabuf;
	char	recv_buffer[BUF_SIZE];
	char	packet_buffer[BUF_SIZE];
	DWORD		in_packet_size = 0;
	int		saved_packet_size = 0;
	
//	Player NPC[NUM_OF_NPC];
	

	//Object object[NUM_OF_OBJECT];

	bool loginState;

};

//static int		g_myid;

