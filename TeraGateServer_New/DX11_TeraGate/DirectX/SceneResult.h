#pragma once
#include"Scene.h"
class SceneResult  : public Scene
{
public:
    SceneResult();
    ~SceneResult();
    void enterScene();
    void init();
    void release();
    //void update();
    //void render();
protected:
    ObjectManager      _objectManager;
    Camera            *_camera;
    AmbientLightShader*_shader;
    Light             *_light;
    Timer              _logoTimer;
};

