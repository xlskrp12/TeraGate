#pragma once
#include"Prev.h"
/*▼ Shader 클래스
    GPU상에 존재하는 3D모델을 렌더할 때 사용하는 HLSL셰이더를 호출하는 용도
*/
class AmbientLightShader
{
public://▼인터페이스
    AmbientLightShader();
    ~AmbientLightShader();
    void init(ID3D11Device*, HWND);
    void release();
    void render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4);
private://▼메소드
    void initShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
    void releaseShader();
    void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
    void setShaderParameter(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4);
    void renderShader(ID3D11DeviceContext*, int);
private://▼자료형
    struct VSCBuffer
    {// *.vs 상수버퍼와 구조 통일
        D3DXMATRIX mtxWorld;
        D3DXMATRIX mtxView;
        D3DXMATRIX mtxProjection;
    };
    struct PSCBuffer
    {// *.ps 상수버퍼와 구조 통일
        D3DXVECTOR4 ambientColor;
        D3DXVECTOR4 diffuseColor;
        D3DXVECTOR3 lightDirection;
        float       padding;
    };
private://▼변수
    ID3D11VertexShader *_vs;
    ID3D11PixelShader  *_ps;
    ID3D11InputLayout  *_inputLayout;
    ID3D11SamplerState *_samplerState;
    ID3D11Buffer       *_vsCBuffer;
    ID3D11Buffer       *_psCBuffer;
};