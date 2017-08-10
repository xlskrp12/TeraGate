#pragma once
#include"Prev.h"
#include"SceneManager.h"
//---------------------------------------------------------------
//#include"Server.h"
//---------------------------------------------------------------
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

	//---------------------------------------------------------------
	//Server *_server;
	//---------------------------------------------------------------
private:
    void initWindow();
    void releaseWindow();
    void render();// run() ����ȣ�� �Լ�1
    void update();// run() ����ȣ�� �Լ�2
//�� ������� 
private:
    // ��ǲ, �׷���
    Keyboard* _keyboard    = NULL;
    Renderer* _renderer    = NULL;
    SceneManager* _sceneManager = NULL;
    // ������ ����
    LPCWSTR   _appTitle    = L"TeraGate";
    HINSTANCE _hInstance   = GetModuleHandle(NULL);
    HWND      _hwnd        = NULL;
    RECT      _rectWindow;

	
};
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);