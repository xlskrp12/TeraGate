#pragma once






#define MAX_LOADSTRING			100

#define	BUF_SIZE				1024
#define	WM_SOCKET				WM_USER + 1

static HWND main_window_handle = NULL;


struct Player {
	bool exist;
	float x;
	float y;
	float z;

	float roty;

	int hp;
	int maxHp;
	int level;
	int exp;
	int maxExp;
	int type;

};

/*struct Object {
	bool exist;
	float x;
	float y;
	float z;
};
*/


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
	void Release();

	char SERVERIP[20];
	SOCKET g_mysocket;

	

	Player player;
	Player otherPC[MAX_USER];
	Player NPC[NUM_OF_NPC];
	
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

static int		g_myid;