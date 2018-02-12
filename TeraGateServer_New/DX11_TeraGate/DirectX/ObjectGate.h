#pragma once
#include"Object.h"
class ObjectGate : public Object
{
public:
    ObjectGate();
    ~ObjectGate();
    void respawn();
    void update();
    void updateTargetGateID(Object* object);
private:
    int  _aiUpdateTerm = 0;
    bool _isGoingUp  = true;
};

