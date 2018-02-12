#pragma once
#include"Prev.h"
#include"FBXManager.h"
#include"Timer.h"
#include"SoundManager.h"
struct Gate{
    int _teamID;
};
struct Map{
    const static int TOTAL_GATE = 13;
    Gate _gate[TOTAL_GATE];

};
class Object
{
public:// ���
    const static int FRAME_PER_1UPDATE = 1;
    const static int FRAME_PER_2UPDATE = 2;
    const static int FRAME_PER_3UPDATE = 3;
    const static int FRAME_PER_4UPDATE = 4;    
    enum{
        ID_EFFECT,
        ID_SHURIKEN,
        ID_SLASH,
        ID_MAGICIAN,
        ID_KNIGHT,
        ID_MINION,
        ID_STATIC,// �浹üũ�� �ϴ��� �����̴� ģ���� �ƴ�.,
        ID_GATE,
        ID_TOTAL
    };
    enum{
        CONTROL_FALSE = false,
        CONTROL_TRUE  = true,
        CONTROL_TOTAL
    };
    enum{
        EXIST_FALSE = false,
        EXIST_TRUE  = true,// STATE_SLEEP�� ��� �浹üũ�� ���� ����
        EXIST_TOTAL
    };
    enum STATE{
        STATE_IDLE,
        STATE_MOVE,
        STATE_ATTACK,
        STATE_FLASH
    };
    enum{
        TEAM_L,// �� ����Ʈ (������)
        TEAM_R,// �� ����Ʈ (��������)
        TEAM_N,// �� ���Ǿ� (����)
        TEAM_TOTAL
    };
    enum{
        // C �߾�
        // L  ������ ���ΰ���Ʈ
        // LC ���������� �߾� �� ����Ʈ
        // LN ���������� ���Ǿ� ��(���� ��) �� ����Ʈ
        // LR ���������� ������ �� �� ����Ʈ
        GATE_C,
        GATE_L,
        GATE_LN,
        GATE_LR,
        GATE_LC,
        GATE_R,
        GATE_RN,
        GATE_RL,
        GATE_RC,
        GATE_N,
        GATE_NL,
        GATE_NR,
        GATE_NC,
        GATE_TOTAL
    };
    const static D3DXVECTOR3 GATE_C_POS;
    const static D3DXVECTOR3 GATE_L_POS;
    const static D3DXVECTOR3 GATE_LN_POS;
    const static D3DXVECTOR3 GATE_LR_POS;
    const static D3DXVECTOR3 GATE_LC_POS;
    const static D3DXVECTOR3 GATE_R_POS;
    const static D3DXVECTOR3 GATE_RL_POS;
    const static D3DXVECTOR3 GATE_RN_POS;
    const static D3DXVECTOR3 GATE_RC_POS;
    const static D3DXVECTOR3 GATE_N_POS;
    const static D3DXVECTOR3 GATE_NL_POS;
    const static D3DXVECTOR3 GATE_NR_POS;
    const static D3DXVECTOR3 GATE_NC_POS;
    const static float       GATE_RADIUS;
public://�������̽�
    Object();
    ~Object();
    bool isCollision(const Object& object);
    bool isInSight(const Object& object);
    void setPosAway(float distance);
    void setDesByDistance(float distance);
    D3DXVECTOR3 getDes();
    void setDes(D3DXVECTOR3 des);
    void setPos(D3DXVECTOR3 pos);
    void setPos(float x, float y, float z);
    void setPosX(float x);
    void setPosY(float y);
    void setPosZ(float z);
    void setRot(D3DXVECTOR3 rot);
    void setRot(float x, float y, float z);
    void setRotX(float x);
    void setRotY(float y);
    void setRotZ(float z);
    void setMovement(float movement);
    void moveToDestination();
    void move(float x, float y, float z);
    void moveX(float x);
    void moveY(float y);
    void moveZ(float z);
    void rot(float x, float y, float z);
    void rotX(float x);
    void rotY(float y);
    void rotZ(float z);
    D3DXVECTOR3 getPos();
    float getPosX();
    float getPosY();
    float getPosZ();
    D3DXVECTOR3 getRot();
    float getRotX();
    float getRotY();
    float getRotZ();
    //�浹üũ
    float getSize();
    void  setSize(float size);
    //������Ʈ ���̵�
    int   getID();
    void  setID(int id);
    int   getTeamID();
    void  setTeamID(int teamID);
    int   getGateID();
    void  setGateID(int lineID);
    bool  getExist();
    void  setExist(bool exist);
    bool  getControl();
    void  setControl(bool control);
    int   getFrame();
    void  setFrame(int frame);
    void  damage(float dmg);
    float getHP();
    void  setHP(float hp);
    //���� �������̽�
    virtual D3DXMATRIX  getMtxWorld();
    virtual void        update();
    virtual void        updateControl();
    virtual void        updateFrame();
    virtual void        updateState();
    virtual void        setArkDes(D3DXVECTOR3 ark, D3DXVECTOR3 des); // �ʱ���ġ, ������ ��ǥ�� ����
    virtual void        updateTargetGateID(Object* object);
    virtual void        attack(Object& object);
    virtual void        damage(Object& object);
    virtual void        respawn();
    virtual int         getState();
    virtual void        setState(int state);
    virtual void        updateMap(int gateIndex, int gateTeamID);
protected:
    // �浹üũ�� ���� ������ ũ��
    float       _size;
    float       _sightSize;
    float       _mapSize;
    // ������ǥ, ������ǥ, ���Ⱚ(ȸ����)
    D3DXVECTOR3 _pos;
    D3DXVECTOR3 _des;
    D3DXVECTOR3 _rot;
    //getMtxWorld()���� �Ź� ���ú��� ��� ������ ��� ������ �ϸ� ��ȿ�����̶� �� ����� ����.
    D3DXMATRIX _mtxRotation[3];
    D3DXMATRIX _mtxPosition;
    D3DXMATRIX _mtxScaling;
    D3DXMATRIX _mtxExport;
    int        _state;
    int        _frame;
    int        _framePerUpdate;
    int        _objectID;
    int        _teamID;
    int        _gateID;  // ����Ʈ�� ��� �ڽ��� ���̵� �ǹ�. �� �� ������Ʈ�� ��� ������ ����Ʈ ���̵� �ǹ�.
    bool       _exist;   // ����ȭ�� ���� ������Ʈ �Ŵ������� ������Ʈ ������ �����Ҵ����� �ʰ� �̸� �Ҵ��س��´�. _exist�� true�� ���� false�� ���̷� ���. false�� �ش� ������Ʈ�� ��Ŷ�� �ȳ־ �ȴ�.
    bool       _control;
    float      _movement;// �̵��ӵ�         // ��Ŷ���� �ȳ־ �Ǵ°�
    float      _distance;// ������������ �Ÿ�// ��Ŷ���� �ȳ־ �Ǵ°�
    float      _hp;
    float      _attack;

    // �� ���� ����
    const static int TOTAL_GATE = 13;
    Map  _map;
};

