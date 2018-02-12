#include"prev.h"
#include"object.h"
#include"errorDisplay.h"

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

class PACKET 
{
public:

	static PACKET* instance;

	static PACKET* GetInstance()
	{
		if (nullptr == instance)
			return instance = new PACKET;

		return instance;
	}

	void Init();
	void Init(int id);	// id하나가 로그아웃했을경우 사용

	void SetWSARecv(SOCKET newClient, int id);
	int GetWSARecvRetval();

	//패킷 보내기
	void SendPacket(int id, unsigned char *packet);	
	void SendPutPlayerPacket(int id, int object);
	void SendRemovePlayerPacket(int id, int objcet);
	void SendAttackPacket(int id);

	// 로그인, 로그아웃
	void LoginPlayer(int id);
	void LogoutPlayer(int id);

	//worldData 내용 확인
	void SetWorldDataConnected(int id, bool exist);
	bool GetWorldDataConnected(int id);

	void SetWorldDataSocket(int id, SOCKET s);
	int GetWorldDataSocket(int id);

	WSABUF GetWolrdDataWsabuf(int id);
	OVERLAPPED GetWorldDataOriginalOverlapped(int id);
	unsigned char *GetIocpBuffer(int id);

	int GetPacketSize(int id);
	void SetPacketSize(int id, int a);
	void SetPacketSize(int id, unsigned char *bufPtr);

	void SetPreviousSize(int id, int a);
	void SetPreviousSizePlus(int id, int remained);

	int GetRequired(int id);

	void SetPacketBufPlusSize(int id, unsigned char *bufPtr, int required);

	unsigned char *GetPacketBuf(int id);

	void SocketClose(int id);

	// vlLock set
	void SetvlLock_Lock(int id);
	void SetvlLock_Unlock(int id);
	
	// viewList set
	int GetViewList_count(int id, int i);
	void SetViewList_insert(int id, int i);
	void SetViewList_erase(int id, int i);
	unordered_set<int> GetViewList(int id);


	// 위치, rot세팅용
	void SetWorldDataPos(int id, D3DXVECTOR3 pos);
	void SetWorldDataPosX(int id, float posx);
	void SetWorldDataPosY(int id, float posy);
	void SetWorldDataPosZ(int id, float posz);

	void SetWorldDataRot(int id, D3DXVECTOR3 rot);
	void SetWorldDataRotX(int id, float rotx);
	void SetWorldDataRotY(int id, float roty);
	void SetWorldDataRotZ(int id, float rotz);

	// 위치, rot확인용
	D3DXVECTOR3 GetWorldDataPos(int id);
	float GetWorldDataPosX(int id);
	float GetWorldDataPosY(int id);
	float GetWorldDataPosZ(int id);

	D3DXVECTOR3 GetWorldDataRot(int id);
	float GetWorldDataRotX(int id);
	float GetWorldDataRotY(int id);
	float GetWorldDataRotZ(int id);

	// 이동속도 세팅, 확인용
	void SetWorldDataMovement(int id, float movement);
	float GetWorldDataMovement(int id);

	// npc, object 확인용
	const bool isNPC(const int id);
	const bool isObject(const int id);
	const bool isActive(const int id);	// 타워나 돌등은 움직이지 않을테니까 확인용


private:
	WorldData worldData[NUM_OF_OBJECT];
	
	int retval;
};