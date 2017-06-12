#pragma once
#include"Prev.h"
// 렌더모듈
#include"Direct.h"
#include"Camera.h"
#include"Model.h"
#include"FBX.h"
#include"TextureShader.h"
#include"LightShader.h"
#include"AmbientLightShader.h"
#include"Light.h"
// 오브젝트 매니저
#include"ObjectManager.h"
#include"FBXManager.h"
class Scene
{
public:
    Scene();
    ~Scene();
    virtual void init()                           = 0;
    virtual void release()                        = 0;
    virtual void proc(HWND, UINT, WPARAM, LPARAM) = 0;
    virtual void update()                         = 0;
    virtual void render()                         = 0;
};

