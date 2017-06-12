#pragma once
#include"Prev.h"
class Scene
{
public:
    Scene();
    ~Scene();
    virtual void proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
    virtual void update();
};

