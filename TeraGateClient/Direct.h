#pragma once
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#include<DXGI.h>
#include<D3Dcommon.h>
#include<D3D11.h>
#include<D3DX10math.h>
class Direct
{
public:
    Direct();
    Direct(const Direct&);
    ~Direct();

    bool init(int, int, bool, HWND, bool, float, float);
    void shut();

    void beginScene(float, float, float, float);
    void endScene();

    ID3D11Device*        getDevice();
    ID3D11DeviceContext* getDeviceContext();
    
    void getProjectionMatrix(D3DXMATRIX&);
    void getWorldMatrix(D3DXMATRIX&);
    void getOrthoMatrix(D3DXMATRIX&);

    void getVideoCardInfo(char*, int&);
private:
    bool                     _vsyncEnabled;
    int                      _videoCardMemory;
    char                     _videoCardDescription[128];

    IDXGISwapChain*          _swapchain;
    ID3D11Device*            _device;
    ID3D11DeviceContext*     _deviceContext;
    ID3D11RenderTargetView*  _renderTargetView;
    ID3D11Texture2D*         _depthStencilBuffer;
    ID3D11DepthStencilState* _depthStencilState;
    ID3D11DepthStencilView*  _depthStencilView;
    ID3D11RasterizerState*   _rasterState;

    D3DXMATRIX               _projectionMatrix;
    D3DXMATRIX               _worldMatrix;
    D3DXMATRIX               _orthoMatrix;
};