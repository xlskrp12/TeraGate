#pragma once
#include"Prev.h"
#include"Direct.h"
#include"Light.h"
#include"FBXManager.h"
#include"TextureManager.h"
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
    void setLight(Light*);            // 스왑체인 한 번 당 한 번씩 밖에 바뀌지 않는 친구들이라 셰이더 render()전에 한 번씩만 호출하면 된다.
    void setMtxCameraView(D3DXMATRIX);// 스왑체인 한 번 당 한 번씩 밖에 바뀌지 않는 친구들이라 셰이더 render()전에 한 번씩만 호출하면 된다.
    void setMtxProjection();          // 스왑체인 한 번 당 한 번씩 밖에 바뀌지 않는 친구들이라 셰이더 render()전에 한 번씩만 호출하면 된다.
    void AmbientLightShader::target(int fbxID, int textureID);// fbx종류당 한 번씩 호출해주면 된다. fbxManager에서 같은 종류는 같은 종류끼리 뭉쳐서 정렬시켜줄거다.
    void render(D3DXMATRIX);          // 백버퍼 드로우 전에 텍스쳐를 설정해야 함을 주의.
private://▼메소드
    void initShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
    void releaseShader();
    void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
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
    // 필수로 필요한 친구들 + 코스트를 줄이기 위한 친구들.
    D3DXMATRIX               _mtxCameraView;
    D3DXMATRIX               _mtxProjection;
    Light                   *_light;
    int                      _indexCount;
    ID3D11ShaderResourceView*_texture;
    FBXManager              *_fbxManager;
    TextureManager          *_textureManager;
    // 필수로 필요한 친구들.
    ID3D11DeviceContext*_dc;
    ID3D11VertexShader *_vs;
    ID3D11PixelShader  *_ps;
    ID3D11InputLayout  *_inputLayout;
    ID3D11SamplerState *_samplerState;
    ID3D11Buffer       *_vsCBuffer;
    ID3D11Buffer       *_psCBuffer;
};