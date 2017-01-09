#pragma once
#include<D3DX10math.h>
class Camera
{
public:
    Camera();
    Camera(const Camera&);
    ~Camera();

    void setPosition(float, float, float);
    void setRotation(float, float, float);

    D3DXVECTOR3 getPosition();
    D3DXVECTOR3 getRotation();

    void render();
    void getViewMatrix(D3DXMATRIX&);
private:
    float      _positionX, _positionY, _positionZ;
    float      _rotationX, _rotationY, _rotationZ;
    D3DXMATRIX _viewMatrix;
};