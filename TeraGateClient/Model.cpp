#include"Model.h"
Model::Model()
{
    _vertexBuffer = 0;
    _indexBuffer  = 0;
}
Model::Model(const Model& other)
{

}
Model::~Model()
{

}

// 정점 버퍼와 인덱스 버퍼의 초기화 함수를 호출
bool Model::init(ID3D11Device* device)
{
    bool result;
    result = initBuffer(device);
    if (!result)
        return false;
    return true;
}

void Model::shut()
{
    shutBuffer();
    return;
}
// renderBuffer() 함수를 호출해 정점 버퍼와 인덱스 버퍼를 그래픽 파이프라인에 넣어 컬러 셰이더가 이들을 그릴 수 있게 한다.
void Model::render(ID3D11DeviceContext* deviceContext)
{
    // 정점 버퍼와 인덱스 버퍼를 그래픽스 파이프라인에 넣어 화면에 그릴 준비
    renderBuffer(deviceContext);
    return;
}
// 해당 모델의 인덱스 개수 반환
// 컬러 셰이더에서 모델을 그리기 위해 필요
int Model::getIndexCount()
{
    return _indexCount;
}
/*****************************************************************************************************
정점 버퍼와 인덱스 버퍼를 생성하는 작업 제어
보통은 데이터 파일로부터 모델 정보를 읽어와서 버퍼들을 만드는 일을 한다.
일단 튜토리얼이라 삼각형 하나만 다루기 때문에 정점 버퍼, 인덱스 버퍼에 점을 세팅하는 일만 해주자.
*****************************************************************************************************/
bool Model::initBuffer(ID3D11Device* device)
{
    /*
    싱글톤 오브젝트 매니저
    오브젝트 당 버텍스 3개 쓴다고 가정.
    3 * 오브젝트 개수 N 개의 버텍스 필요.
    3N 버텍스 필요.

    버텍스 개수를 3N으로 초기화한다.
    오브젝트 위치에 따른 버텍스 좌표를 초기화한다.
    */

    VertexType*            vertex;
    unsigned long*         index;
    D3D11_BUFFER_DESC      vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT                result;


    /*****************************************************************************************************
    정점, 인덱스 데이터를 담아둘 두 개의 임시 배열을 만들고 나중에 최종 버퍼를 생성할 때 사용한다.
    *****************************************************************************************************/
    // 정점 배열 길이 설정
    _vertexCount = 6;

    // 인덱스 배열 길이 설정
    _indexCount  = 6;

    // 정점 배열을 생성
    vertex = new VertexType[_vertexCount];
    if (!vertex)
        return false;

    // 인덱스 배열을 생성
    index = new unsigned long[_indexCount];
    if (!index)
        return false;

    /*****************************************************************************************************
    정점, 인덱스 배열에 삼각형의 각 점과 그 순서를 채워 넣는다.
    주의할 점은, 이것을 그리기 위해 점들을 시계방향으로 만들어야 한다.
    만약 반시계 방향으로 만들게되면 DirectX에서 이 삼각형은 반대편을 바라본다고 판단하여
    backface culling에 의해 그려지지 않게 된다.
    따라서 GPU에게 도형을 그리도록 할 때 이 순서를 기억하는 것이 중요하다.
    여기서는 정점의 description을 작성하기 떄문에 색상도 정해주게 된다.
    *****************************************************************************************************/
    // 삼각형1
    // 왼쪽 아래
    vertex[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  
    vertex[0].color    = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
    // 상단 가운데
    vertex[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  
    vertex[1].color    = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
    // 오른쪽 아래
    vertex[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
    vertex[2].color    = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

    // 삼각형2
    // 왼쪽 아래
    vertex[3].position = D3DXVECTOR3(-3.0f, -3.0f, -2.0f);
    vertex[3].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
    // 상단 가운데
    vertex[4].position = D3DXVECTOR3(-2.0f, -1.0f, -2.0f);
    vertex[4].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
    // 오른쪽 아래
    vertex[5].position = D3DXVECTOR3(-1.0f, -3.0f, -2.0f);
    vertex[5].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

    // 인덱스 배열에 값을 넣는다.
    // 순서대로 왼쪽 아래, 상단 가운데, 오른쪽 아래
    index[0] = 0;
    index[1] = 1;
    index[2] = 2;

    index[3] = 3;
    index[4] = 4;
    index[5] = 5;

    /*****************************************************************************************************
    정점&인덱스 배열이 채워졌으므로 이를 이용해 정점&인덱스 버퍼 생성
    우선 버퍼에 대한 description을 작성한다.
    description에는 버퍼크기 ByteWidth와 버퍼타입 BindFlags를 정확히 입력해야 한다.
    이를 채워넣은 후에는 정점&인덱스 배열을 subresource 포인터에 연결한다.
    이 description과 subresource포인터, D3D디바이스의 CreateBuffer 함수를 사용해 새 버퍼의 포인터를 받아온다.
    *****************************************************************************************************/
    // 정점버퍼 description 작성
    vertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth           = sizeof(VertexType) * _vertexCount;
    vertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags      = 0;
    vertexBufferDesc.MiscFlags           = 0;
    vertexBufferDesc.StructureByteStride = 0;
    
    // 정점 데이터를 가리키는 보조 리소스 구조체 작성
    vertexData.pSysMem          = vertex;
    vertexData.SysMemPitch      = 0;
    vertexData.SysMemSlicePitch = 0;

    // 정점 버퍼를 생성
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
    if (FAILED(result))
        return false;

    // 인덱스 버퍼의 description 작성
    indexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth           = sizeof(unsigned long) * _indexCount;
    indexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags      = 0;
    indexBufferDesc.MiscFlags           = 0;
    indexBufferDesc.StructureByteStride = 0;

    // 인덱스 데이터를 가리키는 보조 리소스 구조체 작성
    indexData.pSysMem          = index;
    indexData.SysMemPitch      = 0;
    indexData.SysMemSlicePitch = 0;

    // 인덱스 버퍼 생성
    result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
    if (FAILED(result))
        return false;

    /*****************************************************************************************************
    정점&인덱스 버퍼 생성 후에는 이미 값이 복사되어 필요가 없어진
    정점&인덱스 배열을 제거해준다.
    *****************************************************************************************************/
    delete[] vertex;
    vertex = 0;

    delete[] index;
    index = 0;

    return true;
}
// initBuffer()에서 만들었떤 정점&인덱스 버퍼를 해제한다
void Model::shutBuffer()
{
    if (_indexBuffer)
    {
        _indexBuffer->Release();
        _indexBuffer = 0;
    }
    if (_vertexBuffer)
    {
        _vertexBuffer->Release();
        _vertexBuffer = 0;
    }
    return;
}
/*****************************************************************************************************
renderBuffer()는 render()함수에서 호출된다.
정점&인덱스 버퍼를 GPU 어셈블러의 버퍼로서 활성화시키는 것이다.
일단 GPU가 활성화된 정점버퍼를 가지게 되면 셰이더를 이용해 버퍼의 내용을 그릴 수 있게 된다.
또한 이 함수에서는 정점을 삼각형, 선분, 부채꼴 등 어떤 모양으로 그릴지 정의한다.
*****************************************************************************************************/
void Model::renderBuffer(ID3D11DeviceContext* deviceContext)
{
    unsigned int stride;// 걸음걸이, 보폭이라는 사전적 의미를 가지고 있다.
    unsigned int offset;

    // 정점버퍼의 단위와 오프셋을 설정한다.
    stride = sizeof(VertexType);
    offset = 0;

    // input assembler에 정점 버퍼를 활성화해 그려질 수 있게 한다.
    deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

    // input assembler에 인덱스 버퍼를 활성화해 그려질 수 있게 한다.
    deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // 정점 버퍼로 그릴 기본형을 삼각형으로 설정한다.
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return;
}