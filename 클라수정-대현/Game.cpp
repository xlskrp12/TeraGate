#include"Game.h"
// 싱글톤 
Game* Game::_singleton = NULL;
Game* Game::getSingletone()
{
    if(_singleton == NULL)
        _singleton = new Game;
    return _singleton;
}
Game::Game(){}
void Game::init()
{
    initWindow();

	//--------------------------------------------------------------------------
	_keyboard = new Keyboard;
	//--------------------------------------------------------------------------

    _sceneManager = new SceneManager;
	//_sceneManager->init(_hwnd, _rectWindow.right, _rectWindow.bottom);

	//--------------------------------------------------------------------------
    _sceneManager->init(_hwnd, _rectWindow.right, _rectWindow.bottom, _keyboard);
	//--------------------------------------------------------------------------

    // 브금 bgm 배경음
    // PlaySound(L"Resource/Sound/bgmUntitled", NULL, SND_ASYNC | SND_LOOP);
#ifdef _DEBUG
    printf("Game::init()\n");
#endif
}
void Game::initWindow()
{
	//-----------------------------server 추가----------------------------------
	printf("서버 ip 주소를 입력하시오: ");
	scanf("%s", _ServerDef->SERVERIP);

	_wsetlocale(LC_ALL, L"korean");
	std::wcout.imbue(std::locale("kor"));



	//---------------------------------------------------------------

    WNDCLASSEX   window;
    DEVMODE      devmode;

    // 윈도우 클래스 초기화 후 등록
    window.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window.lpfnWndProc   = WinProc; // 이 부분 때문에 WinProc을 Game.cpp/h에 선언.. 영 맘에 안드는 부분.
    window.cbClsExtra    = 0;
    window.cbWndExtra    = 0;
    window.hInstance     = _hInstance;
    window.hIcon         = (HICON)LoadImage(NULL, TEXT("Resource/Windows/Venom.ico"), IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    window.hIconSm       = window.hIcon;
    window.hCursor       = (HICON)LoadImage(NULL, TEXT("Resource/Windows/Venom.ani"), IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    window.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    window.lpszMenuName  = NULL;
    window.lpszClassName = _appTitle;
    window.cbSize        = sizeof(WNDCLASSEX);
    RegisterClassEx(&window);

    _rectWindow = { 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
    if(FULL_SCREEN)
    {
        //윈도영역
        _rectWindow.left   = 0; 
        _rectWindow.top    = 0; 
        _rectWindow.right  = GetSystemMetrics(SM_CXSCREEN);
        _rectWindow.bottom = GetSystemMetrics(SM_CYSCREEN);
        //devmode설정
        memset(&devmode, 0, sizeof(devmode));
        devmode.dmSize       = sizeof(devmode);
        devmode.dmPelsWidth  = GetSystemMetrics(SM_CXSCREEN);
        devmode.dmPelsHeight = GetSystemMetrics(SM_CYSCREEN);
        devmode.dmBitsPerPel = 32;
        devmode.dmFields     = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
        //devmode값에 따라 풀스크린 모드로
        ChangeDisplaySettings(&devmode, CDS_FULLSCREEN);
        //윈도생성
        _hwnd = CreateWindowEx(
            WS_EX_APPWINDOW,
            _appTitle, _appTitle,
            WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_POPUP,
            _rectWindow.left, _rectWindow.top, _rectWindow.right, _rectWindow.bottom,
            NULL, NULL, _hInstance, NULL);
    }
    else
    {
        //윈도영역
        _rectWindow.left   = (GetSystemMetrics(SM_CXSCREEN) - CLIENT_WIDTH)  / 2; 
        _rectWindow.top    = (GetSystemMetrics(SM_CYSCREEN) - CLIENT_HEIGHT) / 2; 
        _rectWindow.right  = CLIENT_WIDTH;
        _rectWindow.bottom = CLIENT_HEIGHT;
        // 타이틀바 없게
        //_hwnd = CreateWindowEx(WS_EX_APPWINDOW,
        //    _appTitle, _appTitle,
        //    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
        //    _rectWindow.left, _rectWindow.top,
        //    _rectWindow.right, _rectWindow.bottom,
        //    NULL, NULL, _hInstance, NULL);
        
        // 타이틀바 있게
        _hwnd = CreateWindowEx(
            WS_EX_APPWINDOW,
            _appTitle, _appTitle,
            WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
            _rectWindow.left, _rectWindow.top,
            _rectWindow.right, _rectWindow.bottom,
            NULL, NULL, _hInstance, NULL);        
        
    }

	//------------------------------------------------------------
	main_window_handle = _hwnd;
	//------------------------------------------------------------


    // 윈도우 전위 포커스
    ShowWindow(_hwnd, SW_SHOW);
    SetForegroundWindow(_hwnd);
    SetFocus(_hwnd);
    ShowCursor(true);

	//---------hwnd가 생성된 다음에 해야 함------------------------
	_ServerDef->Init();

	if (nullptr != main_window_handle)
		_ServerDef->Sync(main_window_handle);
	//------------------------------------------------------------
}
void Game::release()
{
	if (_keyboard)
		delete _keyboard;
	_keyboard = NULL;

    if(_sceneManager)
        delete _sceneManager;
    _sceneManager = NULL;

	//-----------------------------server 추가----------------------------------
	if (_ServerDef)
		delete _ServerDef;
	//---------------------------------------------------------------

    releaseWindow();
}
void Game::releaseWindow()
{
    ShowCursor(true);
    if (FULL_SCREEN)
        ChangeDisplaySettings(NULL, 0);
    DestroyWindow(_hwnd);
    UnregisterClass(_appTitle, _hInstance);
}
void Game::run()
{
    MSG  msg;
    bool isRunning = true;
    ZeroMemory(&msg, sizeof(MSG));
    while (isRunning)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (msg.message == WM_QUIT)
            isRunning = false;
        update();
        render();
    }
}
void Game::render()
{
    _sceneManager->render();




    //▼타이머로 윈도우 타이틀 갱신
    _timer.render();
    if(0.3334f <= _timer.getTime()) // 0.3334초에 한 번씩 윈도우 타이틀 갱신, 타이머 리셋
    {
        char  mBuffer[100];
        WCHAR wBuffer[100];
        sprintf_s(mBuffer, sizeof(mBuffer), "%ls    FPS %.2f    LPS %.2f    PlayTime %.0fs", _appTitle, _timer.getRenderFPS(), _timer.getLPS(), _timer.getRuntime());
        MultiByteToWideChar(CP_ACP, NULL, mBuffer, -1, wBuffer, sizeof(wBuffer));
        ::SetWindowText(_hwnd, wBuffer);
        _timer.reset();
    }
}
void Game::update()
{
    if(_timer.isGoingUpdate())
    {
        _timer.update();
        _sceneManager->update();
    }
}
void Game::proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
		//---------------------------------------------------------------
	case WM_KEYDOWN:
	{
		_keyboard->down(wparam);

		int w = 0;
		int x = 0;
		int y = 0;
		int z = 0;



		//if ((GetKeyState(VK_UP) & 0x8000) && (GetKeyState(VK_RIGHT) & 0x8000))
		if ((VK_UP == wparam && VK_RIGHT == wparam))
			w += 1;
		// ↖
		//else if ((GetKeyState(VK_UP) & 0x8000) && (GetKeyState(VK_LEFT) & 0x8000))
		else if ((VK_UP == wparam && VK_LEFT == wparam))
			w -= 1;
		// ↘
		//else if ((GetKeyState(VK_DOWN) & 0x8000) && (GetKeyState(VK_RIGHT) & 0x8000))
		else if ((VK_DOWN == wparam && VK_RIGHT == wparam))
			y += 1;
		// ↙
		//else if ((GetKeyState(VK_DOWN) & 0x8000) && (GetKeyState(VK_LEFT) & 0x8000))
		else if ((VK_DOWN == wparam && VK_LEFT == wparam))
			y -= 1;

		else if (VK_UP == wparam)
			z += 1;
		else if (VK_DOWN == wparam)
			z -= 1;
		else if (VK_LEFT == wparam)
			x += 1;
		else if (VK_RIGHT == wparam)
			x -= 1;

		cs_packet_up *my_packet = reinterpret_cast<cs_packet_up *>(_ServerDef->send_buffer);
		my_packet->size = sizeof(my_packet);
		_ServerDef->send_wsabuf.len = sizeof(my_packet);

		DWORD iobyte;

		if (0 != w)
		{
			if (1 == w)
				my_packet->type = CS_RIGHT_UP;
			else
				my_packet->type = CS_LEFT_UP;

			int retval = WSASend(_ServerDef->g_mysocket, &_ServerDef->send_wsabuf, 1, &iobyte, 0, NULL, NULL);
			if (retval)
			{
				int errCode = WSAGetLastError();
				printf("Error while sending packet [%d]", errCode);
			}
		}

		if (0 != y)
		{
			if (1 == y)
				my_packet->type = CS_RIGHT_DOWN;
			else
				my_packet->type = CS_LEFT_DOWN;

			int retval = WSASend(_ServerDef->g_mysocket, &_ServerDef->send_wsabuf, 1, &iobyte, 0, NULL, NULL);
			if (retval)
			{
				int errCode = WSAGetLastError();
				printf("Error while sending packet [%d]", errCode);
			}
		}

		if (0 != z)
		{
			if (1 == z)
				my_packet->type = CS_UP;
			else
				my_packet->type = CS_DOWN;

			int retval = WSASend(_ServerDef->g_mysocket, &_ServerDef->send_wsabuf, 1, &iobyte, 0, NULL, NULL);
			if (retval)
			{
				int errCode = WSAGetLastError();
				printf("Error while sending packet [%d]", errCode);
			}
		}

		if (0 != x)
		{
			if (1 == x)
				my_packet->type = CS_RIGHT;
			else
				my_packet->type = CS_LEFT;

			int retval = WSASend(_ServerDef->g_mysocket, &_ServerDef->send_wsabuf, 1, &iobyte, 0, NULL, NULL);
			if (retval)
			{
				int errCode = WSAGetLastError();
				printf("Error while sending packet [%d]", errCode);
			}
		}


	}
	break;
	//---------------------------------------------------------------


    case WM_KEYUP:
		//---------------------------------------------------------------
		_keyboard->down(wparam);
		//---------------------------------------------------------------
        break;


    case WM_PAINT:
        /*
            PAINTSTRUCT ps;
            HDC hdc;
            hdc = BeginPaint(hwnd, &ps);
            Rectangle(hdc, 0, 0, 100, 100);
            EndPaint(hwnd, &ps);
        */
        break;
    default:
        break;
    }
}
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    Game::getSingletone()->proc(hwnd, message, wparam, lparam);
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

	//---------------------------------------------------------------
	case WM_SOCKET:
	{
		if (WSAGETSELECTERROR(lparam))
		{
			{
				closesocket((SOCKET)wparam);
				exit(-1);
			}
			break;
		}

		switch (WSAGETSELECTEVENT(lparam))
		{
		case FD_READ:
		{
			_ServerDef->ReadPacket((SOCKET)wparam);
			//std::cout << "read" << std::endl;
		}
		break;
		case FD_CLOSE:
		{
			closesocket((SOCKET)wparam);
			exit(-1);
		}
		break;
		}
	}
	return 0;

	//---------------------------------------------------------------

    default:
        return DefWindowProc(hwnd, message, wparam, lparam);
    }
}