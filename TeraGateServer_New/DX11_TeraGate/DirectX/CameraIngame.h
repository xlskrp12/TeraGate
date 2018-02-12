#pragma once
#include"Camera.h"
class CameraIngame : public Camera
{
public:
    CameraIngame();
    ~CameraIngame();
    void update();
    void updateCameraMode();
    void updateMode1();
    void updateMode2();

private:
    // �߷°���
    float _forceY;
    float _forceJump;
    float _gravity;
    float _lowestHeight;
    float _highestHeight;
    float _baseHeight;
    // �̵�����
    // �̵�����
    float _movement;
    float _cameraRotationY;
    float _cameraRotationUnit;
};

