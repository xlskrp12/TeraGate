#pragma once
#include"Prev.h"
class Camera
{
public:
    Camera();
    Camera(const Camera&);
    ~Camera();

    void rot(float, float, float);
    void setPos(D3DXVECTOR3);
    void setPos(float, float, float);
    void setPosX(float);
    void setPosY(float);
    void setPosZ(float);
    void setRot(D3DXVECTOR3);
    void setRot(float, float, float);
    void setRotX(float);
    void setRotY(float);
    void setRotZ(float);
    void setDes(D3DXVECTOR3);
    void setDesX(float x);
    void setDesY(float y);
    void setDesZ(float z);
    D3DXVECTOR3 getPosition();
    D3DXVECTOR3 getRotation();

    virtual void update();
    virtual void updateByMode();
    virtual void updateMode1();
    virtual void updateMode2();
    void render();
    D3DXMATRIX getMtxView();
protected:
    int         _mode;
    D3DXVECTOR3 _pos;
    D3DXVECTOR3 _des;
    D3DXVECTOR3 _rot;
    D3DXMATRIX  _mtxView;
};

