#include"FBX.h"
FBX::FBX()
{
    _manager  = NULL;
    _ios      = NULL;
    _importer = NULL;
    _scene    = NULL;
    _rootNode = NULL;


    // _magnager, _ios
    _manager  = FbxManager::Create();
    _ios      = FbxIOSettings::Create(_manager, IOSROOT);
    _manager->SetIOSettings(_ios);
    _importer = FbxImporter::Create(_manager, "myImporterName");

    // _scene, _rootNode
    _scene    = FbxScene::Create(_manager, "mySceneName");
    _rootNode = _scene->GetRootNode();
}
FBX::~FBX()
{
    release();
}
void FBX::init(ID3D11Device* device, char* fbxFileName, WCHAR* textureFileName)
{
    strcpy_s(_fbxFileName, sizeof(_fbxFileName), fbxFileName);
    initFBX(fbxFileName);
    initBuffer(device);
    initTexture(device, textureFileName);
#ifdef _DEBUG
    printf("Fbx::init();\n");
#endif
}
void FBX::release()
{
    releaseTexture();
    releaseFBX();
    releaseBuffer();
}
void FBX::info()
{
    printf("Fbx::info();//%s\n", _fbxFileName);
    printf("└ 메쉬 데이터 사이즈 : %d\n", _meshData.size());
    for (int i = 0; i < _meshData.size(); i++)
    {
        _meshData[i].info();
    }
}
void FBX::render(ID3D11DeviceContext* deviceContext)
{
    renderBuffer(deviceContext);
}
int  FBX::getIndexCount()
{
    return _meshData.size();
}
ID3D11ShaderResourceView* FBX::getTexture()
{
    return _texture->getTexture();
}
D3DXMATRIX FBX::getMtxWorld(D3DXMATRIX mtxWorld)
{
    return mtxWorld;
}
void FBX::initFBX(char* fbxFileName)
{
    bool result;
    result = _importer->Initialize(fbxFileName, -1, _manager->GetIOSettings());
#ifdef _DEBUG
    if (false == result)
        printf("fail to import \n", fbxFileName);
    else
        printf("succeed to import %s\n", fbxFileName);
#endif
    _importer->Import(_scene);
    _importer->Destroy();
    
    _meshData.clear();
    initFBXNode(_rootNode);
}
void FBX::initFBXNode(FbxNode* node)
{
    // 지정된 노드의 어트리뷰트 반복
    for (int i = 0; i < node->GetNodeAttributeCount(); i++)
    {
        FbxNodeAttribute* attribute = node->GetNodeAttributeByIndex(i);
        // 지정된 어트리뷰트가 메쉬면
        if (attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            FbxMesh    *mesh = (FbxMesh*)attribute;
            FbxVector4 *controler = mesh->GetControlPoints();
            FBXVertex   fbxVertex;
            int         controlIndex;
            FbxVector4  normalVector;
            FbxVector2  uvVector;
            bool        isUV;
            // 메쉬로부터 버텍스 정보를 추출
            for (int polygonIndex = 0; polygonIndex < mesh->GetPolygonCount(); polygonIndex++)
            {
                for (int vertexIndex = 0; vertexIndex < mesh->GetPolygonSize(polygonIndex); vertexIndex++)
                {
                    // 위치벡터
                    controlIndex = mesh->GetPolygonVertex(polygonIndex, vertexIndex);
                    fbxVertex._pos = D3DXVECTOR3(
                        (float)controler[controlIndex].mData[0],
                        (float)controler[controlIndex].mData[1],
                        (float)controler[controlIndex].mData[2]);
                    
                    // UV
                    // UV텍스쳐 네임 리스트 작성.
                    // 일반적으로 fbx파일 하나당 텍스쳐 하나를 사용하므로 리스트의 0번 인덱스를 사용하면 별 문제가 없지만..
                    // 여러 텍스쳐를 사용하게 되는 경우 어떻게해야할지 모르겠다. 나중에 생각해보자.
                    FbxStringList uvNameList;
                    mesh->GetUVSetNames(uvNameList);
                    //printf("uv 이름 개수 : %d\n", uvNameList.GetCount());
                    //printf("uv 이름 : %s\n", uvNameList[0]);
                    mesh->GetPolygonVertexUV(polygonIndex, vertexIndex, uvNameList[0], uvVector, isUV);                    // 인자 ▶ [in]폴리곤 인덱스, [in]버텍스 인덱스, [in]uv이름, [out]uv벡터, [?] 플래그?
                    fbxVertex._uv = D3DXVECTOR2(
                        (float)uvVector[0],
                        (float)1-uvVector[1]);// uv의 v는 아래로 갈수록 1에 가깝다.

                    // 노멀벡터
                    mesh->GetPolygonVertexNormal(polygonIndex, vertexIndex, normalVector);
                    fbxVertex._normal = D3DXVECTOR3(
                        (float)normalVector.mData[0],
                        (float)normalVector.mData[1],
                        (float)normalVector.mData[2]);
                    _meshData.push_back(fbxVertex);

                }
            }
        }
    }
    // 하위 노드들에 대해 재귀
    for (int i = 0; i < node->GetChildCount(); i++)
    {
        initFBXNode(node->GetChild(i));
    }
}
void FBX::initBuffer(ID3D11Device* device)
{
    Vertex                 *vertex;
    unsigned long          *index;
    D3D11_BUFFER_DESC      vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexBufferData, indexBufferData;
    HRESULT                result;

    //▼정점배열, 인덱스배열 생성
    vertex = new Vertex[_meshData.size()];
    index  = new unsigned long[_meshData.size()];
    //FBX파일로부터 읽은 _meshData값들을 정점배열로 복사
    std::vector<FBXVertex>::iterator it = _meshData.begin();
    for (int i = 0; i < _meshData.size(); i++, it++)
    {
        //정점배열 초기화
        vertex[i]._pos = it->_pos;
        vertex[i]._uv = it->_uv;
        vertex[i]._normal = it->_normal;
        //인덱스배열 초기화
        index[i] = i;
    }
    //정점버퍼 디스크립션
    vertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth           = sizeof(Vertex) * _meshData.size();
    vertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags      = 0;
    vertexBufferDesc.MiscFlags           = 0;
    vertexBufferDesc.StructureByteStride = 0;
    //정점버퍼 데이터
    vertexBufferData.pSysMem             = vertex;
    vertexBufferData.SysMemPitch         = 0;
    vertexBufferData.SysMemSlicePitch    = 0;
    //정점버퍼 생성
    result = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &_vertexBuffer);
    if (FAILED(result))MessageBox(NULL, L"ID3D11Device::CreateBuffer() 정점버퍼", L"Error", NULL);
    //인덱스버퍼 디스크립션
    indexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth           = sizeof(unsigned long) * _meshData.size();
    indexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags      = 0;
    indexBufferDesc.MiscFlags           = 0;
    indexBufferDesc.StructureByteStride = 0;
    //인덱스버퍼 데이터
    indexBufferData.pSysMem          = index;
    indexBufferData.SysMemPitch      = 0;
    indexBufferData.SysMemSlicePitch = 0;
    //인덱스버퍼 생성
    result = device->CreateBuffer(&indexBufferDesc, &indexBufferData, &_indexBuffer);

    if (FAILED(result))MessageBox(NULL, L"ID3D11Devie::CreateBuffer() 인덱스 버퍼", L"Error", NULL);
    //정점, 인덱스버퍼로 이미 값들을 복사해 넘겼기 때문에 더 이상 필요없는 자원해제
    delete[]vertex; vertex = NULL;
    delete[]index;  index = NULL;
}
void FBX::initTexture(ID3D11Device *device, WCHAR *textureFileName)
{
    _texture = new Texture;
    if (!_texture)MessageBox(NULL, L"new Texture", L"Error", NULL);
    _texture->init(device, textureFileName);
}
void FBX::releaseBuffer()
{
    if (_indexBuffer)
    {
        _indexBuffer->Release();
        _indexBuffer = NULL;
    }
    if (_vertexBuffer)
    {
        _vertexBuffer->Release();
        _vertexBuffer = NULL;
    }
}
void FBX::releaseFBX()
{
    _meshData.clear();
    _manager->Destroy();
}
void FBX::releaseTexture()
{
    if (_texture)
    {
        _texture->release();
        delete _texture;
        _texture = NULL;
    }
}
void FBX::renderBuffer(ID3D11DeviceContext *deviceContext)
{
    unsigned int stride = sizeof(Vertex);
    unsigned int offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);//입력조립기(IA, InputAssembler) 정점버퍼   활성화
    deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);   //입력조립기(IA, InputAssembler) 인덱스버퍼 활성화

    //정점버퍼 렌더시 기본형설정 
    //이 친구 없어도 렌더되긴 함
    // D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST : 삼각형
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}