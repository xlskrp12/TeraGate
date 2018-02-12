#pragma once
#include"Prev.h"
class TextureShader
{
public://���������̽�
    TextureShader();
    TextureShader(const TextureShader&);
    ~TextureShader();
    void init(ID3D11Device*, HWND);
    void release();
    void render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
private://��޼ҵ�
    void initShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
    void releaseShader();
    void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
    void setShaderParameter(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
    void renderShader(ID3D11DeviceContext*, int);
private://���ڷ���
    // *.vs���� �ۼ��� ������ۿ� ������ ���ƾ� �ȴ�.
    struct VSCBuffer
    {
        D3DXMATRIX mtxWorld;
        D3DXMATRIX mtxView;
        D3DXMATRIX mtxProjection;
    };
private://�庯��
    ID3D11VertexShader *_vs;
    ID3D11PixelShader  *_ps;
    ID3D11InputLayout  *_inputLayout;
    ID3D11Buffer       *_vsCBuffer;// ������� ������
    ID3D11SamplerState *_samplerState;
};