/*****************************************************************************************************
FileName : Graphic.h
*****************************************************************************************************/
#pragma once
#include"Direct.h"
#include"Camera.h"
#include"Model.h"
#include"ColorShader.h"
const bool  FULL_SCREEN   = false;
const bool  VSYNC_ENABLED = true;
const float SCREEN_DEPTH  = 1000.0f;
const float SCREEN_NEAR   = 0.1f;
class Graphic
{
public:
    Graphic();
    Graphic(const Graphic&);
    ~Graphic();

    bool init(int, int, HWND);
    void shut();
    bool frame();
private:
    bool render();
private:
    Direct*      _direct;
    Camera*      _camera;
    Model*       _model;
    ColorShader* _colorShader;
};