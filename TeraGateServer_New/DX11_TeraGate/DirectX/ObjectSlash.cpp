#include "ObjectSlash.h"
ObjectSlash::ObjectSlash()
{
    _objectID     = ID_SLASH;
    _size         = 30.f;
    _hp           = 180;
    _attack       = 0.01f;
    _movement     = 10.f;
    _slashRotY    = 0;
    _slashRotZ    = 0;
    _deadTimer.setCooltime(0.25);
}
ObjectSlash::~ObjectSlash()
{
}
void ObjectSlash::respawn()
{
    _objectID     = ID_SLASH;
    _size         = 30.f;
    _hp           = 180;
    _attack       = 0.01f;
    _movement     = 10.f;
    _slashRotY = 0;
    _slashRotZ = 0;
    _deadTimer.setCooltime(0.25);
    _deadTimer.reset();
}
D3DXMATRIX  ObjectSlash::getMtxWorld()
{
    // �����ϸ�
    D3DXMatrixScaling(&_mtxScaling, 1, 1, 1);

    // ȸ��
    D3DXMatrixRotationX(&_mtxRotation[0], DEGREE_TO_RADIAN(_rot.x - 90));                // �ƽ� YZ��ȯ ������ x�� -90�� ȸ��
    D3DXMatrixRotationY(&_mtxRotation[1], DEGREE_TO_RADIAN((-1*(_rot.y+_slashRotY))+90));// �ƽ� ������ ��ǥ��->�ٷ� �޼� ��ǥ�踦 ����� ���� -1�� ����. �� ���� ȸ���� (0, 0, 0)�� ������Ʈ�� (1, 0, 0) +x���� �ٶ󺸰� ����� y�� 90���� ��.
    D3DXMatrixRotationZ(&_mtxRotation[2], DEGREE_TO_RADIAN(_rot.z+_slashRotZ));

    // ������Ʈ ������ǥ��ŭ �̵�
    D3DXMatrixTranslation(&_mtxPosition, _pos.x, _pos.y, _pos.z);


    return _mtxScaling *_mtxRotation[0] * _mtxRotation[1] * _mtxRotation[2] * _mtxPosition;
}
void ObjectSlash::update()
{

    if(_deadTimer.isOn())
        _exist = false;
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