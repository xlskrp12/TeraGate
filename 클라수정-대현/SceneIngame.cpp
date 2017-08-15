#include "SceneIngame.h"

//-----------------------------------------------------
SceneIngame* SceneIngame::instance = nullptr;
//-----------------------------------------------------

SceneIngame::SceneIngame()
{
    init();
}
SceneIngame::~SceneIngame()
{
    release();
}
void SceneIngame::enterScene()
{
    _state = Scene::STATE_INGAME;
}
void SceneIngame::init()
{
    enterScene();

    //▼카메라
    _camera = new CameraIngame;
    if (!_camera) MessageBox(NULL, L"new Camera", L"Error", NULL);
    _camera->setPos(D3DXVECTOR3(0, 0, -1250));
    _camera->setRot(D3DXVECTOR3(45, 0, 0));


    _cameraPC = new CameraIngamePC;
    if (!_camera) MessageBox(NULL, L"new Camera", L"Error", NULL);
    _camera->setPos(D3DXVECTOR3(0, 0, -1250));
    _camera->setRot(D3DXVECTOR3(45, 0, 0));

    //▼FBXManager
    FBXManager::getSingleton();


    //▼셰이더
    _shader = new AmbientLightShader;
    if (!_shader) MessageBox(NULL, L"new LightShader", L"Error", NULL);
    _shader->init(Direct::getSingleton()->getDevice(), Direct::getSingleton()->getHWND());
    
    //▼조명
    _light = new Light;
    if (!_light) MessageBox(NULL, L"new Light", L"Error", NULL);
    _light->setDiffuseColor(0.75f, 0.75f, 0.75f, 1.0f);
    _light->setAmbientColor(0.25f, 0.25f, 0.25f, 1.0f);
    _light->setDirection(-1.f, -1.f, -1.f);

    //▼스태틱 매니저----------------------------------------------------------------------------------
    Object* object;
    // 스카이박스 * 1
    object = new ObjectStatic();
    object->setPos(0, 0, 0);
    _staticManager.addObject(object);

    // 땅 * 1
    object= new ObjectStatic();
    object->setPos(0, 0, 0);
    _staticManager.addObject(object);

    // 벽 * 1
    object= new ObjectStatic();
    object->setPos(0, 0, 0);
    _staticManager.addObject(object);
    double radius = 1800;
    double radian = 0;
    // 위에까진 3개
    // 바깥->안쪽1 나무 147그루
    for (int i = 0; i < 147; i++)
    {
        object = new ObjectStatic();
        radius = 3700+RANDOM_M(0, 50);
        radian = DEGREE_TO_RADIAN(RANDOM_M(0, 360));
        object->setPos(radius*cos(radian), RANDOM_M(-60, 0), radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        _staticManager.addObject(object);
    }
    // 바깥->안쪽2 나무 1350그루
    for(int a=0;a<9;a++)
    {
        for (int b = 0; b < 150; b++)
        {
            object = new ObjectStatic();
            radius = 3600-a*50+RANDOM_M(0, 100);
            radian = DEGREE_TO_RADIAN(RANDOM_M(0, 360));
            object->setPos(radius*cos(radian), RANDOM_M(-60, 0), radius*sin(radian));
            object->setRotY(RANDOM_M(0, 360));
            _staticManager.addObject(object);
        }
    }
    //▼오브젝트 매니저----------------------------------------------------------------------------------
    // 게이트 * 13
    // 중앙게이트
    radius = 3000;
    /*object = new ObjectGate();
    object->setPos(0, 0, 0);
    _objectManager.addObject(object);*/
	towerObject[0] = new ObjectGate();
	towerObject[0]->_exist = false;
	towerObject[0]->setPos(0, 0, 0);
	towerObject[0]->setSize(100);
	_objectManager.addObject(towerObject[0]);


    // NPC게이트
    /*object = new ObjectGate();
    object->setPos(radius*cos(DEGREE_TO_RADIAN(90)), 0, radius*sin(DEGREE_TO_RADIAN(90)));
    _objectManager.addObject(object);

	object = new ObjectGate();
	object->setPos(radius*cos(DEGREE_TO_RADIAN(90))/2, 0, radius*sin(DEGREE_TO_RADIAN(90))/2);
	_objectManager.addObject(object);

	object = new ObjectGate();
	object->setPos(radius*cos(DEGREE_TO_RADIAN(50)), 0, radius*sin(DEGREE_TO_RADIAN(50)));
	_objectManager.addObject(object);

	object = new ObjectGate();
	object->setPos(radius*cos(DEGREE_TO_RADIAN(130)), 0, radius*sin(DEGREE_TO_RADIAN(130)));
	_objectManager.addObject(object);
	*/
	towerObject[1] = new ObjectGate();
	towerObject[1]->_exist = false;
	towerObject[1]->setPos(radius*cos(DEGREE_TO_RADIAN(90)), 0, radius*sin(DEGREE_TO_RADIAN(90)));
	towerObject[1]->setSize(100);
	_objectManager.addObject(towerObject[1]);

	towerObject[2] = new ObjectGate();
	towerObject[2]->_exist = false;
	towerObject[2]->setPos(radius*cos(DEGREE_TO_RADIAN(90)) / 2, 0, radius*sin(DEGREE_TO_RADIAN(90)) / 2);
	towerObject[2]->setSize(100);
	_objectManager.addObject(towerObject[2]);

	towerObject[3] = new ObjectGate();
	towerObject[3]->_exist = false;
	towerObject[3]->setPos(radius*cos(DEGREE_TO_RADIAN(50)), 0, radius*sin(DEGREE_TO_RADIAN(50)));
	towerObject[3]->setSize(100);
	_objectManager.addObject(towerObject[3]);

	towerObject[4] = new ObjectGate();
	towerObject[4]->_exist = false;
	towerObject[4]->setPos(radius*cos(DEGREE_TO_RADIAN(130)), 0, radius*sin(DEGREE_TO_RADIAN(130)));
	towerObject[4]->setSize(100);
	_objectManager.addObject(towerObject[4]);

    



    // 왼쪽팀 게이트
    /*object = new ObjectGate();
    object->setPos(radius*cos(DEGREE_TO_RADIAN(210)), 0, radius*sin(DEGREE_TO_RADIAN(210)));
    _objectManager.addObject(object);

    object = new ObjectGate();
    object->setPos(radius*cos(DEGREE_TO_RADIAN(210))/2, 0, radius*sin(DEGREE_TO_RADIAN(210))/2);
    _objectManager.addObject(object);

    object = new ObjectGate();
    object->setPos(radius*cos(DEGREE_TO_RADIAN(170)), 0, radius*sin(DEGREE_TO_RADIAN(170)));
    _objectManager.addObject(object);

    object = new ObjectGate();
    object->setPos(radius*cos(DEGREE_TO_RADIAN(250)), 0, radius*sin(DEGREE_TO_RADIAN(250)));
    _objectManager.addObject(object);*/

	towerObject[5] = new ObjectGate();
	towerObject[5]->_exist = false;
	towerObject[5]->setPos(radius*cos(DEGREE_TO_RADIAN(210)), 0, radius*sin(DEGREE_TO_RADIAN(210)));
	towerObject[5]->setSize(100);
	_objectManager.addObject(towerObject[5]);

	towerObject[6] = new ObjectGate();
	towerObject[6]->_exist = false;
	towerObject[6]->setPos(radius*cos(DEGREE_TO_RADIAN(210)) / 2, 0, radius*sin(DEGREE_TO_RADIAN(210)) / 2);
	towerObject[6]->setSize(100);
	_objectManager.addObject(towerObject[6]);

	towerObject[7] = new ObjectGate();
	towerObject[7]->_exist = false;
	towerObject[7]->setPos(radius*cos(DEGREE_TO_RADIAN(170)), 0, radius*sin(DEGREE_TO_RADIAN(170)));
	towerObject[7]->setSize(100);
	_objectManager.addObject(towerObject[7]);

	towerObject[8] = new ObjectGate();
	towerObject[8]->_exist = false;
	towerObject[8]->setPos(radius*cos(DEGREE_TO_RADIAN(250)), 0, radius*sin(DEGREE_TO_RADIAN(250)));
	towerObject[8]->setSize(100);
	_objectManager.addObject(towerObject[8]);

    // 오른쪽팀 게이트
    /*object = new ObjectGate();
    object->setPos(radius*cos(DEGREE_TO_RADIAN(-30)), 0, radius*sin(DEGREE_TO_RADIAN(-30)));
    _objectManager.addObject(object);

    object = new ObjectGate();
    object->setPos(radius*cos(DEGREE_TO_RADIAN(-30))/2, 0, radius*sin(DEGREE_TO_RADIAN(-30))/2);
    _objectManager.addObject(object);

    object = new ObjectGate();
    object->setPos(radius*cos(DEGREE_TO_RADIAN(10)), 0, radius*sin(DEGREE_TO_RADIAN(10)));
    _objectManager.addObject(object);

    object = new ObjectGate();
    object->setPos(radius*cos(DEGREE_TO_RADIAN(-70)), 0, radius*sin(DEGREE_TO_RADIAN(-70)));
    _objectManager.addObject(object);*/

	towerObject[9] = new ObjectGate();
	towerObject[9]->_exist = false;
	towerObject[9]->setPos(radius*cos(DEGREE_TO_RADIAN(-30)), 0, radius*sin(DEGREE_TO_RADIAN(-30)));
	towerObject[9]->setSize(100);
	_objectManager.addObject(towerObject[9]);

	towerObject[10] = new ObjectGate();
	towerObject[10]->_exist = false;
	towerObject[10]->setPos(radius*cos(DEGREE_TO_RADIAN(-30)) / 2, 0, radius*sin(DEGREE_TO_RADIAN(-30)) / 2);
	towerObject[10]->setSize(100);
	_objectManager.addObject(towerObject[10]);

	towerObject[11] = new ObjectGate();
	towerObject[11]->_exist = false;
	towerObject[11]->setPos(radius*cos(DEGREE_TO_RADIAN(10)), 0, radius*sin(DEGREE_TO_RADIAN(10)));
	towerObject[11]->setSize(100);
	_objectManager.addObject(towerObject[11]);

	towerObject[12] = new ObjectGate();
	towerObject[12]->_exist = false;
	towerObject[12]->setPos(radius*cos(DEGREE_TO_RADIAN(-70)), 0, radius*sin(DEGREE_TO_RADIAN(-70)));
	towerObject[12]->setSize(100);
	_objectManager.addObject(towerObject[12]);

    // 게이트 사이즈
   /* for(int i=0;i<13;i++)
    {
        _objectManager._object[i]->setSize(100);
    }*/
    // 바위 * 7
    for(int i=0;i<3;i++)
    {
        object= new ObjectStatic();
        radius = 1500;
        radian = DEGREE_TO_RADIAN(10+i*20);//10~50도 
        object->setPos(radius*cos(radian), RANDOM_M(-50, 0), radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        object->setSize(300);
        _objectManager.addObject(object);
    }
    for(int i=0;i<4;i++)
    {
        object= new ObjectStatic();
        radius = 1700;
        radian = DEGREE_TO_RADIAN(i*20);//0~60도 
        object->setPos(radius*cos(radian), RANDOM_M(-50, 0), radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        object->setSize(300);
        _objectManager.addObject(object);
    }
    // 바위 * 7
    for(int i=0;i<3;i++)
    {
        object= new ObjectStatic();
        radius = 1500;
        radian = DEGREE_TO_RADIAN(-70-i*20);//-70~-110도 
        object->setPos(radius*cos(radian), RANDOM_M(-50, 0), radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        object->setSize(300);
        _objectManager.addObject(object);
    }
    for(int i=0;i<4;i++)
    {
        object= new ObjectStatic();
        radius = 1700;
        radian = DEGREE_TO_RADIAN(-60-i*20);//-60~-120도 
        object->setPos(radius*cos(radian), RANDOM_M(-50, 0), radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        object->setSize(300);
        _objectManager.addObject(object);
    }
    // 바위 * 7
    for(int i=0;i<3;i++)
    {
        object = new ObjectStatic();
        radius = 1500;
        radian = DEGREE_TO_RADIAN(130+i*20);//130~170도
        object->setPos(radius*cos(radian), RANDOM_M(-50, 0), radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        object->setSize(300);
        _objectManager.addObject(object);
    }
    for(int i=0;i<4;i++)
    {
        object= new ObjectStatic();
        radius = 1700;
        radian = DEGREE_TO_RADIAN(120+i*20);//-60~-120도 
        object->setPos(radius*cos(radian), RANDOM_M(-50, 0), radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        object->setSize(300);
        _objectManager.addObject(object);
    }


    // PC 왼쪽 팀 3
    /*object = new ObjectPC();
    object->setPos(0, 0, -100);
    _objectManager.addObject(object);

    object = new ObjectPC();
    object->setPos(0, 0, -100);
    _objectManager.addObject(object);

    object = new ObjectPC();
    object->setPos(0, 0, -100);
    _objectManager.addObject(object);

    // PC 오른쪽 팀 3
    object = new ObjectPC();
    object->setPos(0, 0, -100);
    _objectManager.addObject(object);

    object = new ObjectPC();
    object->setPos(0, 0, -100);
    _objectManager.addObject(object);

    object = new ObjectPC();
    object->setPos(0, 0, -100);
    _objectManager.addObject(object);*/
	for (int i = 0; i < MAX_USER; i++)
	{
		playerObject[i] = new ObjectPC();
		playerObject[i]->_exist = false;
		playerObject[i]->setPos(0, 0, -10000);
		_objectManager.addObject(playerObject[i]);
	}

    // NPC * 4
    /*object = new ObjectPC();
    object->setPos(0, 0, -100);
    _objectManager.addObject(object);

    object = new ObjectPC();
    object->setPos(0, 0, -100);
    _objectManager.addObject(object);

    object = new ObjectPC();
    object->setPos(0, 0, -100);
    _objectManager.addObject(object);

    object = new ObjectPC();
    object->setPos(0, 0, -100);
    _objectManager.addObject(object);*/
	for (int i = 0; i < NUM_NPC_PLAYER; i++)
	{
		NPCObject[i] = new ObjectPC();
		NPCObject[i]->_exist = false;
		double radius = 400 + RANDOM_M(100, 600);
		double radian = DEGREE_TO_RADIAN(RANDOM_M(0, 360));
		NPCObject[i]->setPos(radius*cos(radian), 0, radius*sin(radian));
		NPCObject[i]->setRotY(RANDOM_M(0, 360));
		_objectManager.addObject(NPCObject[i]);
	}
    // 미니언
    /*for (int i = 0; i < 90; i++)
    {
        object= new ObjectMinion();
        double radius = 400+RANDOM_M(100, 600);
        double radian = DEGREE_TO_RADIAN(RANDOM_M(0, 360));
        object->setPos(radius*cos(radian), 0, radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        _objectManager.addObject(object);
    }*/
	for (int i = NUM_NPC_PLAYER; i < NUM_OF_NPC - NPC_START + NUM_NPC_PLAYER; i++)
	{
		NPCObject[i] = new ObjectMinion();
		NPCObject[i]->_exist = false;
		double radius = 400 + RANDOM_M(100, 600);
		double radian = DEGREE_TO_RADIAN(RANDOM_M(0, 360));
		NPCObject[i]->setPos(radius*cos(radian), 0, radius*sin(radian));
		NPCObject[i]->setRotY(RANDOM_M(0, 360));
		_objectManager.addObject(NPCObject[i]);
	}

}
void SceneIngame::release()
{
    if (_light)
    {
        delete _light;
        _light = NULL;
    }
    if (_shader)
    {
        _shader->release();
        delete _shader;
        _shader = NULL;
    }
    if (_camera)
    {
        delete _camera;
        _camera = NULL;
    }
    if (_cameraPC)
    {
        delete _cameraPC;
        _cameraPC = NULL;
    }
}
void SceneIngame::proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{

}
void SceneIngame::update()
{
    if (GetKeyState(VK_F1) & 0x8000)
    {
        _state = Scene::STATE_INTRO;
        return;
    }

//--------------------------------------------------------
	for (int i = 0; i < MAX_USER; i++)
	{
		if (g_myid == i)
		{
			if (true == _ServerDef->player.exist)
			{
				playerObject[i]->_exist = true;
				playerObject[i]->setPos(_ServerDef->player.x, _ServerDef->player.y, _ServerDef->player.z);
				playerObject[i]->setRot(0, _ServerDef->player.roty, 0);
				//std::cout << "player" << " exist" << std::endl;
				//std::cout << _ServerDef->player.x << " , " << _ServerDef->player.z << std::endl;
			}
		}
		else
		{
			if (true == _ServerDef->otherPC[i].exist)
			{
				playerObject[i]->_exist = true;
				playerObject[i]->setPos(_ServerDef->otherPC[i].x,
					_ServerDef->otherPC[i].y, _ServerDef->otherPC[i].z);
				playerObject[i]->setRot(0, _ServerDef->otherPC[i].roty, 0);
				//std::cout <<"other player " << i << " exist" << std::endl;
			}
			else
			{
				playerObject[i]->_exist = false;
			}
		}
	}

	for (int i = 0; i < NUM_OF_NPC - NPC_START + NUM_NPC_PLAYER; i++)
	{
		if (true == _ServerDef->NPC[i].exist)
		{
			NPCObject[i]->_exist = true;
			NPCObject[i]->setPos(_ServerDef->NPC[i].x,
				_ServerDef->NPC[i].y, _ServerDef->NPC[i].z);
			NPCObject[i]->setRot(0, _ServerDef->NPC[i].roty, 0);
		}
	}

	for (int i = 0; i < NUM_OF_TOWER; i++)
	{
		if (true == _ServerDef->towerData[i].exist)
		{
			towerObject[i]->_exist = true;
			towerObject[i]->setPos(_ServerDef->towerData[i].x, 
				_ServerDef->towerData[i].y, _ServerDef->towerData[i].z);
		}
	}
//--------------------------------------------------------


    // _staticManager.update();
    _objectManager.updateCollision();


    //_camera->update();
    _cameraPC->setDes(_objectManager._object[g_myid]->getPos());
    _cameraPC->setRot(D3DXVECTOR3(45, 90, 0));
    _cameraPC->update();
}
void SceneIngame::render()
{
    // 셰이더에서 필요한 것들 한 번씩만 초기화
    _shader->setMtxCameraView(_cameraPC->getMtxView());
    _shader->setMtxProjection();
    _shader->setLight(_light);

    //▼_staticManager
    // 스카이박스
    _shader->target(FBXManager::SKY, FBXManager::SKY);
    FBXManager::getSingleton()->render(FBXManager::SKY);
    _shader->render(_staticManager.getWorldMatrix(0));
    // 땅
    FBXManager::getSingleton()->render(FBXManager::GROUND);
    _shader->target(FBXManager::GROUND, FBXManager::GROUND);
    _shader->render(_staticManager.getWorldMatrix(1));
    // 벽
    FBXManager::getSingleton()->render(FBXManager::WALL);
    _shader->target(FBXManager::WALL, FBXManager::WALL);
    _shader->render(_staticManager.getWorldMatrix(2));
    //바깥->안쪽 나무
    FBXManager::getSingleton()->render(FBXManager::TREE);
    _shader->target(FBXManager::TREE, FBXManager::TREE);
    for(int i=3;i<1500;i++)
    {
        _shader->render(_staticManager.getWorldMatrix(i));
    }


//--------------------------------------------------------
    //▼_objectManager
    // 게이트
    FBXManager::getSingleton()->render(FBXManager::GATE);
    _shader->target(FBXManager::GATE, FBXManager::GATE);
    for(int i=0;i<13;i++)
    {
		if (true == towerObject[i]->_exist)
		{
			_shader->render(_objectManager.getWorldMatrix(i));
		}
    }
    //// 스톤
    FBXManager::getSingleton()->render(FBXManager::STONE);
    _shader->target(FBXManager::STONE, FBXManager::STONE);
    for(int i=13;i<34;i++)
    {
        _shader->render(_objectManager.getWorldMatrix(i));
    }
    // pc
    // fbx, direct, objectManager, camera, light
    FBXManager::getSingleton()->render(FBXManager::PC);
    _shader->target(FBXManager::PC, FBXManager::PC);
    /*for(int i=34;i<44;i++)
    {
        _shader->render(_objectManager.getWorldMatrix(i));
    }*/
	for (int i = 34; i < 34 + MAX_USER + NUM_NPC_PLAYER; i++)
	{
		if (MAX_USER > i - 34)
		{
			if (true == playerObject[i - 34]->_exist)
				_shader->render(_objectManager.getWorldMatrix(i));
			//std::cout << "player render" << std::endl;
		}
		else
		{
			if (true == NPCObject[i - 34 - MAX_USER]->_exist)
				_shader->render(_objectManager.getWorldMatrix(i));
		}
			
	}

    //임시 미니언
    FBXManager::getSingleton()->render(FBXManager::MINION);
    _shader->target(FBXManager::MINION, FBXManager::MINION);
    /*for(int i=44;i<134;i++)
    {
        _shader->render(_objectManager.getWorldMatrix(i));
    }*/
	for (int i = 44; i < 44 + NUM_OF_NPC - NPC_START; i++)
	{
		if (true == NPCObject[i - 44 + NUM_NPC_PLAYER]->_exist)
			_shader->render(_objectManager.getWorldMatrix(i));
	}
//--------------------------------------------------------

}