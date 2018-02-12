#pragma once
#include "Object.h"
class ObjectShuriken :
    public Object
{
public:
    ObjectShuriken();
    ~ObjectShuriken();
    D3DXMATRIX  getMtxWorld();
    void respawn();
    void update();
protected:
    float _shurikenRotY;
    float _shurikenRotZ;
    Timer _deadTimer;
};

