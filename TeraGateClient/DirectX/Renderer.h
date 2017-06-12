#pragma once
#include"Prev.h"
#include"Direct.h"
#include"Camera.h"
#include"Model.h"
#include"FBX.h"
#include"TextureShader.h"
#include"LightShader.h"
#include"AmbientLightShader.h"
#include"Light.h"
#include"ObjectManager.h"
class Renderer
{
public:
    Renderer();
    Renderer(const Renderer&);
    ~Renderer();

    void init(HWND, int, int);
    void release();
    void render();
    void update();
private:
    Direct            *_direct;
    Camera            *_camera;
    FBX               *_fbx[7];
    AmbientLightShader*_shader;
    Light             *_light;
    ObjectManager      _objectManager;
};