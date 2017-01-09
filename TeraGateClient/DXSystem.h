/*****************************************************************************************************
FileName : GameEngine.h
*****************************************************************************************************/
#pragma once
#define WIN32_LEAN_AND_MEAN // 자주 사용되지 않는 API를 담고있는 Win32헤더를 제외해 빌드 속도 향상
#include<Windows.h>
#include"ObjectManager.h"
#include"Input.h"
#include"Graphic.h"
class DXSystem
{
public:
    DXSystem();
    DXSystem(const DXSystem&);
    ~DXSystem();

    bool init();
    void shut();
    void run();

    LRESULT CALLBACK procMessage(HWND, UINT, WPARAM, LPARAM);
private:
    bool frame();
    void initWindow(int&, int&);
    void shutWindow();
private:
    LPCWSTR   _applicationName;
    HINSTANCE _instance;
    HWND      _window;

    Input*   _input;
    Graphic* _graphic;
};
// 전역 함수
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// 전역 변수
static DXSystem* gDXSystem = 0;