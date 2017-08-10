#pragma once
#include"Prev.h"
#include"Texture.h"
#include<fbxsdk.h>
#include<vector>
class FBX
{
public://�������̽�
    FBX();
    ~FBX();
    void init(ID3D11Device *device, char *fbxFileName, WCHAR *ddsFilename);
    void release();
    void info();
    void render(ID3D11DeviceContext *deviceContext);
    int  getIndexCount();
    ID3D11ShaderResourceView* getTexture();
    D3DXMATRIX getMtxWorld(D3DXMATRIX);
protected://����Լ�
    void initFBX(char *fbxFileName);
    void initFBXNode(FbxNode *node);
    void initBuffer(ID3D11Device *device);
    void initTexture(ID3D11Device *device, WCHAR *textureFileName);
    void releaseFBX();
    void releaseBuffer();
    void releaseTexture();
    void renderBuffer(ID3D11DeviceContext* deviceContext);
protected://Ÿ��
    // FBX������ �о� FBXVertexŸ������ ����
    // �� ���� ���ؽ��� ������ �𸣴� std::vector, push_back�� �̿�
    struct FBXVertex
    {
        FBXVertex()  {};
        ~FBXVertex() {};
        void info()
        {
            printf("  pos(%+.2f, %+.2f, %+.2f)\t  normal(%+.2f, %+.2f, %+.2f)\t  uv(%+.2f, %+.2f)\n", _pos[0], _pos[1], _pos[2], _normal[0], _normal[1], _normal[2], _uv[0], _uv[1]);
        }
        D3DXVECTOR3 _pos;
        D3DXVECTOR2 _uv;
        D3DXVECTOR3 _normal;
    };
    // D3DXVECTOR4 _weight;
    // D3DXVECTOR4 _index;
    // FBX����Ʈ �� _meshData.size()�� ���ؽ� ���ڸ� �˰ԵǾ����Ƿ�
    // new Vertex[_meshData.size()] ������ �� �ִ�.
    // �����̳ʸ� �״�� �Ⱦ��� ���� ���� �Ҵ��ϴ� ������..
    // D3D11_SUBRESOURCE_DATA::pSysMem�� �־�� �ϴϱ�..
    struct Vertex
    {
        D3DXVECTOR3 _pos;
        D3DXVECTOR2 _uv;
        D3DXVECTOR3 _normal;
    };
    /*
    struct KeyFrame
    {
        FBXAMatrix _mtxAni;
        double     _time;
    }
    struct Bone
    {

    }
    struct skin
    {
        ��Ų������ ��~���ؽ��� ����Ʈ ���� �ʿ�
        double _weight;
    }

    */
protected://�������
    //��FBX������
    char _fbxFileName[200];
    int  _tab;
    FbxManager            *_manager;
    FbxIOSettings         *_ios;
    FbxImporter           *_importer;
    FbxScene              *_scene;
    FbxNode               *_rootNode;
    std::vector<FBXVertex> _meshData;
    //��DirectX������
    ID3D11Buffer *_vertexBuffer;
    ID3D11Buffer *_indexBuffer;
    Texture      *_texture;
};