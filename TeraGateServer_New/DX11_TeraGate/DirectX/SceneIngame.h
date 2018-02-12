#pragma once
#include "Scene.h"
class SceneIngame : public Scene
{
public:
    SceneIngame();
    ~SceneIngame();
    void enterScene();
    void init();
    void release();
    void proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
    void update();
    void render();
protected:
    ObjectManager      _uiManager;
    ObjectManager      _staticManager;// 업데이트할 필요가 없는 친구들
    ObjectManager      _objectManager;
    ObjectManager      _effectManager;
    Camera            *_camera;
    Camera            *_cameraPC;
    AmbientLightShader*_shader;
    UI                *_ui;
    Light             *_light;
    Timer              _minionTimer[4];
protected:
    const static int L_TEAM_PC_MAX = 3;
    const static int R_TEAM_PC_MAX = 3;
    const static int N_TEAM_PC_MAX = 4;
    const static int L_MINION_MAX  = 60;
    const static int R_MINION_MAX  = 60;
    const static int N_MINION_MAX  = 60;
};
