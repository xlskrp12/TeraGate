#pragma once
#include"Prev.h"
class LightShader
{
public://���������̽�
    LightShader();
    LightShader(const LightShader&);
    ~LightShader();
    void init(ID3D11Device*, HWND);
    void release();
    void render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4);
private://��޼ҵ�
    void initShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
    void releaseShader();
    void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
    void setShaderParameter(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4);
    void renderShader(ID3D11DeviceContext*, int);
private://���ڷ���
    struct VSCBuffer
    {// *.vs ������ۿ� ���� ����
        D3DXMATRIX mtxWorld;
        D3DXMATRIX mtxView;
        D3DXMATRIX mtxProjection;
    };
    struct PSCBuffer
    {// *.ps ������ۿ� ���� ����
        D3DXVECTOR4 diffuseColor;
        D3DXVECTOR3 lightDirection;
        float       padding;
    };
private://�庯��
    ID3D11VertexShader *_vs;
    ID3D11PixelShader  *_ps;
    ID3D11InputLayout  *_inputLayout;
    ID3D11SamplerState *_samplerState;
    ID3D11Buffer       *_vsCBuffer;
    ID3D11Buffer       *_psCBuffer;
};

