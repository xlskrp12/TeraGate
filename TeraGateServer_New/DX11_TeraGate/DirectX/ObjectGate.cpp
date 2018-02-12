#include "ObjectGate.h"
ObjectGate::ObjectGate()
{
    _objectID = ID_GATE;
}
ObjectGate::~ObjectGate()
{
}
void ObjectGate::respawn()
{
    Object::respawn();
    _hp = 4000;
    SoundManager::getSingleton()->_effect.play(1);
}
void ObjectGate::update()
{
    _rot.y += 0.75;
    if(_hp<=0)
        _exist = false;
}
void ObjectGate::updateTargetGateID(Object* object)
{
    D3DXVECTOR3 pos     = object->getPos();
    int         teamID  = object->getTeamID();
    float       range   = 300.f; // �νĹ���
    if(_gateID == GATE_C)
        range = 400;
    // ������Ʈ�� ����Ʈ �ν� ���� ���� ���� ��
    //����Ʈ �ε���
    //0 : ����
    //1 : N
    //2 : N����
    //3 : N����
    //4 : N����
    //5 : L
    //6 : L����
    //7 : L������
    //8 : L����
    //9 : R
    //10 : R����
    //11 : R������
    //12 : R����
    if(sqrt(pow(pos.x-_pos.x, 2)+pow(pos.z-_pos.z, 2)) < _size + object->getSize() + range)
    {
        switch(_gateID)
        {
        case GATE_C:
            switch(teamID)
            {
            case TEAM_N:
                switch(RANDOM_M(0, 1))
                {
                case 0:
                    if(object->getGateID() == GATE_C)
                        object->setGateID(Object::GATE_LC);
                    break;
                case 1:
                    if(object->getGateID() == GATE_C)
                        object->setGateID(Object::GATE_RC);
                    break;
                }//switch(RANDOM
                break;
            case TEAM_L:
                switch(RANDOM_M(0, 1))
                {
                case 0:
                    if(object->getGateID() == GATE_C)
                        object->setGateID(Object::GATE_NC);
                    break;
                case 1:
                    if(object->getGateID() == GATE_C)
                        object->setGateID(Object::GATE_RC);
                    break;
                }//switch(RANDOM
                break;
            case TEAM_R:
                switch(RANDOM_M(0, 1))
                {
                case 0:
                    if(object->getGateID() == GATE_C)
                        object->setGateID(Object::GATE_NC);
                    break;
                case 1:
                    if(object->getGateID() == GATE_C)
                        object->setGateID(Object::GATE_LC);
                    break;
                }//switch(RANDOM
                break;
            }//switch(teamID
            break;
        case GATE_L:
            switch(teamID)
            {
            case TEAM_N:
                object->setGateID(Object::GATE_LR);
                break;
            case TEAM_L:
                break;
            case TEAM_R:
                object->setGateID(Object::GATE_LN);
                break;
            }//switch(teamID
            break;
        case GATE_LN:
            switch(teamID)
            {
            case TEAM_N:
                object->setGateID(Object::GATE_L);
                break;
            case TEAM_L:
                object->setGateID(Object::GATE_NL);
                break;
            case TEAM_R:
                object->setGateID(Object::GATE_L);
                break;
            }//switch(teamID
            break;
        case GATE_LR:
            switch(teamID)
            {
            case TEAM_N:
                object->setGateID(Object::GATE_L);
                break;
            case TEAM_L:
                object->setGateID(Object::GATE_RL);
                break;
            case TEAM_R:
                object->setGateID(Object::GATE_L);
                break;
            }//switch(teamID
            break;
        case GATE_LC:
            switch(teamID)
            {
            case TEAM_N:
                object->setGateID(Object::GATE_L);
                break;
            case TEAM_L:
                object->setGateID(Object::GATE_C);
                break;
            case TEAM_R:
                object->setGateID(Object::GATE_L);
                break;
            }//switch(teamID)
            break;
        case GATE_R:
            switch(teamID)
            {
            case TEAM_N:
                object->setGateID(Object::GATE_RL);
                break;
            case TEAM_L:
                object->setGateID(Object::GATE_RN);
                break;
            case TEAM_R:
                break;
            }//switch(teamID)
            break;
        case GATE_RN:
            switch(teamID)
            {
            case TEAM_N:
                object->setGateID(Object::GATE_R);
                break;
            case TEAM_L:
                // NEW�н����ε�
                if(_map._gate[Object::GATE_NR]._teamID != teamID)
                    object->setGateID(Object::GATE_NR);
                if(_map._gate[Object::GATE_R]._teamID != teamID)
                    object->setGateID(Object::GATE_R);
                break;
            case TEAM_R:
                object->setGateID(Object::GATE_NR);
                break;
            }//switch(teamID)
            break;
        case GATE_RL:
            switch(teamID)
            {
            case TEAM_N:
                object->setGateID(Object::GATE_R);
                break;
            case TEAM_L:
                object->setGateID(Object::GATE_R);
                break;
            case TEAM_R:
                object->setGateID(Object::GATE_LR);
                break;
            }//switch(teamID)
            break;
        case GATE_RC:
            switch(teamID)
            {
            case TEAM_N:
                object->setGateID(Object::GATE_R);
                break;
            case TEAM_L:
                object->setGateID(Object::GATE_R);
                break;
            case TEAM_R:
                object->setGateID(Object::GATE_C);
                break;
            }//switch(teamID)
            break;
        case GATE_N:
            switch(teamID)
            {
            case TEAM_N:
                break;
            case TEAM_L:
                object->setGateID(Object::GATE_NR);
                break;
            case TEAM_R:
                object->setGateID(Object::GATE_NL);
                break;
            }//switch(teamID)
            break;
        case GATE_NL:
            switch(teamID)
            {
            case TEAM_N:
                object->setGateID(Object::GATE_LN);
                break;
            case TEAM_L:
                object->setGateID(Object::GATE_N);
                break;
            case TEAM_R:
                object->setGateID(Object::GATE_N);
                break;
            }
            break;
        case GATE_NR:
            switch(teamID)
            {
            case TEAM_N:
                object->setGateID(Object::GATE_RN);
                break;
            case TEAM_L:
                // NEW�н����ε�
                if(_map._gate[Object::GATE_RN]._teamID != teamID)
                     object->setGateID(Object::GATE_RN);
                if(_map._gate[Object::GATE_N]._teamID != teamID)
                    object->setGateID(Object::GATE_N);

                break;
            case TEAM_R:
                object->setGateID(Object::GATE_N);
                break;
            }
            break;
        case GATE_NC:
            switch(teamID)
            {
            case TEAM_N:
                object->setGateID(Object::GATE_C);
                break;
            case TEAM_L:
                object->setGateID(Object::GATE_N);
                break;
            case TEAM_R:
                object->setGateID(Object::GATE_N);
                break;
            }
            break;
        }// switch(_gateID)
    }//if
}
