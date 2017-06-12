#include "Texture.h"
Texture::Texture()
{
    _texture = NULL;
}
Texture::Texture(const Texture& texture)
{
}
Texture::~Texture()
{
}
void Texture::init(ID3D11Device* device, WCHAR* textureFileName)
{
    HRESULT result;
    result = D3DX11CreateShaderResourceViewFromFile(device, textureFileName, NULL, NULL, &_texture, NULL);
    if (FAILED(result))
        MessageBox(NULL, L"D3DX11CreateShaderResourceViewFromFile()", L"Error", NULL);
#ifdef _DEBUG
    printf("Texture::init()\n");
#endif
}
void Texture::release()
{
    if (_texture)
    {
        _texture->Release();
        _texture = NULL;
    }
}
ID3D11ShaderResourceView* Texture::getTexture()
{
    return _texture;
}