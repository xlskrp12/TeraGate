#pragma once
#include"Prev.h"
class Camera
{
public:
    Camera();
    Camera(const Camera&);
    ~Camera();

    void        setPos(D3DXVECTOR3);
    void        setPos(float, float, float);
    void        setRot(D3DXVECTOR3);
    void        setRot(float, float, float);
    D3DXVECTOR3 getPosition();
    D3DXVECTOR3 getRotation();

    void update();
    void render();
    D3DXMATRIX getMtxView();
private:
    D3DXVECTOR3 _pos;
    D3DXVECTOR3 _rot;
    D3DXMATRIX  _mtxView;
};

