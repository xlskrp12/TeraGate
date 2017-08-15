#pragma once
#include"Prev.h"
// ¾À
#include"SceneIngame.h"
#include"SceneIntro.h"
//--------------------------------------------------------------------------
#include"Keyboard.h"
//--------------------------------------------------------------------------

class SceneManager
{
public:
    SceneManager();
    SceneManager(const SceneManager&);
    ~SceneManager();

	//void init(HWND, int, int);
	//--------------------------------------------------------------------------
    void init(HWND, int, int, Keyboard*);
	//--------------------------------------------------------------------------
    void release();
    void render();
    void update();
public:
private:
    Scene*_scene[Scene::STATE_VOLUME];
    int   _state = Scene::STATE_INTRO;

	//--------------------------------------------------------------------------
	Keyboard* _keyboard;
	//--------------------------------------------------------------------------

};