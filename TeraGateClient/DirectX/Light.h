#pragma once
#include"Prev.h"
class Light
{
public://▼인터페이스 메소드
    Light();
    Light(const Light&);
    ~Light();

    void setDiffuseColor(float, float, float, float);
    void setAmbientColor(float, float, float, float);
    void setDirection(float, float, float);

    D3DXVECTOR4 getDiffuseColor();
    D3DXVECTOR4 getAmbientColor();
    D3DXVECTOR3 getDirection();
private:
    D3DXVECTOR4 _diffuseColor;
    D3DXVECTOR4 _ambientColor;
    D3DXVECTOR3 _direction;
};

