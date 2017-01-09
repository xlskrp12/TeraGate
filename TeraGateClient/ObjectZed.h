#pragma once
#include "Object.h"
#include "ObjectManager.h"
class ObjectZed : public Object
{
public:
    // 인터페이스
    void changeState(int state) {}
    void update() {}
    void render() {}
    void debug() {}
    void proc() {}
protected:
};