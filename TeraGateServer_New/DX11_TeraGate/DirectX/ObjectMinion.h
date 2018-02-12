#pragma once
#include"Object.h"
/*
������
00~09 : �ȱ�
10~40 : ����
*/
class ObjectMinion : public Object
{
public:
    ObjectMinion();
    ~ObjectMinion();
    void update();
    void updateFrame();
    void updateState();
    void respawn();
    void setState(int state);
    void attack(Object& object);
protected:
    Timer _aiTimer;
    float _degree;    //���� ���ε��� �̵� ���� ����
    float _lineRadius;//���ε��� �̵��� ���� ������
    float _attackRange;
};
