#include "ObjectMinion.h"
ObjectMinion::ObjectMinion()
{
    _id    = ID_MINION;
    _size  = 30.0;
    _aiTimer.setCooltime(2);
}
ObjectMinion::~ObjectMinion()
{
}
void ObjectMinion::update()
{
    if (_aiTimer.isOn())
    {
        _des.x = _pos.x + RANDOM_M(-50, 50);
        _des.z = _pos.z + RANDOM_M(-50, 50);
        switch(RANDOM_M(1, 3))
        {
        case 1:
            _aiTimer.setCooltime(0.5);
            break;
        case 2:
            _aiTimer.setCooltime(1.0);
            break;
        case 3:
            _aiTimer.setCooltime(1.5);
            break;
        }
        _rot.y = RADIAN_TO_DEGREE(atan2(_des.z-_pos.z, _des.x-_pos.x));
    }
    static float movement     = 1.0f;
    static float rotationUnit = 0.2f;
    float distance;
    //이동
    distance = sqrt(pow(_des.x-_pos.x, 2)+pow(_des.z-_pos.z, 2));
    if(distance<=0.1)
    {
        _pos.x = _des.x;
        _pos.z = _des.z;
		
    }
    else
    {
        _pos.x += movement * cos(DEGREE_TO_RADIAN(_rot.y));
        _pos.z += movement * sin(DEGREE_TO_RADIAN(_rot.y));
    }


    // 맵 사이즈 밖으로 나가게 된다면?
    if (pow(_mapSize, 2) <= pow(_pos.x, 2) + pow(_pos.z, 2))
    {
        _pos.x = _mapSize * cos(atan2(_pos.z, _pos.x));
        _pos.z = _mapSize * sin(atan2(_pos.z, _pos.x));
        _rot.y = RADIAN_TO_DEGREE(atan2(_pos.z, _pos.x));
    }
}