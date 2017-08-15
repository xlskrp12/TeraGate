#pragma once
#include"Prev.h"
#include"Timer.h"
class Object
{
public://�������̽�
    enum{
        ID_HERO,
        ID_MINION,
        ID_STATIC,// �浹üũ�� �ϴ��� �����̴� ģ���� �ƴ�.
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
    //�浹üũ
    float getSize();
    void  setSize(float size);
    //������Ʈ ���̵�
    int   getID();
    void  setID(int id);

    //���� �������̽�
    virtual D3DXMATRIX  getMtxWorld();
    virtual void        update();
    virtual void        setArkDes(D3DXVECTOR3 ark, D3DXVECTOR3 des); // �ʱ���ġ, ������ ��ǥ�� ����

//--------------------------------------------------------------
	bool _exist;
//--------------------------------------------------------------

protected:
    // �浹üũ�� ���� ������ ũ��
    float       _size;
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
    int        _id;
};