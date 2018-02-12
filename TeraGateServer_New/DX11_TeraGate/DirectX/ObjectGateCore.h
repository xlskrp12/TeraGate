#pragma once
#include "Object.h"
class ObjectGateCore :
    public Object
{
public:
    ObjectGateCore();
    ~ObjectGateCore();
    void update();
    void updateTargetGateID(Object* object);
private:
    int  _randomUpdateTerm = 0;
    int  _aiUpdateTerm     = 0;
    bool _isGoingUp        = true;
    float _speed;
};

