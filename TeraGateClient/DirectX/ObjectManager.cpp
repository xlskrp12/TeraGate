#include "ObjectManager.h"
ObjectManager::ObjectManager()
{
}
ObjectManager::~ObjectManager()
{
}
void ObjectManager::addObject(Object* object)
{
    _object.push_back(object);
}
void ObjectManager::update()
{
    for (int i = 0; i < _object.size(); i++)
        _object[i]->update();
}
int  ObjectManager::getObjectAmount()
{
    return _object.size();
}
D3DXMATRIX ObjectManager::getWorldMatrix(int objectIndex)
{
    return _object[objectIndex]->getMtxWorld();
}