#pragma once
#include"Prev.h"
#include"Direct.h"
#include"Light.h"
#include"FBXManager.h"
#include"TextureManager.h"
/*�� Shader Ŭ����
    GPU�� �����ϴ� 3D���� ������ �� ����ϴ� HLSL���̴��� ȣ���ϴ� �뵵
*/
class AmbientLightShader
{
public://���������̽�
    AmbientLightShader();
    ~AmbientLightShader();
    void init(ID3D11Device*, HWND);
    void release();
    void setLight(Light*);            // ����ü�� �� �� �� �� ���� �ۿ� �ٲ��� �ʴ� ģ�����̶� ���̴� render()���� �� ������ ȣ���ϸ� �ȴ�.
    void setMtxCameraView(D3DXMATRIX);// ����ü�� �� �� �� �� ���� �ۿ� �ٲ��� �ʴ� ģ�����̶� ���̴� render()���� �� ������ ȣ���ϸ� �ȴ�.
    void setMtxProjection();          // ����ü�� �� �� �� �� ���� �ۿ� �ٲ��� �ʴ� ģ�����̶� ���̴� render()���� �� ������ ȣ���ϸ� �ȴ�.
    void AmbientLightShader::target(int fbxID, int textureID);// fbx������ �� ���� ȣ�����ָ� �ȴ�. fbxManager���� ���� ������ ���� �������� ���ļ� ���Ľ����ٰŴ�.
    void render(D3DXMATRIX);          // ����� ��ο� ���� �ؽ��ĸ� �����ؾ� ���� ����.
private://��޼ҵ�
    void initShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
    void releaseShader();
    void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
private://���ڷ���
    struct VSCBuffer
    {// *.vs ������ۿ� ���� ����
        D3DXMATRIX mtxWorld;
        D3DXMATRIX mtxView;
        D3DXMATRIX mtxProjection;
    };
    struct PSCBuffer
    {// *.ps ������ۿ� ���� ����
        D3DXVECTOR4 ambientColor;
        D3DXVECTOR4 diffuseColor;
        D3DXVECTOR3 lightDirection;
        float       padding;
    };
private://�庯��
    // �ʼ��� �ʿ��� ģ���� + �ڽ�Ʈ�� ���̱� ���� ģ����.
    D3DXMATRIX               _mtxCameraView;
    D3DXMATRIX               _mtxProjection;
    Light                   *_light;
    int                      _indexCount;
    ID3D11ShaderResourceView*_texture;
    FBXManager              *_fbxManager;
    TextureManager          *_textureManager;
    // �ʼ��� �ʿ��� ģ����.
    ID3D11DeviceContext*_dc;
    ID3D11VertexShader *_vs;
    ID3D11PixelShader  *_ps;
    ID3D11InputLayout  *_inputLayout;
    ID3D11SamplerState *_samplerState;
    ID3D11Buffer       *_vsCBuffer;
    ID3D11Buffer       *_psCBuffer;
};