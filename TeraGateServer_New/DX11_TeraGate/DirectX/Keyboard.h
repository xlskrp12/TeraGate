#pragma once
#include"Prev.h"
class Keyboard
{
public:
    Keyboard();
    void down(int key);
    void up(int key);
    bool isDown(int key);
private:
    bool _input[256];
};