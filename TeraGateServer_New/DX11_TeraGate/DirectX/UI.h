#pragma once
#include"Prev.h"
#include"Texture.h"
class UI
{
private:
    struct VertexType
    {
        D3DXVECTOR3 position;
        D3DXVECTOR2 texture;
    };
public:
    UI();
    ~UI();
    void init(ID3D11Device*, int, int, WCHAR*, int, int);
    void release();
    void render(ID3D11DeviceContext*, int x, int y);
    int getIndexCount();
    ID3D11ShaderResourceView* getTexture();
private:
    void initBuffer(ID3D11Device*);
    void releaseBuffer();
    void updateBuffer(ID3D11DeviceContext*, int x, int y);
    void renderBuffer(ID3D11DeviceContext*);
    void loadTexture(ID3D11Device*, WCHAR*);
    void releaseTexture();
private:
    ID3D11Buffer *_vertexBuffer, *_indexBuffer;
    int           _vertexCount, _indexCount;
    Texture      *_texture;
    POINT         _screenSize;
    POINT         _bitmapSize;
    POINT         _prevPos;
};

