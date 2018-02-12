#include"FBXManager.h"
// 싱글톤 
FBXManager::FBXManager(){}
FBXManager* FBXManager::_singleton = NULL;
FBXManager* FBXManager::getSingleton()
{
    if(_singleton == NULL)
        _singleton = new FBXManager;
    return _singleton;
}
// 메소드
void FBXManager::init(HWND hwnd, Direct* direct)
{
    _hwnd   = hwnd;
    _direct = direct;
    //▼fbx로드
    for(int i=0;i<FBXIndex::INDEX_TOTAL;i++)
    {
        _fbx[i] = new FBX;
        if (!_fbx[i]) MessageBox(NULL, L"new FBX", L"Error", NULL);
    }
    _fbx[FBXIndex::SLASH]->init(_direct->getDevice(), "Resource/FBX/slash.fbx");
    _fbx[FBXIndex::HP_GREEN]->init(_direct->getDevice(), "Resource/FBX/hpGreen.fbx");
    _fbx[FBXIndex::UI_HP_BAR]->init(_direct->getDevice(), "Resource/FBX/uiHPBar.fbx");
    _fbx[FBXIndex::UI_MINIMAP]->init(_direct->getDevice(), "Resource/FBX/uiMinimap.fbx");
    _fbx[FBXIndex::UI_MINIMAP_MARK]->init(_direct->getDevice(), "Resource/FBX/uiMinimapMark.fbx");
    _fbx[FBXIndex::UI_MINIMAP_MARK_GATE]->init(_direct->getDevice(), "Resource/FBX/uiMinimapMarkGate.fbx");
    _fbx[FBXIndex::GROUND]->init(_direct->getDevice(), "Resource/FBX/Ground.fbx");
    _fbx[FBXIndex::WALL]->init(_direct->getDevice(), "Resource/FBX/wall.fbx");
    _fbx[FBXIndex::SKY]->init(_direct->getDevice(), "Resource/FBX/skybox.fbx");
    _fbx[FBXIndex::SKY_IRON]->init(_direct->getDevice(), "Resource/FBX/skybox.fbx");
    _fbx[FBXIndex::MAIN_GATE]->init(_direct->getDevice(), "Resource/FBX/mainGate.fbx");
    _fbx[FBXIndex::GATE]->init(_direct->getDevice(), "Resource/FBX/gate.fbx");
    _fbx[FBXIndex::GATE_CORE]->init(_direct->getDevice(), "Resource/FBX/gateCore.fbx");
    _fbx[FBXIndex::STONE]->init(_direct->getDevice(), "Resource/FBX/stone.fbx");
    _fbx[FBXIndex::STONE_TREE]->init(_direct->getDevice(), "Resource/FBX/stoneTree.fbx");
    _fbx[FBXIndex::TREE]->init(_direct->getDevice(), "Resource/FBX/tree.fbx");    //모델링 : 트리   ▶ dds : 스톤트리
    _fbx[FBXIndex::MINION]->init(_direct->getDevice(), "Resource/FBX/minion.fbx");//모델링 : 미니언 ▶ dds : magician
    _fbx[FBXIndex::MAGICIAN]->init(_direct->getDevice(), "Resource/FBX/magician.fbx");
    _fbx[FBXIndex::WORM]->init(_direct->getDevice(), "Resource/FBX/worm.fbx");
    _fbx[FBXIndex::LOGO]->init(_direct->getDevice(), "Resource/FBX/logo.fbx");
    _fbx[FBXIndex::EFFECT1]->init(_direct->getDevice(), "Resource/FBX/effect1.fbx");
    _fbx[FBXIndex::EFFECT2]->init(_direct->getDevice(), "Resource/FBX/effect2.fbx");
    _fbx[FBXIndex::SHURIKEN]->init(_direct->getDevice(), "Resource/FBX/shuriken.fbx");


    _fbx[FBXIndex::MINION]->infoToFile("Resource/FBX/Info/minion.txt");
    //_fbx[FBXIndex::MINION]->test();


    char fbxPath[256];
    // 미니언 애니
    for(int i=0;i<MINION_TOTAL;i++)
    {
        _fbxAni[MINION][i] = new FBX;
        sprintf_s(fbxPath, sizeof(fbxPath), "Resource/FBX/Minion/%d.fbx", i);
        _fbxAni[MINION][i]->init(_direct->getDevice(), fbxPath);
    }
    // 마법사 애니
    for(int i=0;i<21;i++)
    {
        sprintf_s(fbxPath, sizeof(fbxPath), "Resource/FBX/Magician/idle (%d).fbx", i);
        _fbxAni[MAGICIAN][i] = new FBX;
        _fbxAni[MAGICIAN][i]->init(_direct->getDevice(), fbxPath);
    }
    for(int i=21;i<42;i++)
    {
        sprintf_s(fbxPath, sizeof(fbxPath), "Resource/FBX/Magician/move (%d).fbx", i-21);
        _fbxAni[MAGICIAN][i] = new FBX;
        _fbxAni[MAGICIAN][i]->init(_direct->getDevice(), fbxPath);
    }
    for(int i=42;i<63;i++)
    {
        sprintf_s(fbxPath, sizeof(fbxPath), "Resource/FBX/Magician/attack (%d).fbx", i-42);
        _fbxAni[MAGICIAN][i] = new FBX;
        _fbxAni[MAGICIAN][i]->init(_direct->getDevice(), fbxPath);
    }
    for(int i=63;i<74;i++)
    {
        sprintf_s(fbxPath, sizeof(fbxPath), "Resource/FBX/Magician/flash (%d).fbx", i-63);
        _fbxAni[MAGICIAN][i] = new FBX;
        _fbxAni[MAGICIAN][i]->init(_direct->getDevice(), fbxPath);
    }

}
void FBXManager::render(int index)
{
    _fbx[index]->render(_direct->getDeviceContext());
}
int  FBXManager::getIndexCount(int index)
{
    return _fbx[index]->getIndexCount();
}
//▼나중에 삭제할 함수
void FBXManager::renderAni(int index, int frame)
{
    _fbxAni[index][frame]->render(_direct->getDeviceContext());
}