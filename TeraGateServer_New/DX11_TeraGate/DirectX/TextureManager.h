#pragma once
#include"Prev.h"
#include"Direct.h"
#include"Texture.h"
class TextureManager
{
//°ÂΩÃ±€≈Ê
private:
    TextureManager();
    static TextureManager* _singleton;
public:
    static TextureManager* getSingleton();
//°Â¿Œ≈Õ∆‰¿ÃΩ∫
public:
    enum TextureIndex
    {
        SLASH,
        COLOR_RED,
        COLOR_BLUE,
        COLOR_YELLOW,
        HP_GREEN,
        UI_HP_BAR,
        UI_PROFILE,
        UI_MINIMAP,
        GROUND,
        WALL,
        SKY,
        IRON,
        GATE_RED, GATE_BLUE, GATE_GREEN, GATE_BLACK,
        GATE_CORE,
        STONE,
        STONE_TREE,
        TREE,
        MAGICIAN_RED, MAGICIAN_BLUE, MAGICIAN_GREEN, MAGICIAN_BLACK,
        MINION_RED,   MINION_BLUE,   MINION_GREEN,   MINION_BLACK,
        WORM,
        LOGO,
        EFFECT1,
        EFFECT2,
        SHURIKEN,
        INDEX_TOTAL
    };
    ~TextureManager();
    void init(Direct* direct);
    void setTexture(int index, WCHAR *ddsFilename);
    ID3D11ShaderResourceView *getTexture(int index);
private:
    Direct* _direct;
    Texture _texture[100];
};

