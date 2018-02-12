#include "ObjectMinion.h"
ObjectMinion::ObjectMinion()
{
    _objectID   = ID_MINION;
    _movement   = 5.0f;
    _size       = 40.0;
    _sightSize  = 360.0;
    _hp         = 60;
    _attack     = 1;
    _lineRadius = 2800;
    _aiTimer.setCooltime(0.3);
    _state      = STATE_MOVE;
    _attackRange = 40;
}
ObjectMinion::~ObjectMinion()
{
}
void ObjectMinion::respawn()
{
    _exist      = true;
    _objectID   = ID_MINION;
    _movement   = 5.0f;
    _size       = 40.0;
    _sightSize  = 360.0;
    _hp         = 60;
    _attack     = 1;
    _lineRadius = 2800;
    _aiTimer.setCooltime(0.3);
    _state      = STATE_MOVE;
    _attackRange = 40;
}
void ObjectMinion::setState(int state)
{
    switch(state)
    {
    case STATE_MOVE:
        _framePerUpdate = 0;
        _frame = FBXManager::MINION_MOVE_A+RANDOM_M(0, 3);
        _state = STATE_MOVE;
        break;
    case STATE_ATTACK:
        _framePerUpdate = 0;
        _frame = FBXManager::MINION_ATTACK_A;
        _state = STATE_ATTACK;
        break;
    }
}
void ObjectMinion::update()
{
    if(_hp <= 0)
        _exist = false;
    //▼라인따라 가는 AI
    if (_aiTimer.isOn())
    {
        switch(_gateID)
        {
        case GATE_C:
            _des = GATE_C_POS;
            break;
        case GATE_L:
            _des = GATE_L_POS;
            break;
        case GATE_LN:
            _des = GATE_LN_POS;
            break;
        case GATE_LR:
            _des = GATE_LR_POS;
            break;
        case GATE_LC:
            _des = GATE_LC_POS;
            break;
        case GATE_R:
            _des = GATE_R_POS;
            break;
        case GATE_RN:
            _des = GATE_RN_POS;
            break;
        case GATE_RL:
            _des = GATE_RL_POS;
            break;
        case GATE_RC:
            _des = GATE_RC_POS;
            break;
        case GATE_N:
            _des = GATE_N_POS;
            break;
        case GATE_NL:
            _des = GATE_NL_POS;
            break;
        case GATE_NR:
            _des = GATE_NR_POS;
            break;
        case GATE_NC:
            _des = GATE_NC_POS;
            break;
        }// switch(_gateID)
        _rot.y = RADIAN_TO_DEGREE(atan2(_des.z-_pos.z, _des.x-_pos.x));
    }
    //이동
    _distance = sqrt(pow(_des.x-_pos.x, 2)+pow(_des.z-_pos.z, 2));
    if(_distance<=0.1)
    {
        _pos.x = _des.x;
        _pos.z = _des.z;
    }
    else
    {
        _pos.x += _movement * cos(DEGREE_TO_RADIAN(_rot.y));
        _pos.z += _movement * sin(DEGREE_TO_RADIAN(_rot.y));
    }
    // 맵 사이즈 밖으로 나가게 된다면?
    if (pow(_mapSize, 2) <= pow(_pos.x, 2) + pow(_pos.z, 2))
    {
        _pos.x = _mapSize * cos(atan2(_pos.z, _pos.x));
        _pos.z = _mapSize * sin(atan2(_pos.z, _pos.x));
        _rot.y = RADIAN_TO_DEGREE(atan2(_pos.z, _pos.x));
    }
}
void ObjectMinion::attack(Object& object)
{
    D3DXVECTOR3 pos;
    float size;
    pos  = object.getPos();
    size = object.getSize();
    if(sqrt(pow(_pos.z-pos.z, 2)+pow(_pos.x-pos.x, 2)) <= _size+_attackRange+size)
        Object::attack(object);
}
void ObjectMinion::updateFrame()
{
    _framePerUpdate++;
    switch(_state)
    {
    case STATE_MOVE:
        if(FRAME_PER_4UPDATE<=_framePerUpdate)
        {
            _framePerUpdate = 0;
            _frame++;
        }
        break;
    case STATE_ATTACK:
        if(FRAME_PER_2UPDATE<=_framePerUpdate)
        {
            _framePerUpdate = 0;
            _frame++;
        }
        break;
    }
    if(FBXManager::MINION_TOTAL<=_frame)
        setState(STATE_MOVE);
}
void ObjectMinion::updateState()
{
    switch(_state)
    {
        case STATE_MOVE:
            _movement = 4.0;
            if(FBXManager::MINION_MOVE_Z == _frame)
                setState(STATE_MOVE);
            break;
        case STATE_ATTACK:
            _movement = 8.0;
            if(FBXManager::MINION_ATTACK_Z == _frame)
                setState(STATE_MOVE);
            break;
    }
}
