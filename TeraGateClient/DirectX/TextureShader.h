#pragma once
#include"Prev.h"
class TextureShader
{
public://▼인터페이스
    TextureShader();
    TextureShader(const TextureShader&);
    ~TextureShader();
    void init(ID3D11Device*, HWND);
    void release();
    void render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
private://▼메소드
    void initShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
    void releaseShader();
    void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
    void setShaderParameter(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
    void renderShader(ID3D11DeviceContext*, int);
private://▼자료형
    // *.vs에서 작성한 상수버퍼와 구조가 같아야 된다.
    struct VSCBuffer
    {
        D3DXMATRIX mtxWorld;
        D3DXMATRIX mtxView;
        D3DXMATRIX mtxProjection;
    };
private://▼변수
    ID3D11VertexShader *_vs;
    ID3D11PixelShader  *_ps;
    ID3D11InputLayout  *_inputLayout;
    ID3D11Buffer       *_vsCBuffer;// 상수버퍼 포인터
    ID3D11SamplerState *_samplerState;
};