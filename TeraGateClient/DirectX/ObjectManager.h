#pragma once
#include<vector>
#include"ObjectGate.h"
#include"ObjectGround.h"
#include"ObjectMinion.h"
#include"ObjectPC.h"
#include"ObjectStone.h"
#include"ObjectTile.h"
class ObjectManager
{
public:
    ObjectManager();
    ~ObjectManager();
    void addObject(Object* object);
    void update();
    int  getObjectAmount();
    D3DXMATRIX getWorldMatrix(int objectIndex);
    std::vector<Object*> _object;
};