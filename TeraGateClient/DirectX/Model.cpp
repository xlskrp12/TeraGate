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
    // 모델 데이터 파일로부터 정보를 읽어와 버퍼를 만듦
    Vertex*                vertex;
    unsigned long*         index;
    D3D11_BUFFER_DESC      vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT                result;
    
    //▼정점&인덱스 배열 생성
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
    //▼정점버퍼 생성
    // 정점버퍼 디스크립션 작성
    vertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth           = sizeof(Vertex) * _vertexCount;
    vertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags      = 0;
    vertexBufferDesc.MiscFlags           = 0;
    vertexBufferDesc.StructureByteStride = 0;
    // 정점데이터 작성
    vertexData.pSysMem          = vertex;
    vertexData.SysMemPitch      = 0;
    vertexData.SysMemSlicePitch = 0;
    // 정점버퍼 생성
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
    if (FAILED(result)) MessageBox(NULL, L"ID3D11Device::CreateBuffer() 정점버퍼", L"Error", NULL);
    //▼인덱스버퍼 생성
    // 인덱스버퍼 디스크립션 작성
    indexBufferDesc.Usage                = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth            = sizeof(unsigned long)*_indexCount;
    indexBufferDesc.BindFlags            = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags       = 0;
    indexBufferDesc.MiscFlags            = 0;
    indexBufferDesc.StructureByteStride  = 0;
    // 인덱스 데이터 작성
    indexData.pSysMem           = index;
    indexData.SysMemPitch       = 0;
    indexData.SysMemSlicePitch  = 0;
    // 인덱스버퍼 생성
    result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
    if (FAILED(result)) MessageBox(NULL, L"ID3D11Device::CreateBuffer() 인덱스버퍼", L"Error", NULL);

    //▼버퍼로 이미 값들을 복사해 넘겼기 때문에 더 이상 필요없는 자원해제
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
/*▼ renderBuffer() 역할
    정점&인덱스 버퍼를 그래픽 파이프라인에 넣는다.
    디스플레이 드로우를 위한 준비단계.
*/
void Model::renderBuffer(ID3D11DeviceContext* deviceContext)
{
    //▼이 친구들 없으면 아예 안그려짐
    // 정점버퍼 단위, 오프셋 초기화
    unsigned int stride = sizeof(Vertex);
    unsigned int offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);// input assembler의 정점버퍼 활성화
    deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);   // input assembler의 인덱스버퍼 활성화

    //▼이 친구 없어도 그려지긴 함
    // 정점버퍼로 그릴 때 기본형 설정
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST는 삼각형
}
void Model::initModel(char* modelFileName)
{
    using std::ifstream;
    ifstream file;
    char fileBuffer;

    //▼모델파일 파싱
    // 파일오픈
    file.open(modelFileName);
    if (file.fail())MessageBox(NULL, L"file.open(modelFileName)", L"Error", NULL);
    // 버텍스 카운트 체크
    file.get(fileBuffer);
    while(fileBuffer != ':')
        file.get(fileBuffer);
    file >> _vertexCount;      // 버텍스 카운트 읽기
    _indexCount = _vertexCount;// 인덱스 카운트 값 맞춰주기
    // 모델데이터 작성
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
    // 파일닫기
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
// Direct::getMtxWorld()를 인자로 받아 모델의 (로컬)월드좌표계에서 회전시킨 후 리턴
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