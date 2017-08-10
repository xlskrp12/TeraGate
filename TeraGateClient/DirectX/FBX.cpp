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
    printf("�� �޽� ������ ������ : %d\n", _meshData.size());
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
    // ������ ����� ��Ʈ����Ʈ �ݺ�
    for (int i = 0; i < node->GetNodeAttributeCount(); i++)
    {
        FbxNodeAttribute* attribute = node->GetNodeAttributeByIndex(i);
        // ������ ��Ʈ����Ʈ�� �޽���
        if (attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            FbxMesh    *mesh = (FbxMesh*)attribute;
            FbxVector4 *controler = mesh->GetControlPoints();
            FBXVertex   fbxVertex;
            int         controlIndex;
            FbxVector4  normalVector;
            FbxVector2  uvVector;
            bool        isUV;
            // �޽��κ��� ���ؽ� ������ ����
            for (int polygonIndex = 0; polygonIndex < mesh->GetPolygonCount(); polygonIndex++)
            {
                for (int vertexIndex = 0; vertexIndex < mesh->GetPolygonSize(polygonIndex); vertexIndex++)
                {
                    // ��ġ����
                    controlIndex = mesh->GetPolygonVertex(polygonIndex, vertexIndex);
                    fbxVertex._pos = D3DXVECTOR3(
                        (float)controler[controlIndex].mData[0],
                        (float)controler[controlIndex].mData[1],
                        (float)controler[controlIndex].mData[2]);
                    
                    // UV
                    // UV�ؽ��� ���� ����Ʈ �ۼ�.
                    // �Ϲ������� fbx���� �ϳ��� �ؽ��� �ϳ��� ����ϹǷ� ����Ʈ�� 0�� �ε����� ����ϸ� �� ������ ������..
                    // ���� �ؽ��ĸ� ����ϰ� �Ǵ� ��� ����ؾ����� �𸣰ڴ�. ���߿� �����غ���.
                    FbxStringList uvNameList;
                    mesh->GetUVSetNames(uvNameList);
                    //printf("uv �̸� ���� : %d\n", uvNameList.GetCount());
                    //printf("uv �̸� : %s\n", uvNameList[0]);
                    mesh->GetPolygonVertexUV(polygonIndex, vertexIndex, uvNameList[0], uvVector, isUV);                    // ���� �� [in]������ �ε���, [in]���ؽ� �ε���, [in]uv�̸�, [out]uv����, [?] �÷���?
                    fbxVertex._uv = D3DXVECTOR2(
                        (float)uvVector[0],
                        (float)1-uvVector[1]);// uv�� v�� �Ʒ��� ������ 1�� ������.

                    // ��ֺ���
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
    // ���� ���鿡 ���� ���
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

    //�������迭, �ε����迭 ����
    vertex = new Vertex[_meshData.size()];
    index  = new unsigned long[_meshData.size()];
    //FBX���Ϸκ��� ���� _meshData������ �����迭�� ����
    std::vector<FBXVertex>::iterator it = _meshData.begin();
    for (int i = 0; i < _meshData.size(); i++, it++)
    {
        //�����迭 �ʱ�ȭ
        vertex[i]._pos = it->_pos;
        vertex[i]._uv = it->_uv;
        vertex[i]._normal = it->_normal;
        //�ε����迭 �ʱ�ȭ
        index[i] = i;
    }
    //�������� ��ũ����
    vertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth           = sizeof(Vertex) * _meshData.size();
    vertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags      = 0;
    vertexBufferDesc.MiscFlags           = 0;
    vertexBufferDesc.StructureByteStride = 0;
    //�������� ������
    vertexBufferData.pSysMem             = vertex;
    vertexBufferData.SysMemPitch         = 0;
    vertexBufferData.SysMemSlicePitch    = 0;
    //�������� ����
    result = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &_vertexBuffer);
    if (FAILED(result))MessageBox(NULL, L"ID3D11Device::CreateBuffer() ��������", L"Error", NULL);
    //�ε������� ��ũ����
    indexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth           = sizeof(unsigned long) * _meshData.size();
    indexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags      = 0;
    indexBufferDesc.MiscFlags           = 0;
    indexBufferDesc.StructureByteStride = 0;
    //�ε������� ������
    indexBufferData.pSysMem          = index;
    indexBufferData.SysMemPitch      = 0;
    indexBufferData.SysMemSlicePitch = 0;
    //�ε������� ����
    result = device->CreateBuffer(&indexBufferDesc, &indexBufferData, &_indexBuffer);

    if (FAILED(result))MessageBox(NULL, L"ID3D11Devie::CreateBuffer() �ε��� ����", L"Error", NULL);
    //����, �ε������۷� �̹� ������ ������ �Ѱ�� ������ �� �̻� �ʿ���� �ڿ�����
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
    deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);//�Է�������(IA, InputAssembler) ��������   Ȱ��ȭ
    deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);   //�Է�������(IA, InputAssembler) �ε������� Ȱ��ȭ

    //�������� ������ �⺻������ 
    //�� ģ�� ��� �����Ǳ� ��
    // D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST : �ﰢ��
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}