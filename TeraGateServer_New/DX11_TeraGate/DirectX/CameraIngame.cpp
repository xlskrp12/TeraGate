#include "CameraIngame.h"



CameraIngame::CameraIngame()
{
    // 중력관련
    _forceY        = 0.f;
    _forceJump     = 1.0f;
    _gravity       = 0.05f;
    _lowestHeight  = 1.f;
    _highestHeight = 3000.f;
    _baseHeight    = 1500.f;

    // 이동관련
    _movement           = 10.0f;
    _cameraRotationY    = 0.f;
    _cameraRotationUnit = 2.0f;
}


CameraIngame::~CameraIngame()
{
}
void CameraIngame::update()
{
    updateCameraMode();
}
void CameraIngame::updateCameraMode()
{
    if (GetKeyState(VK_F1) & 0x8000)
    {
        _mode=1;
    }
    else if (GetKeyState(VK_F2) & 0x8000)
    {
        _mode=2;
    }
    else if (GetKeyState(VK_F3) & 0x8000)
    {
        _mode=3;
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
void CameraIngame::updateMode1()
{
    if (_pos.y <= _baseHeight)
        _forceY = 0.f;
    _forceY = _forceY - _gravity;
    if (GetKeyState(VK_SPACE) & 0x8000)
    {
        if (_pos.y == _baseHeight)
            _forceY = _forceY + _forceJump;
    }
    _pos.y = _pos.y + _forceY;
    if (_pos.y < _baseHeight)
        _pos.y = _baseHeight;



    // ↗
    if ((GetKeyState(VK_UP) & 0x8000) && (GetKeyState(VK_RIGHT) & 0x8000))
    {
        _cameraRotationY = 45;
    }

    // ↖
    else if ((GetKeyState(VK_UP) & 0x8000) && (GetKeyState(VK_LEFT) & 0x8000))
    {
        _cameraRotationY = -45;
    }

    // ↘
    else if ((GetKeyState(VK_DOWN) & 0x8000) && (GetKeyState(VK_RIGHT) & 0x8000))
    {
        _cameraRotationY = 135;
    }

    // ↙
    else if ((GetKeyState(VK_DOWN) & 0x8000) && (GetKeyState(VK_LEFT) & 0x8000))
    {
        _cameraRotationY = -135;
    }

    else if (GetKeyState(VK_UP) & 0x8000)
    {
        _cameraRotationY = 0;
    }

    else if (GetKeyState(VK_DOWN) & 0x8000)
    {
        _cameraRotationY = 180;
    }

    else if (GetKeyState(VK_LEFT) & 0x8000)
    {
        _cameraRotationY = -90;
    }

    else if (GetKeyState(VK_RIGHT) & 0x8000)
    {
        _cameraRotationY = 90;
    }
    // 카메라 이동방향에 따른 이동
    if ((GetKeyState(VK_RIGHT) & 0x8000)||(GetKeyState(VK_LEFT) & 0x8000)||(GetKeyState(VK_UP) & 0x8000)||(GetKeyState(VK_DOWN) & 0x8000))
    {
        _pos.x += _movement * sin((_cameraRotationY) * D3DX_PI / 180);
        _pos.z += _movement * cos((_cameraRotationY) * D3DX_PI / 180);
    }

    if(GetKeyState(VK_SUBTRACT)&0x8000)
    {
        _baseHeight+=7.5;
        if(_highestHeight<_baseHeight)
            _baseHeight = _highestHeight;
    }
    if(GetKeyState(VK_ADD)&0x8000)
    {
        _baseHeight-=7.5;
        if(_baseHeight < _lowestHeight)
            _baseHeight = _lowestHeight;
        _pos.y = _baseHeight;
    }
    if (GetKeyState(VK_BACK) & 0x8000)
    {
        _rot.x -= 0.5;
    }
    if(GetKeyState(VK_RETURN)&0x8000)
    {
        _rot.x += 0.5;
    }
}
void CameraIngame::updateMode2()
{
    if (_pos.y <= _baseHeight)
        _forceY = 0.f;
    _forceY = _forceY - _gravity;
    if (GetKeyState(VK_SPACE) & 0x8000)
    {
        if (_pos.y == _baseHeight)
            _forceY = _forceY + _forceJump;
    }
    _pos.y = _pos.y + _forceY;
    if (_pos.y < _baseHeight)
        _pos.y = _baseHeight;
    // ↗
    if ((GetKeyState(VK_UP) & 0x8000) && (GetKeyState(VK_RIGHT) & 0x8000))
    {
        _rot.y += _cameraRotationUnit;
    }

    // ↖
    else if ((GetKeyState(VK_UP) & 0x8000) && (GetKeyState(VK_LEFT) & 0x8000))
    {
        _rot.y -= _cameraRotationUnit;
    }

    // ↘
    else if ((GetKeyState(VK_DOWN) & 0x8000) && (GetKeyState(VK_RIGHT) & 0x8000))
    {
        _rot.y += _cameraRotationUnit;
    }

    // ↙
    else if ((GetKeyState(VK_DOWN) & 0x8000) && (GetKeyState(VK_LEFT) & 0x8000))
    {
        _rot.y -= _cameraRotationUnit;
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
        _rot.y -= _cameraRotationUnit;
    }

    else if (GetKeyState(VK_RIGHT) & 0x8000)
    {
        _rot.y += _cameraRotationUnit;
    }


    if (GetKeyState(VK_UP) & 0x8000)
    {
        _pos.x += _movement * sin((_rot.y) * D3DX_PI / 180);
        _pos.z += _movement * cos((_rot.y) * D3DX_PI / 180);
    }
    if (GetKeyState(VK_DOWN) & 0x8000)
    {
        _pos.x -= _movement * sin((_rot.y) * D3DX_PI / 180);
        _pos.z -= _movement * cos((_rot.y) * D3DX_PI / 180);
    }

    if(GetKeyState(VK_SUBTRACT)&0x8000)
    {
        _baseHeight+=7.5;
        if(_highestHeight<_baseHeight)
            _baseHeight = _highestHeight;
    }
    if(GetKeyState(VK_ADD)&0x8000)
    {
        _baseHeight-=7.5;
        if(_baseHeight < _lowestHeight)
            _baseHeight = _lowestHeight;
        _pos.y = _baseHeight;
    }
    if (GetKeyState(VK_BACK) & 0x8000)
    {
        _rot.x -= 0.5;
    }
    if(GetKeyState(VK_RETURN)&0x8000)
    {
        _rot.x += 0.5;
    }
}