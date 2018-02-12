#include "TextureManager.h"
//¡å½Ì±ÛÅæ
TextureManager::TextureManager(){}
TextureManager* TextureManager::_singleton = NULL;
TextureManager* TextureManager::getSingleton()
{
    if(_singleton == NULL)
        _singleton = new TextureManager;
    return _singleton;
}
TextureManager::~TextureManager()
{
}
//¡åÀÎÅÍÆäÀÌ½º
void TextureManager::init(Direct* direct)
{
    _direct = direct;

    setTexture(SLASH, L"Resource/DDS/slash.dds");
    setTexture(COLOR_RED, L"Resource/DDS/colorRed.dds");
    setTexture(COLOR_BLUE, L"Resource/DDS/colorBlue.dds");
    setTexture(COLOR_YELLOW, L"Resource/DDS/colorYellow.dds");
    setTexture(HP_GREEN, L"Resource/DDS/hpGreen.dds");
    setTexture(UI_HP_BAR, L"Resource/DDS/uiHPBar.dds");
    setTexture(UI_PROFILE, L"Resource/DDS/uiProfile.dds");
    setTexture(UI_MINIMAP, L"Resource/DDS/uiMinimap.dds");
    setTexture(GROUND, L"Resource/DDS/Ground.dds");
    setTexture(WALL, L"Resource/DDS/wall.dds");
    setTexture(SKY, L"Resource/DDS/skybox.dds");
    setTexture(IRON, L"Resource/DDS/iron.dds");
    setTexture(GATE_RED, L"Resource/DDS/gateRed.dds");
    setTexture(GATE_BLUE, L"Resource/DDS/gateBlue.dds");
    setTexture(GATE_GREEN, L"Resource/DDS/gateGreen.dds");
    setTexture(GATE_BLACK, L"Resource/DDS/gateBlack.dds");
    setTexture(GATE_CORE, L"Resource/DDS/gateCore.dds");
    setTexture(STONE, L"Resource/DDS/stone.dds");
    setTexture(STONE_TREE, L"Resource/DDS/stoneTree.dds");
    setTexture(MAGICIAN_RED, L"Resource/DDS/magicianRed.dds");
    setTexture(MAGICIAN_BLUE, L"Resource/DDS/magicianBlue.dds");
    setTexture(MAGICIAN_GREEN, L"Resource/DDS/magicianGreen.dds");
    setTexture(MAGICIAN_BLACK, L"Resource/DDS/magicianBlack.dds");
    setTexture(MINION_RED, L"Resource/DDS/minionRed.dds");
    setTexture(MINION_BLUE, L"Resource/DDS/minionBlue.dds");
    setTexture(MINION_GREEN, L"Resource/DDS/minionGreen.dds");
    setTexture(MINION_BLACK, L"Resource/DDS/minionBlack.dds");
    setTexture(WORM,L"Resource/DDS/worm.dds");
    setTexture(LOGO,L"Resource/DDS/logo.dds");
    setTexture(EFFECT1,L"Resource/DDS/effect1.dds");
    setTexture(EFFECT2,L"Resource/DDS/effect2.dds");
    setTexture(SHURIKEN,L"Resource/DDS/shuriken.dds");
}
ID3D11ShaderResourceView* TextureManager::getTexture(int index)
{
    return _texture[index].getTexture();
}
void TextureManager::setTexture(int index, WCHAR *ddsFilename)
{
    _texture[index].init(_direct->getDevice(), ddsFilename);
}
