#pragma once
#include"Prev.h"
#include"Direct.h"
#include"Camera.h"
#include"Model.h"
#include"FBX.h"
#include"TextureShader.h"
#include"LightShader.h"
#include"AmbientLightShader.h"
#include"Light.h"
#include"ObjectManager.h"
//#include"Server.h"
class Renderer
{
public:
    Renderer();
    Renderer(const Renderer&);
    ~Renderer();

    void init(HWND, int, int);
    void release();
    void render();
    void update();

	//Server *_server;
	Object* playerObject[MAX_USER];
	Object* NPCObject[NUM_OF_NPC];

	static Renderer* instance;

	static Renderer* GetInstance()
	{
		if (nullptr == instance)
			return instance = new Renderer;

		return instance;
	}
private:
    Direct            *_direct;
    Camera            *_camera;
    FBX               *_fbx[7];
    AmbientLightShader*_shader;
    Light             *_light;
    ObjectManager      _objectManager;
	Object* object;
};

#define _RendererDef		Renderer::GetInstance()