#pragma once
#include<D3D11.h>
#include<D3DX10math.h>
#include<D3DX11async.h>
#include<fstream>
using namespace std;
/*****************************************************************************************************
GPU상에 존재하는 3D모델들을 그리는 데 사용하는 HLSL 셰이더를 호출하는 클래스
*****************************************************************************************************/
class ColorShader
{
public:
    ColorShader();
    ColorShader(const ColorShader&);
    ~ColorShader();
public:
    // 셰이더의 초기화와 마무리를 제어
    bool init(ID3D11Device*, HWND);
    void shut();
    bool render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

private:
    bool initShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
    void shutShader();
    void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

    bool setShaderParameter(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
    void renderShader(ID3D11DeviceContext*, int);
private:
    ID3D11VertexShader* _vertexShader;
    ID3D11PixelShader*  _pixelShader;
    ID3D11InputLayout*  _layout;
    ID3D11Buffer*       _matrixBuffer;
private:
    // 정점 셰이더에 사용될cBuffer 구조체의 정의
    // 셰이더가 올바른 렌더링을 하기 위해 이 typedef문의 구조체가 반드시
    // 정점 셰이더에 사용된 구조체와 동일해야 한다.
    struct MatrixBufferType
    {
        D3DXMATRIX worldMatrix;
        D3DXMATRIX viewMatrix;
        D3DXMATRIX projectionMatrix;
    };
};