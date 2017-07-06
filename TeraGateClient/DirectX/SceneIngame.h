#pragma once
#include "Scene.h"
class SceneIngame : public Scene
{
public:
    SceneIngame();
    ~SceneIngame();
    void init();
    void release();
    void proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
    void update();
    void render();
protected:
    ObjectManager      _objectManager;
    Camera            *_camera;
    AmbientLightShader*_shader;
    Light             *_light;
};

