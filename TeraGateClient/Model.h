#pragma once
#include<D3D11.h>
#include<D3DX10math.h>
class Model
{
public:
    Model();
    Model(const Model&);
    ~Model();
public:
    // render() 함수는 그래픽 카드에 모델들의 기하 정보를 넣고 컬러 셰이더로 그릴 준비를 한다.
    bool init(ID3D11Device*);
    void shut();
    void render(ID3D11DeviceContext*);
    int  getIndexCount();
private:
    // 아래 함수들은 3D모델의 정점&인덱스 버퍼들의 초기화, 종료과정을 제어한다.
    bool initBuffer(ID3D11Device*);
    void shutBuffer();
    void renderBuffer(ID3D11DeviceContext*);
private:
    ID3D11Buffer *_vertexBuffer, *_indexBuffer;
    int           _vertexCount,   _indexCount;
private:
    // Model 클래스의 정점 버퍼에 사용할 정점 구조체 선언.
    // 선언된 타입은 나중에 사용될 ColorShader 클래스에서 사용할 것과 그 구조가 같아야 한다.
    struct VertexType
    {
        D3DXVECTOR3 position;
        D3DXVECTOR4 color;
    };
};