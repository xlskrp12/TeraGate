/*****************************************************************************************************
FileName : Input.cpp
*****************************************************************************************************/
#include"Input.h"
Input::Input()
{

}
Input::Input(const Input& inputManager)
{

}
Input::~Input()
{

}
void Input::init()
{
    int i;
    for (i = 0; i < 256; i++)
        _key[i] = false;
    return;
}
void Input::keyDown(unsigned int key)
{
    _key[key] = true;
    return;
}
void Input::keyUp(unsigned int key)
{
    _key[key] = false;
    return;
}
bool Input::isKeyDown(unsigned int key)
{
    return _key[key];
}