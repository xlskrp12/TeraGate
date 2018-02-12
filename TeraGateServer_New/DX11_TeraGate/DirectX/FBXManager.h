#pragma once
#include"Direct.h"
#include"FBX.h"
// FbxManager��� Ŭ������ fbxsdk.h�� �ֳ�����..
// ��� ������ �� ���ڰ�, �� �� FBXManager��� Ŭ������ �����ؼ� ��߰ڴ�.
// FBX�� ���� �빮������ ����ؼ� ���߿� FbxManager�� �򰥸��� �ʵ��� ������ ��..
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
    //��ִϸ��̼��� �ȵǼ� �� �ε��߱� ������ �ӽ÷� public..
    // ���߿� �ִϸ��̼� ���� �Ϻ��ϰ� �ذ�Ǹ� public�������� �ִϸ��̼� ó���� �ϴ� ������ ������ ��
    enum FBXFrame
    {
        MAGICIAN_IDLE_A   = 0,
        MAGICIAN_IDLE_Z   = 20,
        MAGICIAN_MOVE_A   = 21,
        MAGICIAN_MOVE_Z   = 41,
        MAGICIAN_ATTACK_A = 45,//������ 42�ε� �ʹ� ����ؼ� ª�� ������.
        MAGICIAN_ATTACK_Z = 55,//������ 62�ε� �ʹ� ����ؼ� ª�� ������.
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