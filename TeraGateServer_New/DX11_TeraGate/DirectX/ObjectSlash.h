#pragma once
#include "Object.h"
class ObjectSlash :
    public Object
{
public:
    ObjectSlash();
    ~ObjectSlash();
    D3DXMATRIX  getMtxWorld();
    void respawn();
    void update();
protected:
    float _slashRotY;
    float _slashRotZ;
    Timer _deadTimer;
};

