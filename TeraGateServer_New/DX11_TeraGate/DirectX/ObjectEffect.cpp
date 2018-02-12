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
    // ���� �������� ���������� ����.
    _distance = sqrt(pow(_des.x - _pos.x, 2) + pow(_des.z - _pos.z, 2));
    if(_distance <= _movement)
    {
        _exist = false;
    }
    else
    {
        // �̵�
        _pos.x += _movement * cos(DEGREE_TO_RADIAN(_rot.y));
        _pos.z += _movement * sin(DEGREE_TO_RADIAN(_rot.y));
    }

    // �� ����� ����ԵǸ�
    if (pow(_mapSize, 2) <= pow(_pos.x, 2) + pow(_pos.z, 2))
    {
        _exist = false;
    }
}