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

	//-----------------------------server 추가----------------------------------
	//_server = new Server;
	//---------------------------------------------------------------

    initWindow();

    _keyboard     = new Keyboard;
    _renderer     = new Renderer;
    _sceneManager = new SceneManager;
    _renderer->init(_hwnd, _rectWindow.right, _rectWindow.bottom);
    _sceneManager->init(_keyboard, _renderer);

    PlaySound(L"Resource/Sound/bgmUntitled", NULL, SND_ASYNC | SND_LOOP);
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

	main_window_handle = _hwnd;

    // 윈도우 전위 포커스
    ShowWindow(_hwnd, SW_SHOW);
    SetForegroundWindow(_hwnd);
    SetFocus(_hwnd);
    ShowCursor(true);

	//---------hwnd가 생성된 다음에 해야 함------------------------
	_ServerDef->Init();
	//------------------------------------------------------------
	//---------hwnd가 생성된 다음에 해야 함------------------------
	if (nullptr != main_window_handle)
		_ServerDef->Sync(main_window_handle);
	//------------------------------------------------------------
}
void Game::release()
{
    if(_keyboard)
        delete _keyboard;
    _keyboard = NULL;

    if(_renderer)
        delete _renderer;
    _renderer = NULL;

    if(_sceneManager)
        delete _sceneManager;
    _sceneManager = NULL;

	//-----------------------------server 추가----------------------------------
	if (_ServerDef)
		delete _ServerDef;
	//_server = NULL;

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


		for (int i = 0; i < MAX_USER; i++)
		{
			if (true == _ServerDef->player.exist)
			{
				_renderer->playerObject[i]->_exist = true;
			}
			if (true == _ServerDef->otherPC[i].exist)
			{
				_renderer->playerObject[i]->_exist = true;
				_renderer->playerObject[i]->setPos(_ServerDef->otherPC[i].x, 
					_ServerDef->otherPC[i].y, _ServerDef->otherPC[i].z);
				_renderer->playerObject[i]->setRot(0, _ServerDef->otherPC[i].roty, 0);
				//std::cout << i << " exist" << std::endl;
			}
		}

		for (int i = 0; i < NUM_OF_NPC - NPC_START; i++)
		{
			if (true == _ServerDef->NPC[i].exist)
			{
				_renderer->NPCObject[i]->_exist = true;
				_renderer->NPCObject[i]->setPos(_ServerDef->NPC[i].x, 
					_ServerDef->NPC[i].y, _ServerDef->NPC[i].z);
				_renderer->NPCObject[i]->setRot(0, _ServerDef->NPC[i].roty, 0);
			}
		}
        render();
        update();
    }
}
void Game::render()
{
    _sceneManager->render();
}
void Game::update()
{
    _sceneManager->update();
}
void Game::proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_KEYDOWN:
	{
		_keyboard->down(wparam);
		/*switch (wparam)
		{
			case VK_NUMPAD8:
			case VK_NUMPAD4:
			case VK_NUMPAD6:
			case VK_NUMPAD2:
				cs_packet_down *my_packet = reinterpret_cast<cs_packet_down *>(_server->send_buffer);
				my_packet->size = sizeof(my_packet);
				_server->send_wsabuf.len = sizeof(my_packet);
				my_packet->type = CS_PC_MOVE;

				DWORD iobyte;

				int retval = WSASend(_server->g_mysocket, &_server->send_wsabuf, 1, &iobyte, 0, NULL, NULL);
				if (retval)
				{
					int errCode = WSAGetLastError();
					printf("Error while sending packet [%d]", errCode);
				}
				std::cout << "push num" << std::endl;
				break;
		}*/

		int x = 0;
		int z = 0;

		if (wparam == VK_NUMPAD4)
			x += 1;
		if (wparam == VK_NUMPAD6)
			x -= 1;

		if (wparam == VK_NUMPAD8)
			z += 1;
		if (wparam == VK_NUMPAD2)
			z -= 1;

		cs_packet_up *my_packet = reinterpret_cast<cs_packet_up *>(_ServerDef->send_buffer);
		my_packet->size = sizeof(my_packet);
		_ServerDef->send_wsabuf.len = sizeof(my_packet);

		DWORD iobyte;
		if (0 != x)
		{
			if (1 == x)
			{
				my_packet->type = CS_RIGHT;
				std::cout << "push num6" << std::endl;
			}
			else
			{
				my_packet->type = CS_LEFT;
				std::cout << "push num4" << std::endl;
			}

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
			{
				my_packet->type = CS_UP;
				std::cout << "push num8" << std::endl;
			}
			else
			{
				my_packet->type = CS_DOWN;
				std::cout << "push num2" << std::endl;
			}

			int retval = WSASend(_ServerDef->g_mysocket, &_ServerDef->send_wsabuf, 1, &iobyte, 0, NULL, NULL);
			if (retval)
			{
				int errCode = WSAGetLastError();
				printf("Error while sending packet [%d]", errCode);
			}
		}


		
	}
        break;
    case WM_KEYUP:
        _keyboard->up(wparam);
        break;
    case WM_PAINT:
        /*
            PAINTSTRUCT ps;
            HDC hdc;
            hdc = BeginPaint(hwnd, &ps);
            Rectangle(hdc, 0, 0, 100, 100);
            EndPaint(hwnd, &ps);
        */
		/*for (int i = 0; i < MAX_USER; i++)
		{
			if(i==g_myid)
				_renderer->playerObject[i]->setPos(_ServerDef->player.x, _ServerDef->player.y, _ServerDef->player.z);
		}*/
        break;

	/*case WM_SOCKET:
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
				_server->ReadPacket((SOCKET)wparam);
				std::cout << "read" << std::endl;
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
	break;*/

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

    default:
        return DefWindowProc(hwnd, message, wparam, lparam);
    }
}