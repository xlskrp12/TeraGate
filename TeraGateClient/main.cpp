/*****************************************************************************************************
FileName : main.cpp
*****************************************************************************************************/
#include"main.h"
int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, PSTR cmdLine, int cmdShow)
{
    DXSystem* dxSystem = new DXSystem;
    if (!dxSystem)
        return 0;   

    if (dxSystem->init())
        dxSystem->run();

    dxSystem->shut();
    delete dxSystem;
    dxSystem = NULL;
    return 0;
}