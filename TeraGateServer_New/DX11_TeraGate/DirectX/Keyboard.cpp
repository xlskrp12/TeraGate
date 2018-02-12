#include"Keyboard.h"
Keyboard::Keyboard()
{
    for(int key=0;key<256;key++)
        _input[key] = false;
}
void Keyboard::down(int key)
{
    _input[key] = true;
    if(_input[VK_ESCAPE] == true)
        PostQuitMessage(1);
}
void Keyboard::up(int key)
{
    _input[key] = false;
}
bool Keyboard::isDown(int key)
{
    return _input[key];
}