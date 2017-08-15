#pragma once
#include "Scene.h"
class SceneIngame : public Scene
{
public:
//-----------------------------------------------------
	static SceneIngame* instance;

	static SceneIngame* GetInstance()
	{
		if (nullptr == instance)
			return instance = new SceneIngame;

		return instance;
	}

	Object* playerObject[MAX_USER];
	Object* NPCObject[NUM_OF_NPC - NPC_START + NUM_NPC_PLAYER];
	Object* towerObject[NUM_OF_TOWER];

//-----------------------------------------------------

    SceneIngame();
    ~SceneIngame();
    void enterScene();
    void init();
    void release();
    void proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
    void update();
    void render();
protected:
    ObjectManager      _staticManager;// 업데이트할 필요가 없는 친구들
    ObjectManager      _objectManager;
    Camera            *_camera;
    Camera            *_cameraPC;
    AmbientLightShader*_shader;
    Light             *_light;
protected:
    const static int L_TEAM_PC_MAX = 3;
    const static int R_TEAM_PC_MAX = 3;
    const static int N_TEAM_PC_MAX = 4;
    const static int L_MINION_MAX  = 60;
    const static int R_MINION_MAX  = 60;
    const static int N_MINION_MAX  = 60;
};

