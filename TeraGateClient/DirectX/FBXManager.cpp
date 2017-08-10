#include"FBXManager.h"
// ΩÃ±€≈Ê 
FBXManager::FBXManager(){}
FBXManager* FBXManager::_singleton = NULL;
FBXManager* FBXManager::getSingleton()
{
    if(_singleton == NULL)
        _singleton = new FBXManager;
    return _singleton;
}
// ∏ﬁº“µÂ
void FBXManager::init(HWND hwnd, Direct* direct)
{
    _hwnd   = hwnd;
    _direct = direct;
    //°Âfbx∑ŒµÂ
    for(int i=0;i<FBXIndex::TOTAL;i++)
    {
        _fbx[i] = new FBX;
        if (!_fbx[i]) MessageBox(NULL, L"new FBX", L"Error", NULL);
    }
    _fbx[FBXIndex::GROUND]->init(_direct->getDevice(), "Resource/FBX/Ground.fbx", L"Resource/DDS/Ground.dds");
    _fbx[FBXIndex::WALL]->init(_direct->getDevice(), "Resource/FBX/wall.fbx", L"Resource/DDS/wall.dds");
    _fbx[FBXIndex::SKY]->init(_direct->getDevice(), "Resource/FBX/skybox.fbx", L"Resource/DDS/skybox.dds");
    _fbx[FBXIndex::GATE]->init(_direct->getDevice(), "Resource/FBX/gate.fbx", L"Resource/DDS/gate.dds");
    _fbx[FBXIndex::STONE]->init(_direct->getDevice(), "Resource/FBX/stone.fbx", L"Resource/DDS/stone.dds");
    _fbx[FBXIndex::MINION]->init(_direct->getDevice(), "Resource/FBX/minion.fbx", L"Resource/DDS/minion.dds");
    _fbx[FBXIndex::PC]->init(_direct->getDevice(), "Resource/FBX/hengelBlack.fbx", L"Resource/DDS/hengelBlack.dds");
    _fbx[FBXIndex::WORM]->init(_direct->getDevice(), "Resource/FBX/worm.fbx", L"Resource/DDS/worm.dds");
}
void FBXManager::render(int index)
{
    _fbx[index]->render(_direct->getDeviceContext());
}
int  FBXManager::getIndexCount(int index)
{
    return _fbx[index]->getIndexCount();
}
ID3D11ShaderResourceView*FBXManager::getTexture(int index)
{
    return _fbx[index]->getTexture();
}