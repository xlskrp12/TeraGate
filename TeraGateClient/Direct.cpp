#include"Direct.h"
Direct::Direct()
{
    _swapchain          = 0;
    _device             = 0;
    _deviceContext      = 0;
    _renderTargetView   = 0;
    _depthStencilBuffer = 0;
    _depthStencilState  = 0;
    _depthStencilView   = 0;
    _rasterState        = 0;
}
Direct::Direct(const Direct& direct)
{

}
Direct::~Direct()
{

}
/**********************************************************************************
# 개요
전체 Direct3D 설정이 일어나는 곳

# 인자
- screenWidth, screenHeight
이미 생성된 윈도우의 너비와 높이이다.
Direct3D에서는 이를 이용해 동일한 크기의 영역을 초기화하고 이용한다.

- vsync
Direct3D렌더링을 모니터의 새로고침 비율에 맞출 것인지
아니면 가능한 빠르게 다시 그릴 것인지 설정

- window
Direct3D에서 이 변수를 이용해 생성된 윈도우에 접근

- fullscreen
어플리케이션이 윈도우 모드, 풀스크린 어떤 모드에서 돌아가야할지 정보

- screenDepth, screenNear
윈도우에 그려질 3D환경에서의 깊이 값.
**********************************************************************************/
bool Direct::init(
    int screenWidth, int screenHeight,
    bool vsync, HWND window, bool fullScreen,
    float screenDepth, float screenNear)
{
    HRESULT                       result;
    IDXGIFactory*                 factory;
    IDXGIAdapter*                 adapter;
    IDXGIOutput*                  adapterOutput;
    unsigned int                  modeNumber;
    unsigned int                  i;
    unsigned int                  numerator;      // 모니터 주파수 분자
    unsigned int                  denominator;    // 모니터 주파수 분모
    unsigned int                  stringLength;
    DXGI_MODE_DESC*               displayModeList;
    DXGI_ADAPTER_DESC             adapterDesc;
    int                           error;
    DXGI_SWAP_CHAIN_DESC          swapchainDesc;
    D3D_FEATURE_LEVEL             featureLevel;
    ID3D11Texture2D*              backBufferPtr;
    D3D11_TEXTURE2D_DESC          depthBufferDesc;
    D3D11_DEPTH_STENCIL_DESC      depthStencilDesc;
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    D3D11_RASTERIZER_DESC         rasterDesc;
    D3D11_VIEWPORT                viewPort;
    float                         fieldOfView, screenAspect;

    /**********************************************************************************
    # 하드웨어 조사
    Direct3D를 초기화하기 전에 하드웨어(그래픽카드, 모니터 주파수)에 대해 알아야 한다.
    만약 이 작업을 하지 않고 대중 지원되는 기본값으로 맞춰놓는다면,
    DirectX는 buffer flip대신 blit를 사용해 화면에 표시하게 되어 어플리케이션의 성능을 떨어뜨린다.
    **********************************************************************************/
    // 수직동기화 vsync 설정
    _vsyncEnabled = vsync; 

    // DirectX 그래픽 인터페이스 팩토리를 만든다.
    result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
    if (FAILED(result))
        return false;

    // 팩토리 객체를 사용해 첫번째 그래픽 카드 인터페이스에 대한 어댑터 생성
    result = factory->EnumAdapters(0, &adapter);
    if (FAILED(result))
        return false;

    // 출력(모니터)에 대한 첫번째 어댑터를 나열
    result = adapter->EnumOutputs(0, &adapterOutput);
    if (FAILED(result))
        return false;

    // DXGI_FORMAT_R8G8B8A8_UNORM 모니터 출력 디스플레이 포맷에 맞는 모니터 출력 개수를 구한다.
    result = adapterOutput->GetDisplayModeList(
        DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modeNumber, NULL);

    // 가능한 모든 모니터와 그래픽카드 조합을 저장할 리스트 생성
    displayModeList = new DXGI_MODE_DESC[modeNumber];
    if (!displayModeList)
        return false;

    // 디스플레이 모드에 대한 리스트 구조를 채워넣는다.
    result = adapterOutput->GetDisplayModeList(
        DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modeNumber, displayModeList);
    if (FAILED(result))
        return false;

    // 모든 디스플레이 모드에 대해 화면 너비/높이에 맞는 디스플레이 모드를 찾는다.
    // 적합한 것을 찾으면 모니터의 새로고침 비율의 분모와 분자 값을 저장한다.
    for (i = 0; i < modeNumber; i++)
        if (displayModeList[i].Width  == (unsigned int)screenWidth &&
            displayModeList[i].Height == (unsigned int)screenHeight)
            {
                numerator = displayModeList[i].RefreshRate.Numerator;
                denominator = displayModeList[i].RefreshRate.Denominator;
            }

    // 어댑터의 description을 가져온다.
    result = adapter->GetDesc(&adapterDesc);
    if (FAILED(result))
        return false;

    // 현재 그래픽카드의 메모리 용량을 메가바이트 단위로 저장한다.
    _videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

    // 그래픽카드의 이름을 char형 문자열 배열로 바꾼 뒤 저장한다.
    error = wcstombs_s(&stringLength, _videoCardDescription, 128, adapterDesc.Description, 128);
    if (error != 0)
        return false;

    // 디스플레이 모드 리스트의 할당을 해제한다.
    delete[] displayModeList;
    displayModeList = 0;

    // 어댑터 출력을 할당해제한다.
    adapterOutput->Release();
    adapterOutput = 0;

    // 어댑터를 할당해제한다.
    adapter->Release();
    adapter = 0;

    // 팩토리 객체를 할당 해제한다.
    factory->Release();
    factory = 0;

    /**********************************************************************************
    # 스왑체인 description 구조체 초기화 - 백버퍼 개수, 사이즈, 포맷
    시스템에서 얻어온 주파수를 가지고 있으므로 DirectX를 제대로 초기화를 할 수 있다.
    이제 스왑체인을 초기화할거다.
    스왑체인은 실제 렌더링을 할 때 쓰여지는 곳이다.
    스왑체인은 프론트버퍼와 백버퍼로 이루어져있다.
    렌더링마다 프론트버퍼와 백버퍼를 바꿔치기(swap)하기 때문에 스왑체인이라 불린다.
    **********************************************************************************/
    // 스왑체인 description 구조체 초기화
    ZeroMemory(&swapchainDesc, sizeof(swapchainDesc));
    // 하나의 백버퍼만을 사용
    swapchainDesc.BufferCount = 1;
    // 백버퍼의 너비, 높이 설정
    swapchainDesc.BufferDesc.Width  = screenWidth;
    swapchainDesc.BufferDesc.Height = screenHeight;
    // 백버퍼로 일반적인 32bit의 서페이스를 지정
    swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    /**********************************************************************************
    # 스왑체인 description 구조체 초기화 - 백버퍼 주파수(새로고침 비율)
    주파수(새로고침 비율)는 초당 몇 장의 백버퍼를 그려 프론트 버퍼와 바꿔치기하는지를 나타내는 수치.
    만약 GrapihcManager의 vsync변수가 true로 설정되있다면 시스템에서 정한 새로고침 비율로 고정(ex:60hz)
    반면 vsync 변수가 false라면 프로그램은 최대한 많이 화면을 그려내려고 하지만, 약간의 잔상을 남긴다.
    **********************************************************************************/
    // 백버퍼의 새로고침 비율을 설정
    if (_vsyncEnabled)
    {
        swapchainDesc.BufferDesc.RefreshRate.Numerator   = numerator;
        swapchainDesc.BufferDesc.RefreshRate.Denominator = denominator;
    }
    else
    {
        swapchainDesc.BufferDesc.RefreshRate.Numerator   = 0;
        swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
    }

    /**********************************************************************************
    # 스왑체인 description 구조체 초기화 - 기타등등
    **********************************************************************************/
    // 백버퍼의 용도 설정
    swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    // 렌더링이 이루어질 윈도우 핸들 설정
    swapchainDesc.OutputWindow = window;

    // 멀티샘플링을 끈다.
    swapchainDesc.SampleDesc.Count   = 1;
    swapchainDesc.SampleDesc.Quality = 0;

    // 윈도우 모드 또는 풀스크린 모드를 설정
    if (fullScreen)
        swapchainDesc.Windowed = false;
    else
        swapchainDesc.Windowed = true;
    
    // 스캔라인의 정렬과 스캔라이닝을 지정되지 않음(unspecified) 설정
    swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapchainDesc.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;

    // 출력된 이후 백버퍼의 내용을 버리도록 설정
    swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    // 추가 옵션 플래그를 사용하지 않는다.
    swapchainDesc.Flags = 0;

    /**********************************************************************************
    # 피쳐레벨 설정
    피처레벨은 어느 버전의 DirectX를 사용할지 구분해준다.
    **********************************************************************************/
    // DirectX11을 의미하는 11.0으로 설정
    featureLevel = D3D_FEATURE_LEVEL_11_0;

    /**********************************************************************************
    # 디바이스, 스왑체인 만들기
    하드웨어 조사 후 스왑체인 description, 피처레벨 설정을 마쳤다.
    이제 스왑체인, Direct3D 디바이스, Direct3D 디바이스 컨텍스트를 만들 수 있다.

    Direct3D 디바이스, 디바이스 컨텍스트는 모든 Direct3D기능의 인터페이스가 가능하기 때문에 매우 중요하다.
    Direct3D 디바이스, 디바이스 컨텍스트는 이후 대부분의 작업을 수행할 것이다.

    이전 버전의 DirectX에 익숙한 사람이면 Direct3D 디바이스는 친숙하나 디바이스 컨텍스트는 생소할 수 있다.
    이 둘은 기존 Direct3D 디바이스의 기능을 둘로 쪼겠다고 생각하면 된다.

    만약 유저가 DirectX11을 지원하는 그래픽카드를 가지고 있지 않다면,
    디바이스, 디바이스 컨텍스트를 생성하는 함수는 실패할 것이다.

    따라서 DirectX11의 기능을 테스트해보고 싶지만 그래픽카드가 지원하지 않는 경우에는,
    D3D_DRIVER_TYPE_HARDWARE를 D3D_DRIVER_TYPE_REFERENCE로 바꾸어서 렌더링을 CPU에서 처리하게 할 수 있다.
    속도는 1/1000 정도로 느리지만 DirectX11을 지원하지 않는 그래픽카드를 가진 사람들에게는 좋은 기능이다.
    **********************************************************************************/
    // 스왑체인, Direct3D 디바이스, 디바이스 컨텍스트를 생성
    // 간혹 첫 번째 그래픽 카드가 DirectX11과 호한되지 않아 실패하는 경우가 있다.
    // 첫 번째 그래픽카드가 DirectX 10까지만 지원하고 두 번째 그래픽카드에서 DirectX11을 지원하는 기기가 있을 수 있다.
    // 또한 일부 하이브리드 그래픽카드의 경우 첫 번째 그래픽카드는 저전력의 인텔, 두 번째 카드가 Nvidia로 동작할 수 있다.
    // 이런 경우에 모두 대응하기 위해 절대 기본 그래픽카드로 생성하지 않고 모든 비디오 카드들을 대조하여 가장 잘 맞는 그래픽 카드로 생성해야 한다.
    result = D3D11CreateDeviceAndSwapChain(
        NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
        D3D11_SDK_VERSION, &swapchainDesc, &_swapchain, &_device, NULL, &_deviceContext);
    if (FAILED(result))
        return false;

    /**********************************************************************************
    # 백버퍼 생성 및 스왑체인과 연결
    **********************************************************************************/
    // 백버퍼 포인터를 가져온다.
    result = _swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
    if (FAILED(result))
        return false;
    
    // 백버퍼 포인터로 렌더타겟 뷰를 생성
    result = _device->CreateRenderTargetView(backBufferPtr, NULL, &_renderTargetView);
    if (FAILED(result))
        return false;
    
    // 백버퍼 포인터를 더 이상 사용하지 않으므로 할당 해제
    backBufferPtr->Release();
    backBufferPtr = 0;

    /**********************************************************************************
    # 뎁스버퍼 description 구조체 작성
    뎁스버퍼 description으로 깊이버퍼를 만들어야 3D공간에서 폴리곤들이 올바르게 그려진다. 
    스텐실 버퍼도 이 뎁스버퍼에 연결할 것이다.
    스텐실 버퍼는 모션 블러나 볼륨 있는 그림자 등의 효과를 나타낼 때 사용된다.
    **********************************************************************************/
    // 뎁스버퍼 description 초기화
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
    
    // 뎁스버퍼 description 작성
    depthBufferDesc.Width              = screenWidth;
    depthBufferDesc.Height             = screenHeight;
    depthBufferDesc.MipLevels          = 1;
    depthBufferDesc.ArraySize          = 1;
    depthBufferDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count   = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage              = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags     = 0;
    depthBufferDesc.MiscFlags          = 0;
    
    // description을 사용해 뎁스버퍼의 텍스쳐 생성
    result = _device->CreateTexture2D(&depthBufferDesc, NULL, &_depthStencilBuffer);
    if (FAILED(result))
        return false;
    
    // 뎁스-스텐실 상태의 description을 초기화
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    // 뎁스-스텐실 상태의 description을 작성
    depthStencilDesc.DepthEnable      = true;
    depthStencilDesc.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc        = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable    = true;
    depthStencilDesc.StencilReadMask  = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing.
    depthStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing.
    depthStencilDesc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;

    // description을 사용해 뎁스-스텐실 상태를 생성
    result = _device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
    if (FAILED(result))
        return false;

    // 생성한 뎁스-스텐실 상태를 적용한다.
    _deviceContext->OMSetDepthStencilState(_depthStencilState, 1);

    /**********************************************************************************
    # 뎁스-스텐실 버퍼의 뷰 description 작성
    이 작업을 해야 Direct3D에서 뎁스버퍼를 뎁스-스텐실 텍스쳐로 인식한다. 
    이 구조체를 채우고 난 후에 CreateDepthStencilView 함수를 호출해 생성한다.
    **********************************************************************************/
    // 뎁스-스텐실 뷰 description 초기화
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
    
    // 뎁스-스텐실 뷰 description 작성
    depthStencilViewDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // 뎁스-스텐실 뷰 생성
    result = _device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
    if (FAILED(result))
        return false;

    // OMSetRenderTargets() 함수 호출
    // 렌더타겟뷰와 뎁스-스텐실 뷰를 출력 렌더링 파이프라인에 바인딩시킨다.
    // 이렇게 하여 파이프라인을 이용한 렌더링이 수행도리 때 위에서 만들었던
    // 백버퍼에 그려지게 된다. 
    // 백버퍼에 그려진 것을 프론트 버퍼와 바꿔치기하여 유저의 모니터에 출력해주면 된다.
    _deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

    /**********************************************************************************
    # 레스터라이저 상태 
    렌더 타겟이 설정된 뒤 추가적인 기능을 설정할 수 있다.
    레스터라이저 상태는 도형이 어떻게 그려지에 대한 제어를 할 수 있게 해준다.
    이를 이용해 화면을 와이어프레임 모드로 그리거나, 도형의 앞뒷면을 모두 그리도록 할 수도 있다.
    기본적으로 DirectX에서 설정되고 돌아가는 레스터라이저 상태가 있지만
    새로 만들지 않는 이상 이에 대한 제어권이 없다.
    **********************************************************************************/
    // 어떤 도형을 어떻게 그릴 것인지 결정하는 레스터라이저 description 작성
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    // 작성한 description으로 레스터라이저 상태 생성
    result = _device->CreateRasterizerState(&rasterDesc, &_rasterState);
    if (FAILED(result))
        return false;

    // 레스터라이저 상태 설정
    _deviceContext->RSSetState(_rasterState);

    /**********************************************************************************
    # 뷰 포트
    뷰 포트가 있어야 렌더타겟 공간에서 클리핑을 수행할 수 있다.
    **********************************************************************************/
    // 윈도우 크기와 동일하게 설정한다.
    viewPort.Width = (float)screenWidth;
    viewPort.Height = (float)screenHeight;
    viewPort.MinDepth = 0.0f;
    viewPort.MaxDepth = 1.0f;
    viewPort.TopLeftX = 0.0f;
    viewPort.TopLeftY = 0.0f;

    // 뷰포트 생성
    _deviceContext->RSSetViewports(1, &viewPort);

    /**********************************************************************************
    # 투영 행렬 projection matrix
    투영행렬은 3D화면을 앞서 만들었던 2D 뷰포트 공간으로 변환하도록 해준다.
    이 행렬의 복사본을 만들어 셰이더에서 사용할 수 있도록 해야 한다.
    **********************************************************************************/
    // 투영행렬 설정
    fieldOfView = (float)D3DX_PI / 4.0f;
    screenAspect = (float)screenWidth / (float)screenHeight;

    // 3D렌더링을 위한 투영행렬을 생성
    D3DXMatrixPerspectiveFovLH(&_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

    /**********************************************************************************
    # 월드 행렬
    오브젝트들을 3D 세계의 좌표로 변환하는 데 사용된다.
    또한 3차원 상에서의 회전/이동/크기 변환에도 사용된다.
    이 역시 복사본을 만들어 셰이더에서 사용할 수 있게 한다.
    **********************************************************************************/
    // 월드 행렬을 단위 행렬로 초기화
    D3DXMatrixIdentity(&_worldMatrix);

    /**********************************************************************************
    # 뷰 행렬
    뷰 행렬은 현재 장면에서 게이머가 어디를 보고 있는가를 계산하는 데 사용된다.
    흔히 말하는 카메라 뷰 등을 생각하면 된다.
    **********************************************************************************/
    // 뷰 행렬 코드 작성은 나중에 카메라 클래스 튜토리얼 때..

    /**********************************************************************************
    # 정사영 행렬
    3D 객체가 아니라 UI와 같은 2D 요소들을 그리기 위해 사용된다.
    **********************************************************************************/
    // 정사영 행렬 생성
    D3DXMatrixOrthoLH(&_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

    return true;
}

/**********************************************************************************
init()에서 사용했던 포인터들을 해제, 반납한다.
풀스크린 상태에서 스왑체인의 해제가 일어날 때 몇몇 예외가 발생하기 때문에
init()에서 사용했던 포인터를 반납하기 전에 스왑체인을 윈도우 모드로 바꿔줘야 한다.
**********************************************************************************/
void Direct::shut()
{
    // 포인터 반납 전에 윈도우 모드로 바꿔준다.
    if (_swapchain)
        _swapchain->SetFullscreenState(false, NULL);

    // 아래부터는 쭉 해제, 반납
    if (_rasterState)
    {
        _rasterState->Release();
        _rasterState = 0;
    }
    if (_depthStencilView)
    {
        _depthStencilView->Release();
        _depthStencilView = 0;
    }
    if (_depthStencilState)
    {
        _depthStencilState->Release();
        _depthStencilState = 0;
    }
    if (_depthStencilBuffer)
    {
        _depthStencilBuffer->Release();
        _depthStencilBuffer = 0;
    }
    if (_renderTargetView)
    {
        _renderTargetView->Release();
        _renderTargetView = 0;
    }
    if (_deviceContext)
    {
        _deviceContext->Release();
        _deviceContext = 0;
    }
    if (_device)
    {
        _device->Release();
        _device = 0;
    }
    if (_swapchain)
    {
        _swapchain->Release();
        _swapchain = 0;
    }
    return;
}
/**********************************************************************************
beginScene은 새로운 매 프레임마다 3D화면을 그리기 시작할 때 호출된다.
버퍼를 빈 값으로 초기화하고 렌더링이 이루어지도록 준비한다.
**********************************************************************************/
void Direct::beginScene(float red, float green, float blue, float alpha)
{
    // 버퍼를 어떤 색상으로 지울 것인지 설정
    float color[4] = { red, green, blue, alpha };

    // 백버퍼 내용을 지움
    _deviceContext->ClearRenderTargetView(_renderTargetView, color);

    // 깊이 버퍼를 지움
    _deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    return;
}

/**********************************************************************************
endScene은 새로운 매 프레임마다 3D화면을 그린 후 호출된다.
스왑체인에게 백버퍼에 그린 3D화면을 표시하도록 한다.
**********************************************************************************/
void Direct::endScene()
{
    // 렌더링이 완료되었으므로 백버퍼의 내용을 화면에 표시
    if (_vsyncEnabled)
        // 새로고침 비율을 고정
        _swapchain->Present(1, 0);
    else
        // 가능한 빠르게 표시
        _swapchain->Present(0, 0);

    return;
}

/**********************************************************************************
포인터 반환
**********************************************************************************/
ID3D11Device* Direct::getDevice()
{
    return _device;
}

/**********************************************************************************
포인터 반환
**********************************************************************************/
ID3D11DeviceContext* Direct::getDeviceContext()
{
    return _deviceContext;
}

/**********************************************************************************
투영 행렬 반환
**********************************************************************************/
void Direct::getProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
    projectionMatrix = _projectionMatrix;
    return;
}

/**********************************************************************************
월드 행렬 반환
**********************************************************************************/
void Direct::getWorldMatrix(D3DXMATRIX& worldMatrix)
{
    worldMatrix = _worldMatrix;
    return;
}

/**********************************************************************************
정사영 행렬 반환
**********************************************************************************/
void Direct::getOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
    orthoMatrix = _orthoMatrix;
    return;
}

/**********************************************************************************
그래픽카드 이름, 사용가능한 메모리 양 반환
서로 다른 환경에서 디버깅하는 데 도움이 될 수 있다.
**********************************************************************************/
void Direct::getVideoCardInfo(char* cardName, int& memory)
{
    strcpy_s(cardName, 128, _videoCardDescription);
    memory = _videoCardMemory;
    return;
}