#include "Model.h"
Model::Model()
{
    _vertexBuffer = NULL;
    _indexBuffer  = NULL;
    _texture      = NULL;
    _modelData    = NULL;
}
Model::Model(const Model&)
{

}
Model::~Model()
{

}
void Model::init(ID3D11Device* device, char* modelFileName, WCHAR* textureFileName)
{
    initModel(modelFileName);
    initBuffer(device);
    initTexture(device, textureFileName);
#ifdef _DEBUG
    printf("Model::init();\n");
#endif
}
void Model::release()
{
    releaseTexture();
    releaseBuffer();
    releaseModel();
}
void Model::render(ID3D11DeviceContext* deviceContext)
{
    renderBuffer(deviceContext);
}
void Model::update()
{

}
int  Model::getIndexCount()
{
    return _indexCount;
}
ID3D11ShaderResourceView* Model::getTexture()
{
    return _texture->getTexture();
}
void Model::initBuffer(ID3D11Device* device)
{
    // �� ������ ���Ϸκ��� ������ �о�� ���۸� ����
    Vertex*                vertex;
    unsigned long*         index;
    D3D11_BUFFER_DESC      vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT                result;
    
    //������&�ε��� �迭 ����
    vertex = new Vertex[_vertexCount];
    if(!vertex) MessageBox(NULL, L"new Vertex[_vertexCount]", L"Error", NULL);
    index = new unsigned long[_indexCount];
    if (!index) MessageBox(NULL, L"new index[_indexCount]", L"Error", NULL);
    for (int i = 0; i < _vertexCount; i++)
    {
        vertex[i].position = D3DXVECTOR3(_modelData[i].x, _modelData[i].y, _modelData[i].z);
        vertex[i].texcoord = D3DXVECTOR2(_modelData[i].u, _modelData[i].v);
        vertex[i].normal   = D3DXVECTOR3(_modelData[i].nx, _modelData[i].ny, _modelData[i].nz);
        index[i] = i;
    }
    //���������� ����
    // �������� ��ũ���� �ۼ�
    vertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth           = sizeof(Vertex) * _vertexCount;
    vertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags      = 0;
    vertexBufferDesc.MiscFlags           = 0;
    vertexBufferDesc.StructureByteStride = 0;
    // ���������� �ۼ�
    vertexData.pSysMem          = vertex;
    vertexData.SysMemPitch      = 0;
    vertexData.SysMemSlicePitch = 0;
    // �������� ����
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
    if (FAILED(result)) MessageBox(NULL, L"ID3D11Device::CreateBuffer() ��������", L"Error", NULL);
    //���ε������� ����
    // �ε������� ��ũ���� �ۼ�
    indexBufferDesc.Usage                = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth            = sizeof(unsigned long)*_indexCount;
    indexBufferDesc.BindFlags            = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags       = 0;
    indexBufferDesc.MiscFlags            = 0;
    indexBufferDesc.StructureByteStride  = 0;
    // �ε��� ������ �ۼ�
    indexData.pSysMem           = index;
    indexData.SysMemPitch       = 0;
    indexData.SysMemSlicePitch  = 0;
    // �ε������� ����
    result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
    if (FAILED(result)) MessageBox(NULL, L"ID3D11Device::CreateBuffer() �ε�������", L"Error", NULL);

    //����۷� �̹� ������ ������ �Ѱ�� ������ �� �̻� �ʿ���� �ڿ�����
    delete[] vertex;
    vertex = NULL;
    delete[] index;
    index  = NULL;
}
void Model::releaseBuffer()
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
/*�� renderBuffer() ����
    ����&�ε��� ���۸� �׷��� ���������ο� �ִ´�.
    ���÷��� ��ο츦 ���� �غ�ܰ�.
*/
void Model::renderBuffer(ID3D11DeviceContext* deviceContext)
{
    //���� ģ���� ������ �ƿ� �ȱ׷���
    // �������� ����, ������ �ʱ�ȭ
    unsigned int stride = sizeof(Vertex);
    unsigned int offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);// input assembler�� �������� Ȱ��ȭ
    deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);   // input assembler�� �ε������� Ȱ��ȭ

    //���� ģ�� ��� �׷����� ��
    // �������۷� �׸� �� �⺻�� ����
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST�� �ﰢ��
}
void Model::initModel(char* modelFileName)
{
    using std::ifstream;
    ifstream file;
    char fileBuffer;

    //������� �Ľ�
    // ���Ͽ���
    file.open(modelFileName);
    if (file.fail())MessageBox(NULL, L"file.open(modelFileName)", L"Error", NULL);
    // ���ؽ� ī��Ʈ üũ
    file.get(fileBuffer);
    while(fileBuffer != ':')
        file.get(fileBuffer);
    file >> _vertexCount;      // ���ؽ� ī��Ʈ �б�
    _indexCount = _vertexCount;// �ε��� ī��Ʈ �� �����ֱ�
    // �𵨵����� �ۼ�
    _modelData = new ModelData[_vertexCount];
    if (!_modelData) MessageBox(NULL, L"new ModelData[_vertexCount]", L"Error", NULL);
    file.get(fileBuffer);
    while (fileBuffer != ':')
        file.get(fileBuffer);
    file.get(fileBuffer);
    file.get(fileBuffer);
    for (int i = 0; i < _vertexCount; i++)
    {
        file >> _modelData[i].x  >> _modelData[i].y  >> _modelData[i].z;
        file >> _modelData[i].u  >> _modelData[i].v;
        file >> _modelData[i].nx >> _modelData[i].ny >> _modelData[i].nz;        
    }
    // ���ϴݱ�
    file.close();
}
void Model::releaseModel()
{
    if (_modelData)
    {
        delete[]_modelData;
        _modelData = NULL;
    }
}
void Model::initTexture(ID3D11Device* device, WCHAR* textureFileName)
{
    _texture = new Texture;
    if (!_texture) MessageBox(NULL, L"new Texture", L"Error", NULL);
    _texture->init(device, textureFileName);
}
void Model::releaseTexture()
{
    if (_texture)
    {
        _texture->release();
        delete _texture;
        _texture = NULL;
    }
}
// Direct::getMtxWorld()�� ���ڷ� �޾� ���� (����)������ǥ�迡�� ȸ����Ų �� ����
D3DXMATRIX Model::getMtxWorld(float x, float y, float z, D3DXMATRIX mtxWorld)
{
    //static float yRot = 0.f;
    //yRot = yRot + 0.05f;
    //if (yRot < -360.f)
    //    yRot = yRot + 360.f;
    //if (360.f < yRot)
    //    yRot = yRot - 360.f;
    //D3DXMatrixRotationY(&mtxWorld, yRot);
    D3DXMatrixTranslation(&mtxWorld, x, y, z);
    return mtxWorld;
}