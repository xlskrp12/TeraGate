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
	void Init(int id);	// id�ϳ��� �α׾ƿ�������� ���

	void SetWSARecv(SOCKET newClient, int id);
	int GetWSARecvRetval();

	//��Ŷ ������
	void SendPacket(int id, unsigned char *packet);	
	void SendPutPlayerPacket(int id, int object);
	void SendRemovePlayerPacket(int id, int objcet);
	void SendAttackPacket(int id);

	// �α���, �α׾ƿ�
	void LoginPlayer(int id);
	void LogoutPlayer(int id);

	//worldData ���� Ȯ��
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


	// ��ġ, rot���ÿ�
	void SetWorldDataPos(int id, D3DXVECTOR3 pos);
	void SetWorldDataPosX(int id, float posx);
	void SetWorldDataPosY(int id, float posy);
	void SetWorldDataPosZ(int id, float posz);

	void SetWorldDataRot(int id, D3DXVECTOR3 rot);
	void SetWorldDataRotX(int id, float rotx);
	void SetWorldDataRotY(int id, float roty);
	void SetWorldDataRotZ(int id, float rotz);

	// ��ġ, rotȮ�ο�
	D3DXVECTOR3 GetWorldDataPos(int id);
	float GetWorldDataPosX(int id);
	float GetWorldDataPosY(int id);
	float GetWorldDataPosZ(int id);

	D3DXVECTOR3 GetWorldDataRot(int id);
	float GetWorldDataRotX(int id);
	float GetWorldDataRotY(int id);
	float GetWorldDataRotZ(int id);

	// �̵��ӵ� ����, Ȯ�ο�
	void SetWorldDataMovement(int id, float movement);
	float GetWorldDataMovement(int id);

	// npc, object Ȯ�ο�
	const bool isNPC(const int id);
	const bool isObject(const int id);
	const bool isActive(const int id);	// Ÿ���� ������ �������� �����״ϱ� Ȯ�ο�


private:
	WorldData worldData[NUM_OF_OBJECT];
	
	int retval;
};