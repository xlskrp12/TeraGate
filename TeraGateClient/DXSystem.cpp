#include"DXSystem.h"
DXSystem::DXSystem()
{
    _input   = 0;
    _graphic = 0;
}
DXSystem::DXSystem(const DXSystem& other)
{

}
DXSystem::~DXSystem()
{

}
// - 인풋&그래픽 객체 초기화
// - 어플리케이션이 사용할 윈도우 생성
bool DXSystem::init()
{
    bool result;
    int  screenWidth, screenHeight;

    screenWidth = 0; screenHeight = 0;
    initWindow(screenWidth, screenHeight);

    // InputManager 객체 생성, 초기화
    _input = new Input;
    if (!_input)
        return false;
    _input->init();

    // GraphicManager 객체 생성, 초기화
    _graphic = new Graphic;
    if (!_graphic)
        return false;
    result = _graphic->init(screenWidth, screenHeight, _window);
    if (!result)
        return false;

    return true;
}
// - 인풋&그래픽 객체와 윈도우 관련 핸들을 정리한다.
void DXSystem::shut()
{
    if (_graphic)
    {
        _graphic->shut();
        delete _graphic;
        _graphic = 0;
    }
    if (_input)
    {
        delete _input;
        _input = 0;
    }
    shutWindow();
    return;
}
// - 프로그램이 종료될 때까지 루프를 돌며 어플리케이션의 모든 작업을 처리한다.
// - 어플리케이션의 모든 작업은 매 루프마다 불리는 frame() 함수에서 수행된다.
void DXSystem::run()
{
    MSG  message;
    bool isRunning;
    // 메시지 구조체 초기ㅘ
    ZeroMemory(&message, sizeof(MSG));
    // 유저로부터 종료 메시지를 받을 때까지 루프를 돈다.
    isRunning = true;
    while (isRunning)
    {
        // 윈도우 메시지 처리
        if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        // 윈도우 종료 메시지면 루프를 빠져나온다.
        if (message.message == WM_QUIT)
            isRunning = false;
        // frame() 함수 수행
        // 정상 수행시 true  리턴
        // 종료 수행시 false 리턴, 루프를 빠져나온다.
        if (frame() == false)
            isRunning = false;
    }
    return;
}
// - 어플리케이션의 모든 작업 처리
bool DXSystem::frame()
{
    // Esc 입력시 어플리케이션 종료
    if (_input->isKeyDown(VK_ESCAPE))
        return false;
    if (_input->isKeyDown(VK_RETURN)) // 여기서 오브젝트 매니저 개체 수 + 1
        return false;
    // 그래픽 객체의 작업을 처리
    if (_graphic->frame() == false)
        return false;
    // run() 루프 계속 수행
    return true;
}
// - 윈도우 메시지 처리
// - WndProc() static 전역 함수 내에서 호출된다.
LRESULT CALLBACK DXSystem::procMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_KEYDOWN:
        _input->keyDown((unsigned int)wParam);
        return 0;
    case WM_KEYUP:
        _input->keyUp((unsigned int)wParam);

        //        ObjectManager::getInstance()->add(new Object);
        break;
    default:
        return DefWindowProc(window, message, wParam, lParam);
    }// switch
}
// - 렌더링할 윈도우 생성
void DXSystem::initWindow(int& screenWidth, int& screenHeight)
{
    WNDCLASSEX windowClass;
    DEVMODE    screenSetting;
    int        posX, posY;

    // 외부 포인터를 이 객체로 설정
    gDXSystem = this;

    // 이 어플리케이션의 인스턴스를 가져온다.
    _instance = GetModuleHandle(NULL);
    // 어플리케이션의 이름을 설정
    _applicationName = L"GameEngine";
    // 윈도우 클래스 초기화, 등록
    windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    windowClass.lpfnWndProc = WndProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = _instance;
    windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    windowClass.hIconSm = windowClass.hIcon;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = _applicationName;
    windowClass.cbSize = sizeof(WNDCLASSEX);
    RegisterClassEx(&windowClass);
    // 모니터 화면의 해상도를 알아온다.
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);
    if (FULL_SCREEN)
    {
        // 풀스크린 모드라면 화면 크기를 데스크톱 크기에 맞추고 색상을 32bit로
        memset(&screenSetting, 0, sizeof(screenSetting));
        screenSetting.dmSize = sizeof(screenSetting);
        screenSetting.dmPelsWidth = (unsigned long)screenWidth;
        screenSetting.dmPelsHeight = (unsigned long)screenHeight;
        screenSetting.dmBitsPerPel = 32;
        screenSetting.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
        // 스크린에 맞는 디스플레이 설정
        ChangeDisplaySettings(&screenSetting, CDS_FULLSCREEN);
        // 윈도우 위치를 화면 왼쪽 위로 맞춘다.
        posX = posY = 0;
    }
    else
    {
        // 윈도우 모드라면 800x600 크기로 맞추고 화면 중앙에 오도록 한다.
        screenWidth  = 800;
        screenHeight = 600;
        posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
        posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
    }
    // 설정한 대로 윈도우 창을 만들고 핸들을 가져온다.
    _window = CreateWindowEx(WS_EX_APPWINDOW,
        _applicationName, _applicationName,
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
        posX, posY, screenWidth, screenHeight, NULL, NULL, _instance, NULL);
    // 윈도우를 화면에 표시하고 포커스를 준다.
    ShowWindow(_window, SW_SHOW);
    SetForegroundWindow(_window);
    SetFocus(_window);
    // 마우스 커서를 표시하지 않는다.
    ShowCursor(false);
    return;
}
// - 화면 설정을 되돌리고 윈도우&핸들을 반환
void DXSystem::shutWindow()
{
    // 마우스 커서를 표시
    ShowCursor(true);
    // 풀스크린 모드를 빠져나올 때 디스플레이 설정을 바꾼다.
    if (FULL_SCREEN)
        ChangeDisplaySettings(NULL, 0);
    // 창을 제거한다.
    DestroyWindow(_window);
    _window = NULL;
    // 이 클래스에 대한 외부 포인터 참조를 제거한다.
    gDXSystem = NULL;
    return;
}
// - 윈도우 메시지 처리
// - GameEngine::procMessage() 함수를 호출한다.
LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        // 윈도우가 제거되었는지 확인
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
        // 윈도우가 닫히는지 확인
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    default:
        return gDXSystem->procMessage(window, message, wParam, lParam);
    }
}