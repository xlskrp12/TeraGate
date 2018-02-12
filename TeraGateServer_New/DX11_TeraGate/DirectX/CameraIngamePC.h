#pragma once
#include"Camera.h"
class CameraIngamePC : public Camera
{
public:
    CameraIngamePC();
    ~CameraIngamePC();
    void updateMode1();
    void updateMode2();
protected:
    float _distance;
    float _distanceSub;
    //����2 ����
    // �߷°���
    float _forceY;
    float _forceJump;
    float _gravity;
    float _lowestHeight;
    float _highestHeight;
    float _baseHeight;
    // �̵�����
    float _movement;
    float _cameraRotationY;
    float _cameraRotationUnit;
};

