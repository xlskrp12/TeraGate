#pragma once
#include"Prev.h"
#include"SceneManager.h"
#include"GameTimer.h"
class Game{
//�� �̱��� ���� 
// �̱��澲�� ������ WinProc()���� getSingletone()�Ҷ��.
// WinProc()���� �̱������� proc()�� ó���ؾ��ϴ� ������
// GameŬ������ ���������� ���� �����ϱ�..
// ���� ���߿� �ҽ��ڵ� ���ٰ� �̰� �� �̱������� ����? �ϸ鼭 �������� ����..
private:
    Game();
    static Game* _singleton;
public:
    static Game* getSingletone();
//�� �޼ҵ�
public:
    void init();
    void release();
    void run();
    void proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
private:
    void initWindow();
    void releaseWindow();
    void render();// run() ����ȣ�� �Լ�1
    void update();// run() ����ȣ�� �Լ�2
//�� ������� 
private:
    // ��ǲ, �׷���
    SceneManager* _sceneManager = NULL;
    // ������ ����
    LPCWSTR   _appTitle    = L"TeraGate";
    HINSTANCE _hInstance   = GetModuleHandle(NULL);
    HWND      _hwnd        = NULL;
    RECT      _rectWindow;
    GameTimer _timer;

	//--------------------------------------------------------------------------
	Keyboard* _keyboard;
	//--------------------------------------------------------------------------
};
LRESULT __stdcall WinProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);