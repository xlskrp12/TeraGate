#include "UI.h"
UI::UI()
{
}
UI::~UI()
{
}
void UI::init(ID3D11Device* device, int screenWidth, int screenHeight, WCHAR* textureFileName, int bitmapWidth, int bitmapHeight)
{
    _screenSize.x = screenWidth;
    _screenSize.y = screenHeight;
    _bitmapSize.x = bitmapWidth;
    _bitmapSize.y = bitmapHeight;
    _prevPos.x = -1;
    _prevPos.y = -1;
    initBuffer(device);
    loadTexture(device, textureFileName);
}
void UI::release()
{
    releaseTexture();
    releaseBuffer();
}
void UI::render(ID3D11DeviceContext* dc, int x, int y)
{
    updateBuffer(dc, x, y);
    renderBuffer(dc);
}
int UI::getIndexCount()
{
    return _indexCount;
}
ID3D11ShaderResourceView* UI::getTexture()
{
    return _texture->getTexture();
}
void UI::initBuffer(ID3D11Device* device)
{
    VertexType *vertex;
    ULONG      *index;
    D3D11_BUFFER_DESC      vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;

    // 사각형(두 개의 삼각형)을 만드릭 위해 정점수 6
    _vertexCount = 6;
    _indexCount  = 6;

    // 버텍스, 인덱스 초기화
    vertex = new VertexType[_vertexCount];
    index  = new ULONG[_indexCount];
    memset(vertex, 0, (sizeof(VertexType)*_vertexCount));
    for(int i=0;i<_indexCount;i++)
        index[i] = i;

    // 버텍스 버퍼 디스크립션 작성(동적 버퍼)
    vertexBufferDesc.Usage               = D3D11_USAGE_DYNAMIC;   // 동적 속성
    vertexBufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;// 동적 속성
    vertexBufferDesc.ByteWidth           = sizeof(VertexType) * _vertexCount;
    vertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;

    vertexBufferDesc.MiscFlags           = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // 인덱스 버퍼 디스크립션(정적 버퍼)
    indexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;// 정적 속성
    indexBufferDesc.CPUAccessFlags      = 0;                  // 정적 속성
    indexBufferDesc.ByteWidth           = sizeof(unsigned long) * _indexCount;
    indexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.MiscFlags           = 0;
    indexBufferDesc.StructureByteStride = 0;

    // 서브리소스
    vertexData.pSysMem          = vertex;
    vertexData.SysMemPitch      = 0;
    vertexData.SysMemSlicePitch = 0;
    indexData.pSysMem           = index;
    indexData.SysMemPitch       = 0;
    indexData.SysMemSlicePitch  = 0;

    HRESULT result;
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
    result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
    delete vertex; vertex = NULL;
    delete index;  index  = NULL;
}
void UI::releaseBuffer()
{
    if(_indexBuffer)
    {
        _indexBuffer->Release();
        _indexBuffer = NULL;
    }
    if(_vertexBuffer)
    {
        _vertexBuffer->Release();
        _vertexBuffer = NULL;
    }
}
void UI::updateBuffer(ID3D11DeviceContext* dc, int x, int y)
{
    float left, right, top, bottom;
    D3D11_MAPPED_SUBRESOURCE mappedSubResource;
    VertexType* vertex;
    VertexType* vertexPointer;
    // 이전 좌표와 동일하다면 바로 종료
    if((x == _prevPos.x) && (y == _prevPos.y))
        return;

    _prevPos.x = x;
    _prevPos.y = y;

    left   = (float)((_screenSize.x / 2)*-1) + (float)x;
    right  = left + (float)_bitmapSize.x;
    top    = (float)(_screenSize.y*1.414/2)-(float)y;
    bottom = top - (float)_bitmapSize.y*1.414;


    vertex = new VertexType[_vertexCount];

    vertex[0].position = D3DXVECTOR3(left, top, 0.0f);
    vertex[0].texture  = D3DXVECTOR2(0.0f, 0.0f);
    vertex[1].position = D3DXVECTOR3(right, bottom, 0.0f);
    vertex[1].texture  = D3DXVECTOR2(1.0f, 1.0f);
    vertex[2].position = D3DXVECTOR3(left, bottom, 0.0f);
    vertex[2].texture  = D3DXVECTOR2(0.0f, 1.0f);

    vertex[3].position = D3DXVECTOR3(left, top, 0.0f);
    vertex[3].texture  = D3DXVECTOR2(0.0f, 0.0f);
    vertex[4].position = D3DXVECTOR3(right, top, 0.0f);
    vertex[4].texture  = D3DXVECTOR2(1.0f, 0.0f);
    vertex[5].position = D3DXVECTOR3(right, bottom, 0.0f);
    vertex[5].texture  = D3DXVECTOR2(1.0f, 1.0f);


    //▼작업내용
    // dc로 정점버퍼를 포인터로 받아온다.
    // 작성권한 획득(map)
    // 정점배열을 정점버퍼로 복사
    // 접근권한 반환(unmap)
    dc->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
    vertexPointer = (VertexType*)mappedSubResource.pData;
    memcpy(vertexPointer, (void*)vertex, (sizeof(VertexType) * _vertexCount));
    dc->Unmap(_vertexBuffer, 0);
    delete[]vertex;

    //

}
void UI::renderBuffer(ID3D11DeviceContext* dc)
{
    unsigned int stride;
    unsigned int offset;
    stride = sizeof(VertexType);
    offset = 0;
    dc->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
    dc->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
void UI::loadTexture(ID3D11Device* device, WCHAR* filename)
{
    _texture = new Texture;
    if(_texture)
        _texture->init(device, filename);
}
void UI::releaseTexture()
{
    if(_texture)
    {
        _texture->release();
        delete _texture;
        _texture = NULL;
    }
}