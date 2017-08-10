#pragma once
#include"Prev.h"
class Direct
{
public:
    Direct();
    void init(HWND hwnd, bool fullscreen, bool vsync, int screenWidth, int screenHeight, float screenDepth, float screenNear);
    void release();
    void clearBuffer(float r, float g, float b, float a);// �������� Ŭ���� �� ������������ ����� Ŭ����
    void swapchain();                                    // ����ü�� ������Ʈ - �Ƹ����� ��ť
    //�� GetSet
    ID3D11Device*        getDevice();
    ID3D11DeviceContext* getDeviceContext();
    D3DXMATRIX           getMtxProjection();
    D3DXMATRIX           getMtxWorld();
    D3DXMATRIX           getMtxOrtho();
    void                 getVideoCardInfo(char* videoCardName, int& memory);
private:
    void queryDisplayFrequency();// init() ���ο��� ȣ��1
    void readyDirectX();         // init() ���ο��� ȣ��2
private:
    //��  init()���ڷ� �޴� ����
    HWND         _hwnd;
    bool         _isFullscreen;
    bool         _isVsync;
    int          _screenWidth, _screenHeight;
    float        _screenDepth, _screenNear;
    //��  
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
    D3DXMATRIX               _mtxProjection;// �������
    D3DXMATRIX               _mtxWorld;     // �������
    D3DXMATRIX               _mtxOrtho;     // �������
};