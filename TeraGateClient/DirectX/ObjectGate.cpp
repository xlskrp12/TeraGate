#include "ObjectGate.h"
ObjectGate::ObjectGate()
{
}
ObjectGate::~ObjectGate()
{
}
void ObjectGate::update()
{
    _rot.y += 0.5;
    
    static int aiUpdateTerm = 0;
    static bool up = true;
    if (200 < aiUpdateTerm)
    {
        if (up == true)
            up = false;
        else if (up == false)
            up = true;
        aiUpdateTerm = 0;
    }
    else
    {
        aiUpdateTerm++;
    }
    if (up == true)
        _pos.y += 0.2;
    else if (up == false)
        _pos.y -= 0.2;
}