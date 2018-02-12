#include "ObjectManager.h"
ObjectManager::ObjectManager()
{
    _timer.setCooltime(0.05);
}
ObjectManager::~ObjectManager()
{
}
void ObjectManager::addObject(Object* object)
{
    _object.push_back(object);
}
void ObjectManager::updateCommon()
{
    for (int a = 0; a < _object.size(); a++)
    {
        if(true == _object[a]->getExist())
        {
            _object[a]->updateFrame();
            _object[a]->update();
        }
    }
}
void ObjectManager::updateCommon(int i)
{
    if(true == _object[i]->getExist())
        _object[i]->update();
}
void ObjectManager::updateCollision()
{
    D3DXVECTOR3 aPosBeforeUpdate;//자연스럽게 회전시키기 위해 업데이트 이전 a오브젝트 좌표를 받는다. 얘가 없으면 정면충돌할 때 오브젝트가 180도 반대로 회전하는 경우가 생겨 되게 부자연스럽게 보이는 경우가 있따.
    D3DXVECTOR3 aPos;
    D3DXVECTOR3 bPos;
    D3DXVECTOR3 aPosAfterUpdate;  //업데이트 이후 좌표를 받는다.
    float distance;
    float sizeDistance;
    float collisionDistance;
    float rotation;
    //▼오브젝트 업데이트
    // 0~13번 인덱스까지 타워
    for (int a = 0; a< 13; a++)
    {
        if(true == _object[a]->getExist())
        {
            for(int b = 44; b < _object.size(); b++)
            {
                _object[a]->updateTargetGateID(_object[b]);
            }
        }
    }
    // 13~40번 인덱스까지 바위
    //for (int a = 13; a< 34; a++)
    //{
    //}
    // 40~50 인덱스는 PC, 44~MAX 미니언 오브젝트이므로 얘네들이 0~MAX에 충돌하는지 체크
    for (int a = 40; a < _object.size(); a++)
    {
        aPosBeforeUpdate = _object[a]->getPos();
        if(true == _object[a]->getExist())
        {
            _object[a]->update();
            _object[a]->updateFrame();
            _object[a]->updateState();
            if(_object[a]->getID() != Object::ID_STATIC)
            {
                for(int b=0;b<_object.size();b++)
                {
                    if(true == _object[b]->getExist())
                    {
                        if(a != b)
                        {
                            if(_object[a]->isCollision(*_object[b]))
                            {
                                aPos         = _object[a]->getPos();
                                bPos         = _object[b]->getPos();
                                distance     = sqrt(pow(bPos.x-aPos.x, 2)+pow(bPos.z-aPos.z, 2));
                                sizeDistance = _object[a]->getSize()+_object[b]->getSize();
                                if(distance<sizeDistance)
                                {
                                    _object[a]->setPosX(bPos.x + sizeDistance*cos(atan2(aPos.z-bPos.z, aPos.x-bPos.x)));
                                    _object[a]->setPosZ(bPos.z + sizeDistance*sin(atan2(aPos.z-bPos.z, aPos.x-bPos.x)));
                                    aPosAfterUpdate = _object[a]->getPos();
                                    //자연스럽게 회전2
                                    rotation = RADIAN_TO_DEGREE(atan2(aPosAfterUpdate.z-aPosBeforeUpdate.z, aPosAfterUpdate.x-aPosBeforeUpdate.x));
                                    _object[a]->setRotY(rotation);
                                }
                            }
                        }//if(a!=b)
                    }//if(true == _object[b]->getExist()
                }//for(int b=0;
            }//if(_object[a]
        }//if(true == _object[a]->getExist()
    }//for int a=0;
}
int  ObjectManager::getObjectAmount()
{
    return _object.size();
}
D3DXMATRIX ObjectManager::getWorldMatrix(int objectIndex)
{
    return _object[objectIndex]->getMtxWorld();
}