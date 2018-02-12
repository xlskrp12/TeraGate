#include "ObjectGateCore.h"
ObjectGateCore::ObjectGateCore()
{
    _objectID = ID_STATIC;
    float speed = (float)RANDOM_M(-20, 20);
    if(speed == 0)
        speed = 1;
    _speed = speed / 10.f;
    _randomUpdateTerm = RANDOM_M(100, 500);
}
ObjectGateCore::~ObjectGateCore()
{
}
void ObjectGateCore::update()
{
    _rot.y += 0.75;
    if (300 < _aiUpdateTerm)
    {
        if (_isGoingUp == true)
        {
            _isGoingUp = false;
        }
        else if (_isGoingUp == false)
        {
            _isGoingUp = true;
        }
        _aiUpdateTerm = 0;
    }
    else
    {
        _aiUpdateTerm++;
    }
    if (_isGoingUp == true)
        _pos.y += _speed;
    else if (_isGoingUp == false)
        _pos.y -= _speed;
}
void ObjectGateCore::updateTargetGateID(Object* object)
{
}