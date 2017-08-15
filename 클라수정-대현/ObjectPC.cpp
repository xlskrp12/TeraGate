#include "ObjectPC.h"
ObjectPC::ObjectPC()
{
    _id    = ID_HERO;
    _size  = 30.0;
    _timer[SKILL_Q].setCooltime(1);
    _timer[SKILL_W].setCooltime(1);
    _timer[SKILL_E].setCooltime(1);
    _timer[SKILL_R].setCooltime(1);
}
ObjectPC::~ObjectPC()
{
}
void ObjectPC::update()
{
    // 이동 후 방향관련
    D3DXVECTOR3 posBefore = _pos;
    //▼중력, 점프 관련
    static float forceY       = 0.f;
    static float forceJump    = 25.0f;
    static float gravity      = 0.20f;
    static float lowestHeight = 0.f;
    static float baseHeight   = 0.f;
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
    // 이동관련
    static float movement     = 5.0f;
    static float rotationUnit = 0.2f;
    if (GetKeyState(VK_SHIFT) & 0x8000)
        movement = 12.f;
    else
        movement = 8.f;



    //▼스킬
    if(GetKeyState(/*Q*/0x51) & 0x8000)
    {
        if(_timer[SKILL_Q].isOn())
        {
        }
    }
    if(GetKeyState(/*W*/0x57) & 0x8000)
    {
        if(_timer[SKILL_W].isOn())
        {

        }
    }
    if(GetKeyState(/*E*/0x45) & 0x8000)
    {
        if(_timer[SKILL_E].isOn())
        {
            _pos.x += 30 * movement * cos(DEGREE_TO_RADIAN(_rot.y));
            _pos.z += 30 * movement * sin(DEGREE_TO_RADIAN(_rot.y));
        }
    }
    if(GetKeyState(/*R*/0x52) & 0x8000)
    {
        if(_timer[SKILL_R].isOn())
        {
        }
    }
    //▼복합키, 단독키 입력시 방향처리
    // ↗
    /*if ((GetKeyState(VK_UP) & 0x8000) && (GetKeyState(VK_RIGHT) & 0x8000))
        _rot.y = 45;
    // ↖
    else if ((GetKeyState(VK_UP) & 0x8000) && (GetKeyState(VK_LEFT) & 0x8000))
        _rot.y = 135;
    // ↘
    else if ((GetKeyState(VK_DOWN) & 0x8000) && (GetKeyState(VK_RIGHT) & 0x8000))
        _rot.y = -45;
    // ↙
    else if ((GetKeyState(VK_DOWN) & 0x8000) && (GetKeyState(VK_LEFT) & 0x8000))
        _rot.y = -135;
    else if (GetKeyState(VK_UP) & 0x8000)
        _rot.y = 90;
    else if (GetKeyState(VK_DOWN) & 0x8000)
        _rot.y = -90;
    else if (GetKeyState(VK_LEFT) & 0x8000)
        _rot.y = 180;
    else if (GetKeyState(VK_RIGHT) & 0x8000)
        _rot.y = 0;
	*/

    // 어쨌든 이동키를 하나라도 누른다면 이동
   /* if ((GetKeyState(VK_UP) & 0x8000) ||  (GetKeyState(VK_DOWN) & 0x8000))
    {
        //_pos.x += movement * cos(DEGREE_TO_RADIAN(_rot.y));
        //_pos.z += movement * sin(DEGREE_TO_RADIAN(_rot.y));
		_pos.x = _ServerDef->player.x;
		_pos.z = _ServerDef->player.z;

		_rot.y = _ServerDef->player.roty + 90;
    }
	else if ((GetKeyState(VK_RIGHT) & 0x8000) || (GetKeyState(VK_LEFT) & 0x8000))
	{
		_pos.x = _ServerDef->player.x;
		_pos.z = _ServerDef->player.z;

		_rot.y = _ServerDef->player.roty - 90;
	}*/
    // 맵 사이즈 밖으로 나가게 된다면?
    if (pow(_mapSize, 2) <= pow(_pos.x, 2) + pow(_pos.z, 2))
    {
        //_pos.z = _mapSize * sin(atan2(_pos.z, _pos.x));
        //_pos.x = _mapSize * cos(atan2(_pos.z, _pos.x));
//        _rot.y = RADIAN_TO_DEGREE(atan2(_pos.z, _pos.x));
    }
}