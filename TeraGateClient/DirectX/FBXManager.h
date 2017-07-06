#pragma once
#include"Direct.h"
#include"FBX.h"
#include"AmbientLightShader.h"
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
    ID3D11ShaderResourceView *getTexture(int index);
public:
    enum FBXIndex
    {
        GROUND, WALL, SKY, GATE, STONE, MINION, PC, WORM, TOTAL
    };
protected:
    HWND    _hwnd;
    Direct* _direct;
    FBX*    _fbx[TOTAL];
    AmbientLightShader* _ambientLightShader;
};