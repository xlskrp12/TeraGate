#pragma once
#include "Scene.h"
class SceneIntro : public Scene
{
public:
    SceneIntro();
    ~SceneIntro();
    void enterScene();
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
    Timer              _logoTimer;
};

