#pragma once
#include"Direct.h"
#include"FBX.h"
// FbxManager라는 클래스가 fbxsdk.h에 있나보다..
// 어떻게 쓰는진 잘 몰겠고, 난 걍 FBXManager라는 클래스를 정의해서 써야겠다.
// FBX가 전부 대문자임을 기억해서 나중에 FbxManager랑 헷갈리지 않도록 주의할 것..
class FBXManager
{
private:
    FBXManager();
    static FBXManager* _singleton;
public:
    static FBXManager* getSingleton();
public:
    void                      init(HWND hwnd, Direct* direct);
    void                      render(int fbxIndex);
    int                       getIndexCount(int index);
public:
    enum FBXIndex
    {
        SLASH,
        HP_GREEN,
        UI_ETC,
        UI_HP_BAR,
        UI_MINIMAP,
        UI_MINIMAP_MARK,
        UI_MINIMAP_MARK_GATE,
        GROUND, WALL, SKY, SKY_IRON, MAIN_GATE, GATE, GATE_CORE, STONE, STONE_TREE, TREE, MINION, MAGICIAN, WORM, LOGO, EFFECT1, EFFECT2, SHURIKEN,
        INDEX_TOTAL
    };
    //▼애니메이션이 안되서 깡 로드했기 때문에 임시로 public..
    // 나중에 애니메이션 문제 완벽하게 해결되면 public영역에서 애니메이션 처리를 하는 변수들 삭제할 것
    enum FBXFrame
    {
        MAGICIAN_IDLE_A   = 0,
        MAGICIAN_IDLE_Z   = 20,
        MAGICIAN_MOVE_A   = 21,
        MAGICIAN_MOVE_Z   = 41,
        MAGICIAN_ATTACK_A = 45,//원래는 42인데 너무 답답해서 짧게 설정함.
        MAGICIAN_ATTACK_Z = 55,//원래는 62인데 너무 답답해서 짧게 설정함.
        MAGICIAN_FLASH_A  = 63,
        MAGICIAN_FLASH_Z  = 74,
        MAGICIAN_TOTAL    = 74,


        MINION_MOVE_A   = 0,
        MINION_MOVE_Z   = 9,
        MINION_ATTACK_A = 10,
        MINION_ATTACK_Z = 39,
        MINION_TOTAL    = 40,
        FRAME_TOTAL     = 100
    };
    FBX*    _fbxAni[INDEX_TOTAL][FRAME_TOTAL];
    void renderAni(int index, int frame);
protected:
    HWND    _hwnd;
    Direct* _direct;
    FBX*    _fbx[INDEX_TOTAL];
};