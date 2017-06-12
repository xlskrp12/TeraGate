#pragma once
#include"Prev.h"
#include"Texture.h"
#include<fbxsdk.h>
#include<vector>
class FBX
{
public://인터페이스
    FBX();
    ~FBX();
    void init(ID3D11Device *device, char *fbxFileName, WCHAR *ddsFilename);
    void release();
    void info();
    void render(ID3D11DeviceContext *deviceContext);
    int  getIndexCount();
    ID3D11ShaderResourceView* getTexture();
    D3DXMATRIX getMtxWorld(D3DXMATRIX);
protected://멤버함수
    void initFBX(char *fbxFileName);
    void initFBXNode(FbxNode *node);
    void initBuffer(ID3D11Device *device);
    void initTexture(ID3D11Device *device, WCHAR *textureFileName);
    void releaseFBX();
    void releaseBuffer();
    void releaseTexture();
    void renderBuffer(ID3D11DeviceContext* deviceContext);
protected://타입
    // FBX파일을 읽어 FBXVertex타입으로 저장
    // 몇 개의 버텍스가 있을지 모르니 std::vector, push_back을 이용
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
    // FBX임포트 후 _meshData.size()로 버텍스 숫자를 알게되었으므로
    // new Vertex[_meshData.size()] 일케할 수 있다.
    // 컨테이너를 그대로 안쓰고 굳이 새로 할당하는 이유는..
    // D3D11_SUBRESOURCE_DATA::pSysMem에 넣어야 하니까..
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
        스킨값에는 본~버텍스간 웨이트 값이 필요
        double _weight;
    }

    */
protected://멤버변수
    //▼FBX변수들
    char _fbxFileName[200];
    int  _tab;
    FbxManager            *_manager;
    FbxIOSettings         *_ios;
    FbxImporter           *_importer;
    FbxScene              *_scene;
    FbxNode               *_rootNode;
    std::vector<FBXVertex> _meshData;
    //▼DirectX변수들
    ID3D11Buffer *_vertexBuffer;
    ID3D11Buffer *_indexBuffer;
    Texture      *_texture;
};