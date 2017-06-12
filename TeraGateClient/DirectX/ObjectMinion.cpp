#include "ObjectMinion.h"
ObjectMinion::ObjectMinion()
{
}
ObjectMinion::~ObjectMinion()
{
}
void ObjectMinion::update()
{
    if (200 < _updateCount)
    {
        //static int aiUpdateTerm = 0;
        //aiUpdateTerm += RANDOM_M(1, 3);
        //if (150 < aiUpdateTerm)
        //{
        //    _rot.y = RANDOM_M(0, 360);
        //    aiUpdateTerm = 0;
        //}
        static float movement = 1.0f;
        static float rotationUnit = 0.2f;

        if (pow(1000, 2) < pow(_pos.x + movement * sin(DEGREE_TO_RADIAN(_rot.y)), 2) + pow(_pos.z + movement * cos(DEGREE_TO_RADIAN(_rot.y)), 2))
        {
            _rot.y += RANDOM_M(0, 360);
        }
        else
        {
            _pos.x += movement * sin(DEGREE_TO_RADIAN(_rot.y));
            _pos.z += movement * cos(DEGREE_TO_RADIAN(_rot.y));
        }
    }
    else
    {
        _updateCount++;
    }
    // _pos, _des가 일정거리 이상 가까워지면 _pos를 _ark로 바꾸고 _updateCount를 0으로
    if (pow(_pos.x - _des.x, 2)+pow(_pos.z-_des.z, 2) < pow(40, 2))
    {
        _updateCount = 0;
        _pos = _ark;
    }
}