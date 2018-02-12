#include "ObjectMagician.h"
ObjectMagician::ObjectMagician()
{
    _objectID = ID_MAGICIAN;
    _hp       = 200;
    _size     = 30.0;
    _timer[SKILL_Q].setCooltime(0);
    _timer[SKILL_W].setCooltime(1);
    _timer[SKILL_E].setCooltime(0);
    _timer[SKILL_R].setCooltime(1);
}
ObjectMagician::~ObjectMagician()
{
}
void ObjectMagician::respawn()
{
    _exist    = true;
    _objectID = ID_MAGICIAN;
    _hp       = 200;
    _size     = 30.0;
    _timer[SKILL_Q].setCooltime(0);
    _timer[SKILL_W].setCooltime(1);
    _timer[SKILL_E].setCooltime(0);
    _timer[SKILL_R].setCooltime(1);
}
void ObjectMagician::update()
{
    if(_hp<=0)
        _exist = false;
    if(_control)
        updateControl();
}
void ObjectMagician::updateControl()
{
    // 이동 후 방향관련
    D3DXVECTOR3 posBefore = _pos;
    //▼중력, 점프 관련
    static float forceY       = 0.f;
    static float forceJump    = 37.5f;
    static float gravity      = 2.5f;
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
    static float rotationUnit = 0.2f;
    if (GetKeyState(VK_SHIFT) & 0x8000)
        _movement = 21.f;
    else
        _movement = 14.f;
    if (0<_pos.y)
        _movement *= 1.4;
    //if(FBXManager::MAGICIAN_FLASH_A<=_frame && _frame <=FBXManager::MAGICIAN_FLASH_Z)
    //    _movement = 90.f;
    //▼스킬
    if(GetKeyState(/*Q*/0x51) & 0x8000)
    {
        if(_timer[SKILL_Q].isOn())
        {
            if(STATE_IDLE == _state || STATE_MOVE == _state || STATE_FLASH == _state)
            {
                _state = STATE_ATTACK;
                _frame = FBXManager::MAGICIAN_ATTACK_A;
            }
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
            if(STATE_IDLE == _state  || STATE_MOVE == _state || (STATE_ATTACK == _state && FBXManager::MAGICIAN_ATTACK_A+15 < _frame && _frame < FBXManager::MAGICIAN_ATTACK_Z))
            {
                _state = STATE_FLASH;
                _frame = FBXManager::MAGICIAN_FLASH_A;
            }
        }
    }
    if(GetKeyState(/*R*/0x52) & 0x8000)
    {
        if(_timer[SKILL_R].isOn())
        {
        }
    }
    //▼복합키, 단독키 입력시 방향처리
    if(STATE_IDLE == _state|| STATE_MOVE == _state || STATE_ATTACK == _state || STATE_FLASH == _state)
    {
        // ↗
        /*if ((GetKeyState(VK_UP) & 0x8000) && (GetKeyState(VK_RIGHT) & 0x8000))
        {
            _rot.y = 45;
        }
        // ↖
        else if ((GetKeyState(VK_UP) & 0x8000) && (GetKeyState(VK_LEFT) & 0x8000))
        {
            _rot.y = 135;
        }
        // ↘
        else if ((GetKeyState(VK_DOWN) & 0x8000) && (GetKeyState(VK_RIGHT) & 0x8000))
        {
            _rot.y = 315;
        }
        // ↙
        else if ((GetKeyState(VK_DOWN) & 0x8000) && (GetKeyState(VK_LEFT) & 0x8000))
        {
            _rot.y = 225;
        }
        else if (GetKeyState(VK_UP) & 0x8000)
        {
            _rot.y = 90;
        }
        else if (GetKeyState(VK_DOWN) & 0x8000)
        {
            _rot.y = 270;
        }
        else if (GetKeyState(VK_LEFT) & 0x8000)
        {
            _rot.y = 180;
        }
        else if (GetKeyState(VK_RIGHT) & 0x8000)
        {
            _rot.y = 0;
        }*/
    }
    // 어쨌든 이동키를 하나라도 누른다면 이동
    if ((GetKeyState(VK_UP) & 0x8000) || (GetKeyState(VK_RIGHT) & 0x8000) || (GetKeyState(VK_DOWN) & 0x8000) || (GetKeyState(VK_LEFT) & 0x8000))
    {
        if(STATE_IDLE == _state)
        {
            _state = STATE_MOVE;
            _frame = FBXManager::MAGICIAN_MOVE_A;
        }
        if(STATE_MOVE == _state)
        {
           // _pos.x += _movement * cos(DEGREE_TO_RADIAN(_rot.y));
           // _pos.z += _movement * sin(DEGREE_TO_RADIAN(_rot.y));
        }
    }
    else
    {
        if(STATE_MOVE == _state)
        {
            _state = STATE_IDLE;
            _frame = FBXManager::MAGICIAN_IDLE_A;
        }

    }
    // 맵 사이즈 밖으로 나가게 된다면?
    if (pow(_mapSize, 2) <= pow(_pos.x, 2) + pow(_pos.z, 2))
    {
        _pos.x = _mapSize * cos(atan2(_pos.z, _pos.x));
        _pos.z = _mapSize * sin(atan2(_pos.z, _pos.x));
//        _rot.y = RADIAN_TO_DEGREE(atan2(_pos.z, _pos.x));
    }
}
void ObjectMagician::updateFrame()
{
    _framePerUpdate++;
    switch(_state)
    {
    case STATE_IDLE:
        if(FRAME_PER_2UPDATE<=_framePerUpdate)
        {
            _framePerUpdate = 0;
            _frame++;
        }
        break;
    case STATE_MOVE:
        if(GetKeyState(VK_SHIFT) & 0x8000)
        {
            if(FRAME_PER_1UPDATE<=_framePerUpdate)
            {
                _framePerUpdate = 0;
                _frame++;
            }
        }
        else
        {
            if(FRAME_PER_2UPDATE<=_framePerUpdate)
            {
                _framePerUpdate = 0;
                _frame++;
            }
        }
        break;
    case STATE_ATTACK:
        if(FRAME_PER_1UPDATE<=_framePerUpdate)
        {
            _framePerUpdate = 0;
            _frame++;
        }
        break;
    case STATE_FLASH:
        if(FRAME_PER_3UPDATE<=_framePerUpdate)
        {
            _framePerUpdate = 0;
            _frame++;
        }
        break;
    }
}
void ObjectMagician::updateState()
{
    switch(_state)
    {
        case STATE_IDLE:
            if(FBXManager::MAGICIAN_IDLE_Z == _frame)
            {
                _state = STATE_IDLE;
                _frame = FBXManager::MAGICIAN_IDLE_A;
            }

            break;
        case STATE_MOVE:
            if(FBXManager::MAGICIAN_MOVE_Z == _frame)
            {
                _state = STATE_MOVE;
                _frame = FBXManager::MAGICIAN_MOVE_A;
            }

            break;
        case STATE_ATTACK:
            // 히트백 : 장풍 발사 반동으로 뒤로 밀린다.
            _pos.x -= powf(FBXManager::MAGICIAN_ATTACK_Z - _frame, 2)/200 * cos(DEGREE_TO_RADIAN(_rot.y));
            _pos.z -= powf(FBXManager::MAGICIAN_ATTACK_Z - _frame, 2)/200 * sin(DEGREE_TO_RADIAN(_rot.y));

            if(0<_pos.y)
            {
                if ((GetKeyState(VK_UP) & 0x8000) || (GetKeyState(VK_RIGHT) & 0x8000) || (GetKeyState(VK_DOWN) & 0x8000) || (GetKeyState(VK_LEFT) & 0x8000))
                {
                    _pos.x += 1.4*_movement * cos(DEGREE_TO_RADIAN(_rot.y));
                    _pos.z += 1.4*_movement * sin(DEGREE_TO_RADIAN(_rot.y));
                }
            }
            if(FBXManager::MAGICIAN_ATTACK_Z == _frame)
            {
                _state = STATE_IDLE;
                _frame = FBXManager::MAGICIAN_IDLE_A;
            }

            break;
        case STATE_FLASH:
            _pos.x += powf(FBXManager::MAGICIAN_FLASH_Z-_frame, 3)/30 * cos(DEGREE_TO_RADIAN(_rot.y));
            _pos.z += powf(FBXManager::MAGICIAN_FLASH_Z-_frame, 3)/30 * sin(DEGREE_TO_RADIAN(_rot.y));
            if(FBXManager::MAGICIAN_FLASH_Z == _frame)
            {
                _state = STATE_IDLE;
                _frame = FBXManager::MAGICIAN_IDLE_A;
            }
            break;
    }
}