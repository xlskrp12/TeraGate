#pragma once
#include<vector>
#include"ObjectEffect.h"
#include"ObjectGate.h"
#include"ObjectGateCore.h"
#include"ObjectMinion.h"
#include"ObjectMagician.h"
#include"ObjectSlash.h"
#include"ObjectShuriken.h"
#include"ObjectStatic.h"
class ObjectManager
{
public:
    ObjectManager();
    ~ObjectManager();
    void addObject(Object* object);
    void updateCommon();
    void updateCommon(int i);
    void updateCollision();
    int  getObjectAmount();
    D3DXMATRIX getWorldMatrix(int objectIndex);
    std::vector<Object*> _object;

    Timer _timer;
};
