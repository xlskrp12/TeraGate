#include "Camera.h"
Camera::Camera()
{
    _pos = { 0.f, 0.f, 0.f };
    _rot = { 0.f, 0.f, 0.f };
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
    lookat.z = 2.f;

    // XYZ �ະ ȸ�� ������ �������� ����
    pitch    = DEGREE_TO_RADIAN(_rot.x);
    yaw      = DEGREE_TO_RADIAN(_rot.y);
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
    ////��ī�޶�
    //// �߷�, ���� ����
    static float forceY       = 0.f;
    static float forceJump    = 1.0f;
    static float gravity      = 0.05f;
    static float lowestHeight = 125.f;
    static float baseHeight   = 800.f;
    if (_pos.y <= baseHeight)
        forceY = 0.f;
    forceY = forceY - gravity;
    if (GetKeyState(VK_SPACE) & 0x8000)
    {
        if (_pos.y == baseHeight)
            forceY = forceY + forceJump;
    }
    _pos.y = _pos.y + forceY;
    if (_pos.y < baseHeight)
        _pos.y = baseHeight;
    // �̵�����
    static float movement = 10.0f;
    static float cameraRotationUnit = 1.5f;
    // ��
    if ((GetKeyState(VK_UP) & 0x8000) && (GetKeyState(VK_RIGHT) & 0x8000))
    {
        _rot.y += cameraRotationUnit;
    }

    // ��
    else if ((GetKeyState(VK_UP) & 0x8000) && (GetKeyState(VK_LEFT) & 0x8000))
    {
        _rot.y -= cameraRotationUnit;
    }

    // ��
    else if ((GetKeyState(VK_DOWN) & 0x8000) && (GetKeyState(VK_RIGHT) & 0x8000))
    {
        _rot.y += cameraRotationUnit;
    }

    // ��
    else if ((GetKeyState(VK_DOWN) & 0x8000) && (GetKeyState(VK_LEFT) & 0x8000))
    {
        _rot.y -= cameraRotationUnit;
    }


    else if (GetKeyState(VK_UP) & 0x8000)
    {
        ;
    }

    else if (GetKeyState(VK_DOWN) & 0x8000)
    {
        ;
    }

    else if (GetKeyState(VK_LEFT) & 0x8000)
    {
        _rot.y -= cameraRotationUnit;
    }

    else if (GetKeyState(VK_RIGHT) & 0x8000)
    {
        _rot.y += cameraRotationUnit;
    }


    if (GetKeyState(VK_UP) & 0x8000)
    {
        _pos.x += movement * sin((_rot.y) * D3DX_PI / 180);
        _pos.z += movement * cos((_rot.y) * D3DX_PI / 180);
    }
    if (GetKeyState(VK_DOWN) & 0x8000)
    {
        _pos.x -= movement * sin((_rot.y) * D3DX_PI / 180);
        _pos.z -= movement * cos((_rot.y) * D3DX_PI / 180);
    }

    if(GetKeyState(VK_SUBTRACT)&0x8000)
    {
        baseHeight+=7.5;
        if(1500<baseHeight)
            baseHeight = 1500;
    }
    if(GetKeyState(VK_ADD)&0x8000)
    {
        baseHeight-=7.5;
        if(baseHeight < lowestHeight)
            baseHeight = lowestHeight;
        _pos.y = baseHeight;
    }
    if (GetKeyState(VK_BACK) & 0x8000)
    {
        _rot.x -= 0.5;
    }
    if(GetKeyState(VK_RETURN)&0x8000)
    {
        _rot.x += 0.5;
    }
    if (GetKeyState(VK_F1) & 0x8000)
    {
        _rot.x = 45;
        _rot.y = 0;

        _pos.x = 0;
        _pos.y = 1000;
        _pos.z = -1500;
    }
    else if (GetKeyState(VK_F2) & 0x8000)
    {
        _rot.x = 20;
        _rot.y = 0;

        _pos.x = 0;
        _pos.y = 700;
        _pos.z = -1200;
    }
    else if (GetKeyState(VK_F3) & 0x8000)
    {
        _rot.x = 20;
        _rot.y = 0;

        _pos.x = 0;
        _pos.y = 100;
        _pos.z = -1500;
    }
    //setPos(_pos);
    //setRot(_rot);
}