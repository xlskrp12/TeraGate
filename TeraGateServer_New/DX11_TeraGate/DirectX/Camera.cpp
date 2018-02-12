#include "Camera.h"
Camera::Camera()
{
    _mode = 1;
    _pos  = { 0.f, 0.f, 0.f };
    _rot  = { 0.f, 0.f, 0.f };
#ifdef _DEBUG
    printf("Camera::Camera();\n");
#endif
}
Camera::Camera(const Camera& camera)
{

}
Camera::~Camera()
{

}
void Camera::rot(float x, float y, float z)
{
    _rot.x+=x;
    _rot.y+=y;
    _rot.z+=z;
}
void Camera::setPos(D3DXVECTOR3 pos)
{
    _pos = pos;
}
void Camera::setPos(float x, float y, float z)
{
    _pos.x = x;
    _pos.y = y;
    _pos.z = z;
}
void Camera::setPosX(float x)
{
    _pos.x = x;
}
void Camera::setPosY(float y)
{
    _pos.y = y;
}
void Camera::setPosZ(float z)
{
    _pos.z = z;
}
void Camera::setRot(D3DXVECTOR3 rot)
{
    _rot = rot;
}
void Camera::setRot(float x, float y,  float z)
{
    _rot.x = x;
    _rot.y = y;
    _rot.z = z;
}
void Camera::setRotX(float x)
{
    _rot.x = x;
}
void Camera::setRotY(float y)
{
    _rot.y = y;
}
void Camera::setRotZ(float z)
{
    _rot.z = z;
}
void Camera::setDes(D3DXVECTOR3 des)
{
    _des = des;
}
void Camera::setDesX(float x)
{
    _des.x = x;
}
void Camera::setDesY(float y)
{
    _des.y = y;
}
void Camera::setDesZ(float z)
{
    _des.z = z;
}
D3DXVECTOR3 Camera::getPosition()
{
    return _pos;
}
D3DXVECTOR3 Camera::getRotation()
{
    return _rot;
}
void Camera::render()
{
    D3DXVECTOR3 up, lookat;
    float       yaw, pitch, roll;
    D3DXMATRIX  mtxRotation;

    // ī�޶� ������, ��ġ����, �ü����� ����
    up.x     = 0.f;
    up.y     = 1.f;
    up.z     = 0.f;
    lookat.x = 0.f;
    lookat.y = 0.f;
    lookat.z = 1.f;

    // XYZ �ະ ȸ�� ������ �������� ����
    pitch    = DEGREE_TO_RADIAN(_rot.x);
    yaw      = DEGREE_TO_RADIAN(_rot.y-90); // ī�޶� ȸ������ (0, 0, 0)�� �� (1, 0, 0) +x���� �ٶ󺸰� �ϱ� ����
    roll     = DEGREE_TO_RADIAN(_rot.z);

    // pitch, yaw, roll�� ȸ����� ����
    D3DXMatrixRotationYawPitchRoll(&mtxRotation, yaw, pitch, roll);

    // �䰡 ��Ȯ�� ȸ���Ǳ� ����
    D3DXVec3TransformCoord(&lookat, &lookat, &mtxRotation);// ȸ����ķ� �ü����� ����
    D3DXVec3TransformCoord(&up, &up, &mtxRotation);        // ȸ����ķ� ������ ����

    lookat = _pos + lookat;
    // ������� ī�޶� ��ġ�ϰ��ϱ� ����
    // ������, ��ġ����, �ü����ͷ� �� ��� ����
    D3DXMatrixLookAtLH(&_mtxView, &_pos, &lookat, &up);
}
D3DXMATRIX Camera::getMtxView()
{
    render();
    return _mtxView;
}
void Camera::update()
{
    updateByMode();
}
void Camera::updateByMode()
{
    if (GetKeyState(VK_F11) & 0x8000)
    {
        _mode=1;
    }
    else if (GetKeyState(VK_F12) & 0x8000)
    {
        _mode=2;
    }
    else if (GetKeyState(VK_F12) & 0x8000)
    {
        _mode=2;
    }
    switch(_mode)
    {
    case 1:
        updateMode1();
        break;
    case 2:
        updateMode2();
        break;
    case 3:
        break;
    default:
        break;
    }
}
void Camera::updateMode1()
{
}
void Camera::updateMode2()
{
}