#pragma once
#include"Prev.h"
#include"Timer.h"
class Object
{
public://인터페이스
    enum{
        ID_HERO,
        ID_MINION,
        ID_STATIC,// 충돌체크를 하더라도 움직이는 친구가 아님.
        ID_TOTAL
    };
    enum{
        STATE_ALIVE,
        STATE_SLEEP,
        STATE_TOAL
    };
    enum{
    };
    Object();
    ~Object();
    bool isCollision(const Object& object);
    void setPos(float x, float y, float z);
    void setPosX(float x);
    void setPosY(float y);
    void setPosZ(float z);
    void setRot(float x, float y, float z);
    void setRotX(float x);
    void setRotY(float y);
    void setRotZ(float z);
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

    //가상 인터페이스
    virtual D3DXMATRIX  getMtxWorld();
    virtual void        update();
    virtual void        setArkDes(D3DXVECTOR3 ark, D3DXVECTOR3 des); // 초기위치, 목적지 좌표를 결정

//--------------------------------------------------------------
	bool _exist;
//--------------------------------------------------------------

protected:
    // 충돌체크를 위한 반지름 크기
    float       _size;
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
    int        _id;
};