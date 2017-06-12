#pragma once
#include"Direct.h"
#include"FBX.h"
#include"AmbientLightShader.h"
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