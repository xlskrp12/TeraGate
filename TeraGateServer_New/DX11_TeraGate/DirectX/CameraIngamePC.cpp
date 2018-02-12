#include "CameraIngamePC.h"
CameraIngamePC::CameraIngamePC()
{
    _distanceSub = 2000;
    //▼모드2관련
    // 중력관련
    _forceY        = 0.f;
    _forceJump     = 1.0f;
    _gravity       = 0.05f;
    _lowestHeight  = 1.f;
    _highestHeight = 3000.f;
    _baseHeight    = 1200.f;

    // 이동관련
    _movement           = 10.0f;
    _cameraRotationY    = 0.f;
    _cameraRotationUnit = 2.0f;
}
CameraIngamePC::~CameraIngamePC()
{
}
void CameraIngamePC::updateMode1()
{
    setRot(D3DXVECTOR3(45, 90, 0));
    // 카메라 확대&축소&초기화
    if(GetKeyState(/*+*/VK_ADD) & 0x8000)
    {
        if(1000<=_distanceSub)
            _distanceSub -= 25;
        else
            _distanceSub = 1000;
    }
    if(GetKeyState(/*-*/VK_SUBTRACT) & 0x8000)
    {
        if(_distanceSub <= 4000)
            _distanceSub += 25;
        else
            _distanceSub = 4000;
    }
    if(GetKeyState(/*-*/VK_RETURN) & 0x8000)
    {
            _distanceSub = 2000;
    }


    // 카메라가 타겟(목적지)를 향해 스무스하게 이동
    _distance = _des.x - _pos.x;
    if(fabs(_distance) < 0.1)
        _distance = 0;
    _pos.x += _distance/20;

    _distance = _des.y - _pos.y + _distanceSub;
    if(fabs(_distance) < 0.1)
        _distance = 0;
    _pos.y += _distance/10;

    _distance = _des.z - _pos.z - (_distanceSub*5/4);
    if(fabs(_distance) < 0.1)
        _distance = 0;
    _pos.z += _distance/20;

}
void CameraIngamePC::updateMode2()
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
    if ((GetKeyState(VK_NUMPAD8) & 0x8000) && (GetKeyState(VK_NUMPAD6) & 0x8000))
    {
        _cameraRotationY = 45;
    }

    // ↖
    else if ((GetKeyState(VK_NUMPAD8) & 0x8000) && (GetKeyState(VK_NUMPAD4) & 0x8000))
    {
        _cameraRotationY = -45;
    }

    // ↘
    else if ((GetKeyState(VK_NUMPAD2) & 0x8000) && (GetKeyState(VK_NUMPAD6) & 0x8000))
    {
        _cameraRotationY = 135;
    }

    // ↙
    else if ((GetKeyState(VK_NUMPAD2) & 0x8000) && (GetKeyState(VK_NUMPAD4) & 0x8000))
    {
        _cameraRotationY = -135;
    }

    else if (GetKeyState(VK_NUMPAD8) & 0x8000)
    {
        _cameraRotationY = 0;
    }

    else if (GetKeyState(VK_NUMPAD2) & 0x8000)
    {
        _cameraRotationY = 180;
    }

    else if (GetKeyState(VK_NUMPAD4) & 0x8000)
    {
        _cameraRotationY = -90;
    }

    else if (GetKeyState(VK_NUMPAD6) & 0x8000)
    {
        _cameraRotationY = 90;
    }
    // 카메라 이동방향에 따른 이동
    if ((GetKeyState(VK_NUMPAD6) & 0x8000)||(GetKeyState(VK_NUMPAD4) & 0x8000)||(GetKeyState(VK_NUMPAD8) & 0x8000)||(GetKeyState(VK_NUMPAD2) & 0x8000))
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