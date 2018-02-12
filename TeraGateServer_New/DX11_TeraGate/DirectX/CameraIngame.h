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
    // 중력관련
    float _forceY;
    float _forceJump;
    float _gravity;
    float _lowestHeight;
    float _highestHeight;
    float _baseHeight;
    // 이동관련
    // 이동관련
    float _movement;
    float _cameraRotationY;
    float _cameraRotationUnit;
};

