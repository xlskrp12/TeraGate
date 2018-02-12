#include "ObjectShuriken.h"
ObjectShuriken::ObjectShuriken()
{
    _objectID     = ID_SHURIKEN;
    _size         = 30.f;
    _hp           = 3;
    _attack       = 25.f;
    _movement     = 55.f;
    _shurikenRotY = 0;
    _shurikenRotZ = 0;
    _deadTimer.setCooltime(1.2);
}
ObjectShuriken::~ObjectShuriken()
{
}
void ObjectShuriken::respawn()
{
    _objectID     = ID_SHURIKEN;
    _size         = 30.f;
    _hp           = 3;
    _attack       = 25.f;
    _movement     = 55.f;
    _shurikenRotY = 0;
    _shurikenRotZ = 0;
    _deadTimer.setCooltime(1.2);
    _deadTimer.reset();
}
D3DXMATRIX  ObjectShuriken::getMtxWorld()
{
    _shurikenRotY -= 25;
    _shurikenRotZ += 3;
    if(30<_shurikenRotZ)
        _shurikenRotZ = -30;


    // 스케일링
    D3DXMatrixScaling(&_mtxScaling, 1, 1, 1);

    // 회전
    D3DXMatrixRotationX(&_mtxRotation[0], DEGREE_TO_RADIAN(_rot.x - 90));     // 맥스 YZ변환 때문에 x축 -90도 회전
    D3DXMatrixRotationY(&_mtxRotation[1], DEGREE_TO_RADIAN((-1*(_rot.y+_shurikenRotY))+90));// 맥스 오른손 좌표계->다렉 왼손 좌표계를 만들기 위해 -1을 곱함. 그 전에 회전값 (0, 0, 0)인 오브젝트가 (1, 0, 0) +x축을 바라보게 만들기 y축 90도를 뺌.
    D3DXMatrixRotationZ(&_mtxRotation[2], DEGREE_TO_RADIAN(_rot.z+_shurikenRotZ));

    // 오브젝트 월드좌표만큼 이동
    D3DXMatrixTranslation(&_mtxPosition, _pos.x, _pos.y, _pos.z);


    return _mtxScaling *_mtxRotation[0] * _mtxRotation[1] * _mtxRotation[2] * _mtxPosition;
}
void ObjectShuriken::update()
{

    if(_deadTimer.isOn())
        _exist = false;
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