#pragma once
#include"Prev.h"
#include"Texture.h"
class Model
{
public://▼인터페이스 메소드
    Model();
    Model(const Model&);
    ~Model();
    void init(ID3D11Device*, char*, WCHAR*);
    void release();
    void render(ID3D11DeviceContext*);
    void update();
    int  getIndexCount();
    ID3D11ShaderResourceView* getTexture();
    D3DXMATRIX getMtxWorld(float x, float y, float z, D3DXMATRIX);
private://▼메소드
    void initBuffer(ID3D11Device*);
    void releaseBuffer();
    void renderBuffer(ID3D11DeviceContext*);
    void initTexture(ID3D11Device*, WCHAR*);
    void releaseTexture();
    void initModel(char*);
    void releaseModel();
private://▼자료형
    // *.vs에서 작성한 자료형과 구조가 같아야 된다.
    struct Vertex
    {
        D3DXVECTOR3 position;
        D3DXVECTOR2 texcoord;
        D3DXVECTOR3 normal;
    };
    struct ModelData
    {
        float x, y, z;
        float u, v;
        float nx, ny, nz;
    };
    // 변수
    ID3D11Buffer *_vertexBuffer, *_indexBuffer;
    int           _vertexCount,   _indexCount;
    Texture      *_texture;
    ModelData    *_modelData;
};