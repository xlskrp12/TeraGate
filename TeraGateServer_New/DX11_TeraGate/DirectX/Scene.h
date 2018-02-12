#pragma once
#include"Prev.h"
// 렌더모듈
#include"Direct.h"
#include"Camera.h"
#include"CameraIngame.h"
#include"CameraIngamePC.h"
#include"Model.h"
#include"FBX.h"
#include"UI.h"
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
    int  getSceneState();
    void setSceneState(int state);
    virtual void enterScene()                     = 0;
    virtual void init()                           = 0;
    virtual void release()                        = 0;
    virtual void proc(HWND, UINT, WPARAM, LPARAM) = 0;
    virtual void update()                         = 0;
    virtual void render()                         = 0;
public:
    const static int STATE_INGAME = 0;
    const static int STATE_INTRO  = 1;
    const static int STATE_RESULT = 2;
    const static int STATE_VOLUME = 3;
    int _state;
};

