#pragma once
#include"Object.h"
/*
프레임
00~09 : 걷기
10~40 : 공격
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
    float _degree;    //현재 라인따라 이동 중인 각도
    float _lineRadius;//라인따라 이동할 때의 반지름
    float _attackRange;
};
