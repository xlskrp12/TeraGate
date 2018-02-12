#pragma once
#include"Object.h"
/*
프레임
00~09 : 걷기
10~40 : 공격
*/
class ObjectMagician : public Object
{
public:
    ObjectMagician();
    ~ObjectMagician();
    void respawn();
    void update();
    void updateControl();
    void updateFrame();
    void updateState();
protected:
    enum { SKILL_Q, SKILL_W, SKILL_E, SKILL_R, SKILL_TOTAL };
    Timer _timer[SKILL_TOTAL];
};

