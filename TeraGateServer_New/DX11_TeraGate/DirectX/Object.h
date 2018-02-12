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
public:// 상수
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
        ID_STATIC,// 충돌체크를 하더라도 움직이는 친구가 아님.,
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
        EXIST_TRUE  = true,// STATE_SLEEP의 경우 충돌체크를 하지 않음
        EXIST_TOTAL
    };
    enum STATE{
        STATE_IDLE,
        STATE_MOVE,
        STATE_ATTACK,
        STATE_FLASH
    };
    enum{
        TEAM_L,// 팀 레프트 (왼쪽팀)
        TEAM_R,// 팀 라이트 (오른쪽팀)
        TEAM_N,// 팀 엔피씨 (윗팀)
        TEAM_TOTAL
    };
    enum{
        // C 중앙
        // L  왼쪽팀 메인게이트
        // LC 왼쪽팀에서 중앙 길 게이트
        // LN 왼쪽팀에서 엔피씨 팀(위쪽 팀) 길 게이트
        // LR 왼쪽팀에서 오른쪽 팀 길 게이트
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
public://인터페이스
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
    //충돌체크
    float getSize();
    void  setSize(float size);
    //오브젝트 아이디
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
    //가상 인터페이스
    virtual D3DXMATRIX  getMtxWorld();
    virtual void        update();
    virtual void        updateControl();
    virtual void        updateFrame();
    virtual void        updateState();
    virtual void        setArkDes(D3DXVECTOR3 ark, D3DXVECTOR3 des); // 초기위치, 목적지 좌표를 결정
    virtual void        updateTargetGateID(Object* object);
    virtual void        attack(Object& object);
    virtual void        damage(Object& object);
    virtual void        respawn();
    virtual int         getState();
    virtual void        setState(int state);
    virtual void        updateMap(int gateIndex, int gateTeamID);
protected:
    // 충돌체크를 위한 반지름 크기
    float       _size;
    float       _sightSize;
    float       _mapSize;
    // 현재좌표, 목적좌표, 방향값(회전값)
    D3DXVECTOR3 _pos;
    D3DXVECTOR3 _des;
    D3DXVECTOR3 _rot;
    //getMtxWorld()에서 매번 로컬변수 얻고 버리고 얻고 버리고 하면 비효율적이라 걍 멤버로 냅둠.
    D3DXMATRIX _mtxRotation[3];
    D3DXMATRIX _mtxPosition;
    D3DXMATRIX _mtxScaling;
    D3DXMATRIX _mtxExport;
    int        _state;
    int        _frame;
    int        _framePerUpdate;
    int        _objectID;
    int        _teamID;
    int        _gateID;  // 게이트의 경우 자신의 아이디를 의미. 그 외 오브젝트의 경우 목적지 게이트 아이디를 의미.
    bool       _exist;   // 최적화를 위해 오브젝트 매니저에서 오브젝트 관리시 동적할당하지 않고 미리 할당해놓는다. _exist가 true면 존재 false면 더미로 취급. false면 해당 오브젝트를 패킷에 안넣어도 된다.
    bool       _control;
    float      _movement;// 이동속도         // 패킷에는 안넣어도 되는거
    float      _distance;// 목적지까지의 거리// 패킷에는 안넣어도 되는거
    float      _hp;
    float      _attack;

    // 맵 관련 변수
    const static int TOTAL_GATE = 13;
    Map  _map;
};

