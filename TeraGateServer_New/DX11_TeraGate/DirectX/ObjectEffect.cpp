#include "ObjectEffect.h"
ObjectEffect::ObjectEffect()
{
    _objectID = ID_EFFECT;
    _size     = 30.f;
    _attack   = 30.f;
    _movement = 30.f;
}
ObjectEffect::~ObjectEffect()
{
}
void ObjectEffect::update()
{
    // 최종 목적지에 도착했으면 없앰.
    _distance = sqrt(pow(_des.x - _pos.x, 2) + pow(_des.z - _pos.z, 2));
    if(_distance <= _movement)
    {
        _exist = false;
    }
    else
    {
        // 이동
        _pos.x += _movement * cos(DEGREE_TO_RADIAN(_rot.y));
        _pos.z += _movement * sin(DEGREE_TO_RADIAN(_rot.y));
    }

    // 맵 사이즈를 벗어나게되면
    if (pow(_mapSize, 2) <= pow(_pos.x, 2) + pow(_pos.z, 2))
    {
        _exist = false;
    }
}