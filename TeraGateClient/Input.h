/*****************************************************************************************************
FileName : Input.h
*****************************************************************************************************/
#pragma once
class Input
{
public:
    Input();
    Input(const Input&);
    ~Input();
    void init();
    void keyDown(unsigned int);
    void keyUp(unsigned int);
    bool isKeyDown(unsigned int);

    const static int KEY_BUFFER_SIZE = 256;
private:
    bool _key[KEY_BUFFER_SIZE];

};