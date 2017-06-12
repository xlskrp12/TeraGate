#include "Light.h"
Light::Light()
{

}
Light::Light(const Light&)
{

}
Light::~Light()
{

}
void Light::setDiffuseColor(float r, float g, float b, float a)
{
    _diffuseColor = D3DXVECTOR4(r, g, b, a);
}
void Light::setAmbientColor(float r, float g, float b, float a)
{
    _ambientColor = D3DXVECTOR4(r, g, b, a);
}
void Light::setDirection(float x, float y, float z)
{
    _direction = D3DXVECTOR3(x, y, z);
}
D3DXVECTOR4 Light::getDiffuseColor()
{
    return _diffuseColor;
}
D3DXVECTOR4 Light::getAmbientColor()
{
    return _ambientColor;
}
D3DXVECTOR3 Light::getDirection()
{
    return _direction;
}