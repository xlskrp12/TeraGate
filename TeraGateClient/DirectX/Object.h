#pragma once
#include"Prev.h"
class Object
{
public://인터페이스
    Object();
    ~Object();
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
    D3DXVECTOR3 getPos();
    float getPosX();
    float getPosY();
    float getPosZ();
    D3DXVECTOR3 getRot();
    float getRotX();
    float getRotY();
    float getRotZ();
    D3DXMATRIX  getMtxWorld();
    //가상 인터페이스
    virtual void update();
    virtual void setArkDes(D3DXVECTOR3 ark, D3DXVECTOR3 des) {
        _ark = ark;
        _pos = ark;
        _des = des;
    }// 나중에 지울 것. 초기위치, 목적지 좌표를 결정
    virtual void setUpdateCount(int count) { _updateCount = count; }
protected:
    int         _updateCount = 0;
    D3DXVECTOR3 _ark;
    D3DXVECTOR3 _pos;
    D3DXVECTOR3 _des;
    D3DXVECTOR3 _rot;
};

