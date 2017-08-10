#pragma once
#include"Prev.h"
// ¾À
#include"SceneIngame.h"
#include"SceneIntro.h"
class SceneManager
{
public:
    SceneManager();
    SceneManager(const SceneManager&);
    ~SceneManager();

    void init(HWND, int, int);
    void release();
    void render();
    void update();
public:
    const static int STATE_INGAME = 0;
    const static int STATE_INTRO  = 1;
    const static int STATE_VOLUME = 2;
private:
    Scene             *_scene[STATE_VOLUME];
};