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
    sprintf_s(textBuffer, sizeof(textBuffer), "\t����ī�� ��  �� : %s\n\t����ī�� �޸� : %d MB\n", _videoCardDesc, _videoCardMemory);
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
    // �������� Ŭ����
    _deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);

    // ����� Ŭ���� - ��������
    float clearColor[4] = { r, g, b, a };
    _deviceContext->ClearRenderTargetView(_renderTargetView, clearColor);

}
void Direct::swapchain()
{
    if (_isVsync)
        // vsync���� ���
        _swapchain->Present(1, 0);
    else
        // �Ǵ´�� ���
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
// ���丮, ����� ��ü ���� - ���÷��� ���� üũ
// Direct3D �ʱ�ȭ ����, ����ī��&���÷����� ���ļ� üũ�ϱ� ���� �Լ�.
// ���ļ� ����&�и� ���� üũ�� �� DirctX���� �˷��־� ������ ���ΰ�ħ ������ ����ϵ��� �Ѵ�.
// �� �۾����� �⺻���� DirectX���� �Ѱ��ָ� buffer flip��� blit�� ����ϰԵǾ� �� ������ ����߸���.
void Direct::queryDisplayFrequency()
{
    HRESULT           result;
    IDXGIFactory*     factory;      // 
    IDXGIAdapter*     adapter;      // �׷���ī��
    IDXGIOutput*      adapterOutput;
    unsigned int      modeNumber;

    DXGI_MODE_DESC*   dxgiModeDesc;
    DXGI_ADAPTER_DESC dxgiAdapterDesc;
    int               error;
    size_t            stringLength;

    // ���丮 ����
    result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
    if (FAILED(result))
        MessageBox(NULL, L"CreateDXGIFactory", L"Error", NULL);

    // ���丮�� ����� ����� ����
    result = factory->EnumAdapters(0, &adapter);
    if (FAILED(result)) MessageBox(NULL, L"IDXGIFactory::EnumAdaters()", L"Error", NULL);

    // ���÷���(�����)�� ù ��° ����� ����
    result = adapter->EnumOutputs(0, &adapterOutput);
    if (FAILED(result)) MessageBox(NULL, L"IDXGIAdapter::EnumOutputs()", L"Error", NULL);

    // ��ũ����(����4)�� NULL�� ��,
    // ����(����1)�� �´� ���÷���(�����)�� ��� ������ ����3�� ����.
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modeNumber, NULL);
    if (FAILED(result)) MessageBox(NULL, L"IDXGIAdapterOutput::GetDisplayModeList", L"Error", NULL);

    // ������ ��� ����ŭ ��������� ������ ��ũ���� ����Ʈ ����.
    dxgiModeDesc = new DXGI_MODE_DESC[modeNumber];
    if (!dxgiModeDesc) MessageBox(NULL, L"new DXGI_MODE_DESC[..]", L"Error", NULL);

    // ��ũ����(����4)�� NULL�� �ƴ� ��,
    // ������ ��� ��ũ���ǵ�(����4)�� �ۼ�.
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modeNumber, dxgiModeDesc);
    if (FAILED(result)) MessageBox(NULL, L"DXGIAdapterOutput::GetDisplayModeList()", L"Error", NULL);
    for (int i = 0; i < modeNumber; i++)
    {
        // �� ��庰 ���÷���(�����) �ֻ����� ���� �и�&���ڸ� ��ũ���ǿ� �ۼ�.
        if (dxgiModeDesc[i].Width == (unsigned int)_screenWidth &&
            dxgiModeDesc[i].Height == (unsigned int)_screenHeight)
        {
            _frequencyNumerator = dxgiModeDesc[i].RefreshRate.Numerator;    // ����
            _frequencyDenominator = dxgiModeDesc[i].RefreshRate.Denominator;// �и�
        }
    }

    // �����(�׷���ī��)�� ��ũ���� Get
    result = adapter->GetDesc(&dxgiAdapterDesc);
    if (FAILED(result)) MessageBox(NULL, L"IDXGIAdapter::GetDesc()", L"Error", NULL);

    // ���� �׷���ī���� �޸𸮿뷮�� MB������ ����
    _videoCardMemory = (int)(dxgiAdapterDesc.DedicatedVideoMemory / 1024 / 1024);

    // char str[100];sprintf_s(str, "����ī�� �޸� : %d", _videoCardMemory);printf(str);
    // wcstombs_s()
    // ��Ȱ  : ����4(�����ڵ� ���̵� ���ڿ�)�� ����2(��Ƽ����Ʈ ����)�� �������ִ� ģ��.
    // ����1 : �Լ� ȣ�� �� ��ȯ�� ���� ��
    // ����2 : ����Ƽ���̼� ��Ʈ��
    // ����3 : ����Ƽ���̼� ��Ʈ�� ���� ������
    // ����4 : ������ �ҽ� ��Ʈ��
    // ����5 : ������ �ҽ� ��Ʈ�� ���ۿ��� �󸶳� ������ ���ΰ�?

    error = wcstombs_s(&stringLength, _videoCardDesc, 128, dxgiAdapterDesc.Description, 128);
    if (error) MessageBox(NULL, L"wcstombs_s()", L"Error", NULL);

    // ���÷���(�����) �ֻ����� ���� �и�&���� ��
    // �׷���ī�忡 ���� ����(DXGI_ADAPTER_DESC)�� Ȯ���߱� ������
    // �̵��� ��� ���� ����ߴ� �ڿ��� �ݳ�.
    delete[] dxgiModeDesc;
    dxgiModeDesc = NULL;

    adapterOutput->Release();
    adapterOutput = NULL;

    adapter->Release();
    adapter = NULL;

    factory->Release();
    factory = NULL;
}
// ����̽�, ����ü�� ����
// ����Ÿ�� ��, �������ٽ� �� ���� �� ���ε�
// �����Ͷ����� ���� ���� �� ����Ʈ ����
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

    //�� ����̽�, ����ü�� ����
    // ���ķ��� �ۼ�
    featureLevel = D3D_FEATURE_LEVEL_11_0;
    // ����ü�� ��ũ���� �ۼ�
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
    swapchainDesc.BufferCount        = 1;                              // ����۴� �ϳ��� ����ϰڽ���
    swapchainDesc.BufferDesc.Width   = _screenWidth;                   // ����� ������
    swapchainDesc.BufferDesc.Height  = _screenHeight;                  // ����� ������
    swapchainDesc.BufferDesc.Format  = DXGI_FORMAT_R8G8B8A8_UNORM;     // 
    swapchainDesc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;// ����� �뵵
    swapchainDesc.OutputWindow       = _hwnd;                          // �� ü������ ŷ �� �� ���䷯ �߿� ������Ʈ �ʻ� �ȵ�θ޴� �̺�ű
    swapchainDesc.SampleDesc.Count   = 1;                              // ��Ƽ���ø� OFF 1
    swapchainDesc.SampleDesc.Quality = 0;                              // ��Ƽ���ø� OFF 2
    swapchainDesc.Flags              = 0;                              // �ɼ� �÷��� ����X
    swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;// ��ĵ���� ���� ������.
    swapchainDesc.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;       // �����ϸ� ������
    swapchainDesc.SwapEffect                  = DXGI_SWAP_EFFECT_DISCARD;            // ���� �� ����۴� ���� �� ȿ�� ���ְ� ����
    // ����̽�, ����ü�� ����
    result = D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE, // DX11���� �׷���ī��� ����2�� D3D_DRIVER_TYPE_HARDWARE, �ƴ϶�� D3D_DRIVER_TYPE_REFERENCE
        NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION,
        &swapchainDesc, &_swapchain,
        &_device, NULL, &_deviceContext);
    if (FAILED(result)) MessageBox(NULL, L"D3D11CreateDeviceAndSwapChain()", L"Error", NULL);

    //�巻��Ÿ�� �� ����
    // ����ü�� ��ü�� ���۸� �ް�, ����̽����� �Ѱ� ����Ÿ�� ��(�����) ����
    // GetBuffer()ȣ��� ����ۿ� ���� COM����Ƚ���� ���������Ƿ� ������
    result = _swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    if (FAILED(result)) MessageBox(NULL, L"IDXGISwapChain::GetBuffer()", L"Error", NULL);
    result = _device->CreateRenderTargetView(backBuffer, NULL, &_renderTargetView);
    if (FAILED(result)) MessageBox(NULL, L"ID3D11Device::CreateRenderTargetView()", L"Error", NULL);
    backBuffer->Release();
    backBuffer = NULL;

    //�嵪�����ٽ� �� ����
    // �������� ��ũ���� �ۼ�
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
    // �������� �ؽ��� ���� 
    result = _device->CreateTexture2D(&depthBufferDesc, NULL, &_depthStencilBuffer);
    if (FAILED(result)) MessageBox(NULL, L"ID3D11Device::CreateTexture2D()", L"Error", NULL);
    // �������ٽ� ��ũ���� �ۼ�
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
                                                                    // �������ٽ� ���� ����
    result = _device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
    if (FAILED(result)) MessageBox(NULL, L"ID3D11Device::CreateDepthStencilState()", L"Error", NULL);
    // �������ٽ� ���� ���� �� dc�� �����Ų��
    _deviceContext->OMSetDepthStencilState(_depthStencilState, 1);
    // �������ٽ� �� ��ũ���� �ۼ�
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
    depthStencilViewDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;
    // �������ٽ� �� ����
    result = _device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
    if (FAILED(result)) MessageBox(NULL, L"ID3D11Device::CreateDepthStencilView()", L"Error", NULL);

    //�巻��Ÿ�ٺ�, �������ٽǺ� ���ε�
    _deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

    //�巹���Ͷ����� ���� ����
    // �����Ͷ����� ��ũ���� �ۼ�
    rasterizerDesc.AntialiasedLineEnable = false;
    rasterizerDesc.CullMode              = D3D11_CULL_BACK;
    rasterizerDesc.DepthBias             = 0;
    rasterizerDesc.DepthClipEnable       = true;
    rasterizerDesc.FillMode              = D3D11_FILL_SOLID;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.MultisampleEnable     = false;
    rasterizerDesc.ScissorEnable         = false;
    rasterizerDesc.SlopeScaledDepthBias  = 0.0f;
    // �����Ͷ����� ���� ����
    result = _device->CreateRasterizerState(&rasterizerDesc, &_rasterizerState);
    if (FAILED(result)) MessageBox(NULL, L"ID3D11Device::CreateRasterizerState()", L"Error", NULL);
    _deviceContext->RSSetState(_rasterizerState);

    //�� ����Ʈ ���� �� ����
    viewport.Width    = (float)_screenWidth;
    viewport.Height   = (float)_screenHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    _deviceContext->RSSetViewports(1, &viewport);

    //�� ������� ����
    fieldOfView  = (float)D3DX_PI / 4.0f;
    screenAspect = (float)_screenWidth / (float)_screenHeight;
    D3DXMatrixPerspectiveFovLH(&_mtxProjection, fieldOfView, screenAspect, _screenNear, _screenDepth);

    //�� ��������� ������ķ� �ʱ�ȭ
    D3DXMatrixIdentity(&_mtxWorld);

    //�� 2D�������� �� ������� ����
    D3DXMatrixOrthoLH(&_mtxOrtho, (float)_screenWidth, (float)_screenHeight, _screenNear, _screenDepth);
}