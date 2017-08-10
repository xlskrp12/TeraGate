#include"Direct.h"
Direct::Direct()
{
    _swapchain          = NULL;
    _device             = NULL;
    _deviceContext      = NULL;
    _renderTargetView   = NULL;
    _depthStencilBuffer = NULL;
    _depthStencilView   = NULL;
    _rasterizerState    = NULL;
}
void Direct::init(HWND hwnd, bool fullscreen, bool vsync, int screenWidth, int screenHeight, float screenDepth, float screenNear)
{
    _hwnd         = hwnd;
    _isFullscreen = fullscreen;
    _isVsync      = vsync;
    _screenWidth  = screenWidth;
    _screenHeight = screenHeight;
    _screenDepth  = screenDepth;
    _screenNear   = screenNear;

    queryDisplayFrequency();
    readyDirectX();
#ifdef _DEBUG
    char textBuffer[100];
    printf("Direct::init();\n");
    sprintf_s(textBuffer, sizeof(textBuffer), "\t글픽카드 이  름 : %s\n\t글픽카드 메모리 : %d MB\n", _videoCardDesc, _videoCardMemory);
    printf(textBuffer);
#endif
}
void Direct::release() 
{
    if (_swapchain)
        _swapchain->SetFullscreenState(false, NULL);
    if (_rasterizerState)
    {
        _rasterizerState->Release();
        _rasterizerState = NULL;
    }
    if (_depthStencilView)
    {
        _depthStencilView->Release();
        _depthStencilView = NULL;
    }
    if (_depthStencilState)
    {
        _depthStencilState->Release();
        _depthStencilState = NULL;
    }
    if (_depthStencilBuffer)
    {
        _depthStencilBuffer->Release();
        _depthStencilBuffer = NULL;
    }
    if (_renderTargetView)
    {
        _renderTargetView->Release();
        _renderTargetView = NULL;
    }
    if (_deviceContext)
    {
        _deviceContext->Release();
        _deviceContext = NULL;
    }
    if (_device)
    {
        _device->Release();
        _device = NULL;
    }
    if (_swapchain)
    {
        _swapchain->Release();
        _swapchain = NULL;
    }
}
void Direct::clearBuffer(float r, float g, float b, float a)
{
    // 뎁스버퍼 클리어
    _deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);

    // 백버퍼 클리어 - 색상지정
    float clearColor[4] = { r, g, b, a };
    _deviceContext->ClearRenderTargetView(_renderTargetView, clearColor);

}
void Direct::swapchain()
{
    if (_isVsync)
        // vsync맞춰 출력
        _swapchain->Present(1, 0);
    else
        // 되는대로 출력
        _swapchain->Present(0, 0);
}
ID3D11Device* Direct::getDevice()
{
    return _device;
}
ID3D11DeviceContext* Direct::getDeviceContext()
{
    return _deviceContext;
}
D3DXMATRIX Direct::getMtxProjection()
{
    return _mtxProjection;
}
D3DXMATRIX Direct::getMtxWorld()
{
    return _mtxWorld;
}
D3DXMATRIX Direct::getMtxOrtho()
{
    return _mtxOrtho;
}
void Direct::getVideoCardInfo(char* videoCardName, int& memory)
{
    strcpy_s(videoCardName, 128, _videoCardDesc);
    memory = _videoCardMemory;
}
// 팩토리, 어댑터 객체 생성 - 디스플레이 인포 체크
// Direct3D 초기화 이전, 글픽카드&디스플레이의 주파수 체크하기 위한 함수.
// 주파수 분자&분모 값을 체크한 뒤 DirctX에게 알려주어 적절한 새로고침 비율을 계산하도록 한다.
// 이 작업없이 기본값을 DirectX에게 넘겨주면 buffer flip대신 blit를 사용하게되어 앱 성능을 떨어뜨린다.
void Direct::queryDisplayFrequency()
{
    HRESULT           result;
    IDXGIFactory*     factory;      // 
    IDXGIAdapter*     adapter;      // 그래픽카드
    IDXGIOutput*      adapterOutput;
    unsigned int      modeNumber;

    DXGI_MODE_DESC*   dxgiModeDesc;
    DXGI_ADAPTER_DESC dxgiAdapterDesc;
    int               error;
    size_t            stringLength;

    // 팩토리 생성
    result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
    if (FAILED(result))
        MessageBox(NULL, L"CreateDXGIFactory", L"Error", NULL);

    // 팩토리를 사용해 어댑터 생성
    result = factory->EnumAdapters(0, &adapter);
    if (FAILED(result)) MessageBox(NULL, L"IDXGIFactory::EnumAdaters()", L"Error", NULL);

    // 디스플레이(모니터)의 첫 번째 어댑터 나열
    result = adapter->EnumOutputs(0, &adapterOutput);
    if (FAILED(result)) MessageBox(NULL, L"IDXGIAdapter::EnumOutputs()", L"Error", NULL);

    // 디스크립션(인자4)가 NULL일 때,
    // 포맷(인자1)에 맞는 디스플레이(모니터)의 모드 개수를 인자3에 저장.
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modeNumber, NULL);
    if (FAILED(result)) MessageBox(NULL, L"IDXGIAdapterOutput::GetDisplayModeList", L"Error", NULL);

    // 가능한 모드 수만큼 모드정보를 저장할 디스크립션 리스트 생성.
    dxgiModeDesc = new DXGI_MODE_DESC[modeNumber];
    if (!dxgiModeDesc) MessageBox(NULL, L"new DXGI_MODE_DESC[..]", L"Error", NULL);

    // 디스크립션(인자4)가 NULL이 아닐 때,
    // 가능한 모드 디스크립션들(인자4)을 작성.
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modeNumber, dxgiModeDesc);
    if (FAILED(result)) MessageBox(NULL, L"DXGIAdapterOutput::GetDisplayModeList()", L"Error", NULL);
    for (int i = 0; i < modeNumber; i++)
    {
        // 각 모드별 디스플레이(모니터) 주사율에 대한 분모&분자를 디스크립션에 작성.
        if (dxgiModeDesc[i].Width == (unsigned int)_screenWidth &&
            dxgiModeDesc[i].Height == (unsigned int)_screenHeight)
        {
            _frequencyNumerator = dxgiModeDesc[i].RefreshRate.Numerator;    // 분자
            _frequencyDenominator = dxgiModeDesc[i].RefreshRate.Denominator;// 분모
        }
    }

    // 어댑터(그래픽카드)의 디스크립션 Get
    result = adapter->GetDesc(&dxgiAdapterDesc);
    if (FAILED(result)) MessageBox(NULL, L"IDXGIAdapter::GetDesc()", L"Error", NULL);

    // 현재 그래픽카드의 메모리용량을 MB단위로 저장
    _videoCardMemory = (int)(dxgiAdapterDesc.DedicatedVideoMemory / 1024 / 1024);

    // char str[100];sprintf_s(str, "비디오카드 메모리 : %d", _videoCardMemory);printf(str);
    // wcstombs_s()
    // 역활  : 인자4(유니코드 와이드 문자열)를 인자2(멀티바이트 버퍼)로 복사해주는 친구.
    // 인자1 : 함수 호출 후 변환된 문자 수
    // 인자2 : 데스티네이션 스트링
    // 인자3 : 데스티네이션 스트링 버퍼 사이즈
    // 인자4 : 복사할 소스 스트링
    // 인자5 : 복사할 소스 스트링 버퍼에서 얼마나 복사할 것인가?

    error = wcstombs_s(&stringLength, _videoCardDesc, 128, dxgiAdapterDesc.Description, 128);
    if (error) MessageBox(NULL, L"wcstombs_s()", L"Error", NULL);

    // 디스플레이(모니터) 주사율에 대한 분모&분자 및
    // 그래픽카드에 대한 정보(DXGI_ADAPTER_DESC)를 확보했기 때문에
    // 이들을 얻기 위해 사용했던 자원들 반납.
    delete[] dxgiModeDesc;
    dxgiModeDesc = NULL;

    adapterOutput->Release();
    adapterOutput = NULL;

    adapter->Release();
    adapter = NULL;

    factory->Release();
    factory = NULL;
}
// 디바이스, 스왑체인 생성
// 렌더타겟 뷰, 뎁스스텐실 뷰 생성 후 바인딩
// 레스터라이저 상태 생성 후 뷰포트 설정
void Direct::readyDirectX()
{
    HRESULT                       result;
    D3D_FEATURE_LEVEL             featureLevel;
    DXGI_SWAP_CHAIN_DESC          swapchainDesc;
    ID3D11Texture2D*              backBuffer;
    D3D11_TEXTURE2D_DESC          depthBufferDesc;
    D3D11_DEPTH_STENCIL_DESC      depthStencilDesc;
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    D3D11_RASTERIZER_DESC         rasterizerDesc;
    D3D11_VIEWPORT                viewport;
    float                         fieldOfView, screenAspect;

    //▼ 디바이스, 스왑체인 생성
    // 피쳐레벨 작성
    featureLevel = D3D_FEATURE_LEVEL_11_0;
    // 스왑체인 디스크립션 작성
    ZeroMemory(&swapchainDesc, sizeof(swapchainDesc));
    if (_isFullscreen) { swapchainDesc.Windowed = false; }
    else { swapchainDesc.Windowed = true; }
    if (_isVsync)
    {
        swapchainDesc.BufferDesc.RefreshRate.Numerator = _frequencyNumerator;
        swapchainDesc.BufferDesc.RefreshRate.Denominator = _frequencyDenominator;
    }
    else
    {
        swapchainDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
    }
    swapchainDesc.BufferCount        = 1;                              // 백버퍼는 하나만 사용하겠슴다
    swapchainDesc.BufferDesc.Width   = _screenWidth;                   // 백버퍼 사이즈
    swapchainDesc.BufferDesc.Height  = _screenHeight;                  // 백버퍼 사이즈
    swapchainDesc.BufferDesc.Format  = DXGI_FORMAT_R8G8B8A8_UNORM;     // 
    swapchainDesc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;// 백버퍼 용도
    swapchainDesc.OutputWindow       = _hwnd;                          // ★ 체고조넘 킹 더 갇 엠페러 중요 임폴턴트 필살 안드로메다 이불킥
    swapchainDesc.SampleDesc.Count   = 1;                              // 멀티샘플링 OFF 1
    swapchainDesc.SampleDesc.Quality = 0;                              // 멀티샘플링 OFF 2
    swapchainDesc.Flags              = 0;                              // 옵션 플래그 지정X
    swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;// 스캔라인 정렬 미지정.
    swapchainDesc.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;       // 스케일링 미지정
    swapchainDesc.SwapEffect                  = DXGI_SWAP_EFFECT_DISCARD;            // 스왑 후 백버퍼는 딱히 별 효과 안주고 버림
    // 디바이스, 스왑체인 생성
    result = D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE, // DX11지원 그래픽카드는 인자2에 D3D_DRIVER_TYPE_HARDWARE, 아니라면 D3D_DRIVER_TYPE_REFERENCE
        NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION,
        &swapchainDesc, &_swapchain,
        &_device, NULL, &_deviceContext);
    if (FAILED(result)) MessageBox(NULL, L"D3D11CreateDeviceAndSwapChain()", L"Error", NULL);

    //▼렌더타겟 뷰 생성
    // 스왑체인 객체의 버퍼를 받고, 디바이스에게 넘겨 렌더타겟 뷰(백버퍼) 생성
    // GetBuffer()호출로 백버퍼에 대한 COM참조횟수가 증가했으므로 릴리즈
    result = _swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    if (FAILED(result)) MessageBox(NULL, L"IDXGISwapChain::GetBuffer()", L"Error", NULL);
    result = _device->CreateRenderTargetView(backBuffer, NULL, &_renderTargetView);
    if (FAILED(result)) MessageBox(NULL, L"ID3D11Device::CreateRenderTargetView()", L"Error", NULL);
    backBuffer->Release();
    backBuffer = NULL;

    //▼뎁스스텐실 뷰 생성
    // 뎁스버퍼 디스크립션 작성
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
    depthBufferDesc.Width              = _screenWidth;
    depthBufferDesc.Height             = _screenHeight;
    depthBufferDesc.MipLevels          = 1;
    depthBufferDesc.ArraySize          = 1;
    depthBufferDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count   = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage              = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags     = 0;
    depthBufferDesc.MiscFlags          = 0;
    // 뎁스버퍼 텍스쳐 생성 
    result = _device->CreateTexture2D(&depthBufferDesc, NULL, &_depthStencilBuffer);
    if (FAILED(result)) MessageBox(NULL, L"ID3D11Device::CreateTexture2D()", L"Error", NULL);
    // 뎁스스텐실 디스크립션 작성
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
    depthStencilDesc.DepthEnable      = true;
    depthStencilDesc.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc        = D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable    = true;
    depthStencilDesc.StencilReadMask  = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;
    depthStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;  // 1 
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;  // 1
    depthStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;  // 1
    depthStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;// 1
    depthStencilDesc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;  // 2
    depthStencilDesc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_DECR;  // 2
    depthStencilDesc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;  // 2
    depthStencilDesc.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;// 2
                                                                    // 뎁스스텐실 상태 생성
    result = _device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
    if (FAILED(result)) MessageBox(NULL, L"ID3D11Device::CreateDepthStencilState()", L"Error", NULL);
    // 뎁스스텐실 상태 생성 후 dc로 적용시킨다
    _deviceContext->OMSetDepthStencilState(_depthStencilState, 1);
    // 뎁스스텐실 뷰 디스크립션 작성
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
    depthStencilViewDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;
    // 뎁스스텐실 뷰 생성
    result = _device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
    if (FAILED(result)) MessageBox(NULL, L"ID3D11Device::CreateDepthStencilView()", L"Error", NULL);

    //▼렌더타겟뷰, 뎁스스텐실뷰 바인딩
    _deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

    //▼레스터라이저 상태 생성
    // 레스터라이저 디스크립션 작성
    rasterizerDesc.AntialiasedLineEnable = false;
    rasterizerDesc.CullMode              = D3D11_CULL_BACK;
    rasterizerDesc.DepthBias             = 0;
    rasterizerDesc.DepthClipEnable       = true;
    rasterizerDesc.FillMode              = D3D11_FILL_SOLID;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.MultisampleEnable     = false;
    rasterizerDesc.ScissorEnable         = false;
    rasterizerDesc.SlopeScaledDepthBias  = 0.0f;
    // 레스터라이저 상태 생성
    result = _device->CreateRasterizerState(&rasterizerDesc, &_rasterizerState);
    if (FAILED(result)) MessageBox(NULL, L"ID3D11Device::CreateRasterizerState()", L"Error", NULL);
    _deviceContext->RSSetState(_rasterizerState);

    //▼ 뷰포트 설정 및 생성
    viewport.Width    = (float)_screenWidth;
    viewport.Height   = (float)_screenHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    _deviceContext->RSSetViewports(1, &viewport);

    //▼ 투영행렬 생성
    fieldOfView  = (float)D3DX_PI / 4.0f;
    screenAspect = (float)_screenWidth / (float)_screenHeight;
    D3DXMatrixPerspectiveFovLH(&_mtxProjection, fieldOfView, screenAspect, _screenNear, _screenDepth);

    //▼ 월드행렬을 단위행렬로 초기화
    D3DXMatrixIdentity(&_mtxWorld);

    //▼ 2D렌더링에 쓸 직교행렬 생성
    D3DXMatrixOrthoLH(&_mtxOrtho, (float)_screenWidth, (float)_screenHeight, _screenNear, _screenDepth);
}