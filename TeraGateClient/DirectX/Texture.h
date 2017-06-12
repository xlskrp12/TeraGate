#pragma once
#include"Prev.h"
class Texture
{
public:
    Texture();
    Texture(const Texture&);
    ~Texture();
    void init(ID3D11Device*, WCHAR*);
    void release();
    ID3D11ShaderResourceView* getTexture();
private:
    ID3D11ShaderResourceView* _texture;
};

