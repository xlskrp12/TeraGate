#pragma once
#include"Prev.h"
/*▼ ColorShader 클래스
    GPU상에 존재하는 3D모델을 렌더할 때 사용하는 HLSL셰이더를 호출하는 용도
*/
class ColorShader
{
public:
    ColorShader();
    ColorShader(const ColorShader&);
    ~ColorShader();
    void init(ID3D11Device*, HWND);
    void release();
    void render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
private:
    // 메소드
    void initShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
    void releaseShader();
    void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
    void setShaderParameter(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
    void renderShader(ID3D11DeviceContext*, int);
    // 자료형 정의
    // Color.vs에 선언할 CBuffer구조체와 형태가 동일해야 한다.
    struct CBuffer
    {
        D3DXMATRIX mtxWorld;
        D3DXMATRIX mtxView;
        D3DXMATRIX mtxProjection;
    };
    // 멤버변수
    ID3D11VertexShader* _vertexShader;
    ID3D11PixelShader*  _pixelShader;
    ID3D11InputLayout*  _inputLayout;
    ID3D11Buffer*       _cBuffer; // 상수버퍼 포인터
};

