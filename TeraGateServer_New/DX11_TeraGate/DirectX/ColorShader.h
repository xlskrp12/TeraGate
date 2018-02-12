#pragma once
#include"Prev.h"
/*�� ColorShader Ŭ����
    GPU�� �����ϴ� 3D���� ������ �� ����ϴ� HLSL���̴��� ȣ���ϴ� �뵵
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
    // �޼ҵ�
    void initShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
    void releaseShader();
    void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
    void setShaderParameter(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
    void renderShader(ID3D11DeviceContext*, int);
    // �ڷ��� ����
    // Color.vs�� ������ CBuffer����ü�� ���°� �����ؾ� �Ѵ�.
    struct CBuffer
    {
        D3DXMATRIX mtxWorld;
        D3DXMATRIX mtxView;
        D3DXMATRIX mtxProjection;
    };
    // �������
    ID3D11VertexShader* _vertexShader;
    ID3D11PixelShader*  _pixelShader;
    ID3D11InputLayout*  _inputLayout;
    ID3D11Buffer*       _cBuffer; // ������� ������
};

