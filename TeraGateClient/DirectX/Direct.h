#pragma once
#include"Prev.h"
class Direct
{
public:
    Direct();
    void init(HWND hwnd, bool fullscreen, bool vsync, int screenWidth, int screenHeight, float screenDepth, float screenNear);
    void release();
    void clearBuffer(float r, float g, float b, float a);// 뎁스버퍼 클리어 및 지정색상으로 백버퍼 클리어
    void swapchain();                                    // 스왑체인 프레젠트 - 아리가또 땡큐
    //▼ GetSet
    ID3D11Device*        getDevice();
    ID3D11DeviceContext* getDeviceContext();
    D3DXMATRIX           getMtxProjection();
    D3DXMATRIX           getMtxWorld();
    D3DXMATRIX           getMtxOrtho();
    void                 getVideoCardInfo(char* videoCardName, int& memory);
private:
    void queryDisplayFrequency();// init() 내부에서 호출1
    void readyDirectX();         // init() 내부에서 호출2
private:
    //▼  init()인자로 받는 값들
    HWND         _hwnd;
    bool         _isFullscreen;
    bool         _isVsync;
    int          _screenWidth, _screenHeight;
    float        _screenDepth, _screenNear;
    //▼  
    int          _videoCardMemory;
    char         _videoCardDesc[128];
    unsigned int _frequencyNumerator, _frequencyDenominator;

    IDXGISwapChain*          _swapchain;
    ID3D11Device*            _device;
    ID3D11DeviceContext*     _deviceContext;
    ID3D11RenderTargetView*  _renderTargetView;
    ID3D11Texture2D*         _depthStencilBuffer;
    ID3D11DepthStencilState* _depthStencilState;
    ID3D11DepthStencilView*  _depthStencilView;
    ID3D11RasterizerState*   _rasterizerState;
    D3DXMATRIX               _mtxProjection;// 투영행렬
    D3DXMATRIX               _mtxWorld;     // 월드행렬
    D3DXMATRIX               _mtxOrtho;     // 직교행렬
};