#include "Object.h"
const float Object::GATE_RADIUS = 3000;
const D3DXVECTOR3 Object::GATE_C_POS  = D3DXVECTOR3(0, 0, 0);
const D3DXVECTOR3 Object::GATE_L_POS  = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(210)), 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(210)));
const D3DXVECTOR3 Object::GATE_LN_POS = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(170)), 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(170)));
const D3DXVECTOR3 Object::GATE_LR_POS = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(250)), 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(250)));
const D3DXVECTOR3 Object::GATE_LC_POS = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(210))/2, 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(210))/2);
const D3DXVECTOR3 Object::GATE_R_POS  = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(-30)), 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(-30)));
const D3DXVECTOR3 Object::GATE_RL_POS = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(-70)), 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(-70)));
const D3DXVECTOR3 Object::GATE_RN_POS = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(10)), 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(10)));
const D3DXVECTOR3 Object::GATE_RC_POS = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(-30))/2, 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(-30))/2);
const D3DXVECTOR3 Object::GATE_N_POS  = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(90)), 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(90)));
const D3DXVECTOR3 Object::GATE_NL_POS = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(130)), 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(130)));
const D3DXVECTOR3 Object::GATE_NR_POS = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(50)), 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(50)));
const D3DXVECTOR3 Object::GATE_NC_POS = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(90))/2, 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(90))/2);
Object::Object()
{
//    _exist           = EXIST_FALSE;
    _control         = CONTROL_FALSE;
    _objectID        = ID_STATIC;
    _teamID          = TEAM_N;
    _state           = STATE_IDLE;
    _frame           = 0;
    _framePerUpdate  = 0;


    _pos.x = 0;
    _pos.y = 0;
    _pos.z = 0;
    _rot.x = 0;
    _rot.y = 0;
    _rot.z = 0;
    _size      = 10.0;
    _sightSize = 0;
    _mapSize   = 3150;
    _movement  = 5;
}
Object::~Object()
{
}
bool Object::isCollision(const Object& object)
{
    //if(abs(_pos.x-object._pos.x) < 200)
    //    return true;
    //if(abs(_pos.z-object._pos.z) < 200)
    //    return true;
    //▼충돌체크 최적화 코드
    // 최적화 코드를 짜고나니 AABB가 뭔질 알겠넹. 이게 AABB구낫. 
    if(fabs(object._pos.x - _pos.x) < object._size + _size)
        if(fabs(object._pos.y - _pos.y) < object._size + _size)
            if(fabs(object._pos.z - _pos.z) < object._size + _size)
                if(pow(_pos.x - object._pos.x, 2) + pow(_pos.y - object._pos.y, 2) + pow(_pos.z - object._pos.z, 2) < pow(_size + object._size, 2))
                {
                    return true;
                }

    return false;
}
bool Object::isInSight(const Object& object)
{
    if(fabs(object._pos.x - _pos.x) < object._size + _sightSize)
        if(fabs(object._pos.y - _pos.y) < object._size + _sightSize)
            if(fabs(object._pos.z - _pos.z) < object._size + _sightSize)
                if(pow(_pos.x - object._pos.x, 2) + pow(_pos.y - object._pos.y, 2) + pow(_pos.z - object._pos.z, 2) < pow(_sightSize + object._size, 2))
                {
                    return true;
                }
    return false;
}
// 거리를 인자로 주면 현재 위치, 방향을 기준으로 pos를 잡아줌
void Object::setPosAway(float distance)
{
    _pos.x = _pos.x + distance * cos(DEGREE_TO_RADIAN(_rot.y));
    _pos.z = _pos.z + distance * sin(DEGREE_TO_RADIAN(_rot.y));
}
// 거리를 인자로 주면 현재 위치, 방향을 기준으로 des를 잡아줌
void Object::setDesByDistance(float distance)
{
    _des.x = _pos.x + distance * cos(DEGREE_TO_RADIAN(_rot.y));
    _des.z = _pos.z + distance * sin(DEGREE_TO_RADIAN(_rot.y));
}
D3DXVECTOR3 Object::getDes()
{
    return _des;
}
void Object::setDes(D3DXVECTOR3 des)
{
    _des = des;
    _rot.y = RADIAN_TO_DEGREE(atan2(_des.z - _pos.z, _des.x - _pos.x));
}
void Object::setPos(D3DXVECTOR3 pos){ _pos = pos; }
void Object::setPos(float x, float y, float z) { _pos = {x, y, z}; };
void Object::setPosX(float x) { _pos.x = x; }
void Object::setPosY(float y) { _pos.y = y; }
void Object::setPosZ(float z) { _pos.z = z; }
void Object::setRot(D3DXVECTOR3 rot) { _rot = rot;}
void Object::setRot(float x, float y, float z) { _rot = {x, y, z}; }
void Object::setRotX(float x){_rot.x = x;}
void Object::setRotY(float y){_rot.y = y;}
void Object::setRotZ(float z){_rot.z = z;}
void Object::moveToDestination()
{
    _rot.y = RADIAN_TO_DEGREE(atan2(_des.z - _pos.z, _des.x - _pos.x));
    _pos.x += _movement * cos(DEGREE_TO_RADIAN(_rot.y));
    _pos.z += _movement * sin(DEGREE_TO_RADIAN(_rot.y));
}
void Object::move(float x, float y, float z) { _pos.x += x; _pos.y += y; _pos.z += z; };
void Object::moveX(float x)   { _pos.x += x; }
void Object::moveY(float y)   { _pos.y += y; }
void Object::moveZ(float z)   { _pos.z += z; }
void Object::rot(float x, float y, float z){ _rot.x+=x; _rot.y+=y; _rot.z+=z;}
void Object::rotX(float x){_rot.x+=x;}
void Object::rotY(float y){_rot.y+=y;}
void Object::rotZ(float z){_rot.z+=z;}
D3DXVECTOR3 Object::getPos()  { return _pos; }
float Object::getPosX() { return _pos.x; }
float Object::getPosY() { return _pos.y; }
float Object::getPosZ() { return _pos.z; }
D3DXVECTOR3 Object::getRot()  { return _rot; }
float Object::getRotX(){ return _rot.x; }
float Object::getRotY(){ return _rot.y; }
float Object::getRotZ(){ return _rot.z; }
float Object::getSize(){ return _size; }
void  Object::setSize(float size){ _size = size; }
int   Object::getID(){ return _objectID; }
void  Object::setID(int id) { _objectID = id; }
int   Object::getTeamID()   { return _teamID; }
void  Object::setTeamID(int teamID) { _teamID = teamID; }
int   Object::getGateID() { return _gateID; }
void  Object::setGateID(int lineID) { _gateID = lineID; }
bool  Object::getExist() { return _exist; }
void  Object::setExist(bool exist) { _exist = exist; }
bool  Object::getControl(){ return _control; }
void  Object::setControl(bool control) { _control = control; }
int   Object::getFrame() { return _frame; }
void  Object::setFrame(int frame){ _frame = frame; }
void  Object::setMovement(float movement) { _movement = movement;}
D3DXMATRIX Object::getMtxWorld()
{
    // 스케일링
    D3DXMatrixScaling(&_mtxScaling, 1, 1, 1);

    // 회전
    D3DXMatrixRotationX(&_mtxRotation[0], DEGREE_TO_RADIAN(_rot.x - 90));     // 맥스 YZ변환 때문에 x축 -90도 회전
    D3DXMatrixRotationY(&_mtxRotation[1], DEGREE_TO_RADIAN((-1*_rot.y)+90));// 맥스 오른손 좌표계->다렉 왼손 좌표계를 만들기 위해 -1을 곱함. 그 전에 회전값 (0, 0, 0)인 오브젝트가 (1, 0, 0) +x축을 바라보게 만들기 y축 90도를 뺌.
    D3DXMatrixRotationZ(&_mtxRotation[2], DEGREE_TO_RADIAN(_rot.z));

    // 오브젝트 월드좌표만큼 이동
    D3DXMatrixTranslation(&_mtxPosition, _pos.x, _pos.y, _pos.z);


    return _mtxScaling *_mtxRotation[0] * _mtxRotation[1] * _mtxRotation[2] * _mtxPosition;
}
void Object::update(){if(_control) updateControl();}
void Object::updateControl(){}
void Object::updateFrame(){}
void Object::updateState(){}
void Object::setArkDes(D3DXVECTOR3 ark, D3DXVECTOR3 des){}
void Object::updateTargetGateID(Object* object){}
void Object::attack(Object& object){object._hp = object._hp - _attack;}
void Object::damage(Object& object){_hp = _hp - object._attack;}
void Object::damage(float dmg) { _hp -= dmg;}
float Object::getHP(){ return _hp;}
void  Object::setHP(float hp){_hp = hp;}
void Object::respawn(){_exist = true;}
int  Object::getState(){return _state;}
void Object::setState(int state){_state = state;}
void Object::updateMap(int gateIndex, int gateTeamID){_map._gate[gateIndex]._teamID = gateTeamID;}