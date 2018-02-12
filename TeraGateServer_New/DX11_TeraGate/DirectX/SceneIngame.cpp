#include "SceneIngame.h"
SceneIngame::SceneIngame()
{
    _camera = NULL;
    _shader = NULL;
    _ui     = NULL;
    init();
}
SceneIngame::~SceneIngame()
{
    release();
}
void SceneIngame::enterScene()
{
    _state = Scene::STATE_INGAME;
    // 미니언 생성 타이머
    for(int i=0;i<4;i++)
    {
        _minionTimer[i].setCooltime(8);
        _minionTimer[i].setDelay(i);
    }
    // 브금 bgm 배경음
    PlaySound(L"Resource/Sound/bgmUntitled.wav", NULL, SND_ASYNC | SND_LOOP);
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
    _cameraPC->setRot(D3DXVECTOR3(45, 90, 0));

    //▼FBXManager
    FBXManager::getSingleton();


    //▼셰이더
    _shader = new AmbientLightShader;
    if (!_shader) MessageBox(NULL, L"new AmbientLightShader", L"Error", NULL);
    _shader->init(Direct::getSingleton()->getDevice(), Direct::getSingleton()->getHWND());

    //▼UI
    _ui = new UI;
    if(!_ui) MessageBox(NULL, L"new UI", L"Error", NULL);
    _ui->init(
        Direct::getSingleton()->getDevice(),
        CLIENT_WIDTH, CLIENT_HEIGHT,
        L"Resource/DDS/uiMinimap.dds",
        250, 250);//크기. dds사이즈랑 일치할 필요없이 자유롭게 바꿀 수 있음.



    //▼조명
    _light = new Light;
    if (!_light) MessageBox(NULL, L"new Light", L"Error", NULL);
    _light->setDiffuseColor(0.75f, 0.75f, 0.75f, 1.0f);
    _light->setAmbientColor(0.25f, 0.25f, 0.25f, 1.0f);
    _light->setDirection(-1.f, -1.f, -1.f);

    //▼UI 매니저----------------------------------------------------------------------------------
    Object* object;
    // 0 : 상단 타이머
    // 1 : 하단 좌측 
    // 2 : 하단
    // 3 : 하단 우측(미니맵)
    for(int i=0;i<4;i++)
    {
        object = new ObjectStatic();
        object->setPos(0, 0, 0);
        _uiManager.addObject(object);
    }
    // 4 : 플레이어 체력바 
    // 5 : 플레이어 발치 아우라
    for(int i=0;i<6;i++)
    {
        object = new ObjectStatic();
        object->setPos(0, 0, 0);
        _uiManager.addObject(object);
    }
    //▼스태틱 매니저----------------------------------------------------------------------------------
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
    object->setPos(0, 0.1, 0);
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
    // 그라운드 바깥 공중을 떠다니는 게이트 코어들
    for(int a=0;a<100;a++)
    {       
        object = new ObjectGateCore();
        radius = 5000+RANDOM_M(0, 5000);
        radian = DEGREE_TO_RADIAN(RANDOM_M(0, 360));
        object->setPos(radius*cos(radian), RANDOM_M(-1000, -500), radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        _staticManager.addObject(object);
    }
    for(int a=0;a<100;a++)
    {
        object = new ObjectGateCore();
        radius = 5000+RANDOM_M(0, 5000);
        radian = DEGREE_TO_RADIAN(RANDOM_M(0, 360));
        object->setPos(radius*cos(radian), RANDOM_M(-500, 500), radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        _staticManager.addObject(object);
    }
    for(int a=0;a<100;a++)
    {       
        object = new ObjectGateCore();
        radius = 5000+RANDOM_M(0, 5000);
        radian = DEGREE_TO_RADIAN(RANDOM_M(0, 360));
        object->setPos(radius*cos(radian), RANDOM_M(500, 1000), radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        _staticManager.addObject(object);
    }
    //▼오브젝트 매니저----------------------------------------------------------------------------------
    // 게이트 * 13
    // 중앙게이트 (처음엔 NPC게이트)
    radius = 3000;
    object = new ObjectGate();
    object->respawn();
    object->setTeamID(Object::TEAM_N);
    object->setGateID(Object::GATE_C);
    object->setPos(0, 0, 0);
    _objectManager.addObject(object);

    // NPC게이트
    object = new ObjectGate();
    object->respawn();
    object->setTeamID(Object::TEAM_N);
    object->setGateID(Object::GATE_N);
    object->setPos(radius*cos(DEGREE_TO_RADIAN(90)), 0, radius*sin(DEGREE_TO_RADIAN(90)));
    _objectManager.addObject(object);

    object = new ObjectGate();
    object->respawn();
    object->setTeamID(Object::TEAM_N);
    object->setGateID(Object::GATE_NC);
    object->setPos(radius*cos(DEGREE_TO_RADIAN(90))/2, 0, radius*sin(DEGREE_TO_RADIAN(90))/2);
    _objectManager.addObject(object);

    object = new ObjectGate();
    object->respawn();
    object->setTeamID(Object::TEAM_N);
    object->setGateID(Object::GATE_NR);
    object->setPos(radius*cos(DEGREE_TO_RADIAN(50)), 0, radius*sin(DEGREE_TO_RADIAN(50)));
    _objectManager.addObject(object);

    object = new ObjectGate();
    object->respawn();
    object->setTeamID(Object::TEAM_N);
    object->setGateID(Object::GATE_NL);
    object->setPos(radius*cos(DEGREE_TO_RADIAN(130)), 0, radius*sin(DEGREE_TO_RADIAN(130)));
    _objectManager.addObject(object);

    // 왼쪽팀 게이트
    object = new ObjectGate();
    object->respawn();
    object->setTeamID(Object::TEAM_L);
    object->setGateID(Object::GATE_L);
    object->setPos(radius*cos(DEGREE_TO_RADIAN(210)), 0, radius*sin(DEGREE_TO_RADIAN(210)));
    _objectManager.addObject(object);

    object = new ObjectGate();
    object->respawn();
    object->setTeamID(Object::TEAM_L);
    object->setGateID(Object::GATE_LC);
    object->setPos(radius*cos(DEGREE_TO_RADIAN(210))/2, 0, radius*sin(DEGREE_TO_RADIAN(210))/2);
    _objectManager.addObject(object);

    object = new ObjectGate();
    object->respawn();
    object->setTeamID(Object::TEAM_L);
    object->setGateID(Object::GATE_LN);
    object->setPos(radius*cos(DEGREE_TO_RADIAN(170)), 0, radius*sin(DEGREE_TO_RADIAN(170)));
    _objectManager.addObject(object);

    object = new ObjectGate();
    object->respawn();
    object->setTeamID(Object::TEAM_L);
    object->setGateID(Object::GATE_LR);
    object->setPos(radius*cos(DEGREE_TO_RADIAN(250)), 0, radius*sin(DEGREE_TO_RADIAN(250)));
    _objectManager.addObject(object);

    // 오른쪽팀 게이트
    object = new ObjectGate();
    object->respawn();
    object->setTeamID(Object::TEAM_R);
    object->setGateID(Object::GATE_R);
    object->setPos(radius*cos(DEGREE_TO_RADIAN(-30)), 0, radius*sin(DEGREE_TO_RADIAN(-30)));
    _objectManager.addObject(object);

    object = new ObjectGate();
    object->respawn();
    object->setTeamID(Object::TEAM_R);
    object->setGateID(Object::GATE_RC);
    object->setPos(radius*cos(DEGREE_TO_RADIAN(-30))/2, 0, radius*sin(DEGREE_TO_RADIAN(-30))/2);
    _objectManager.addObject(object);

    object = new ObjectGate();
    object->respawn();
    object->setTeamID(Object::TEAM_R);
    object->setGateID(Object::GATE_RN);
    object->setPos(radius*cos(DEGREE_TO_RADIAN(10)), 0, radius*sin(DEGREE_TO_RADIAN(10)));
    _objectManager.addObject(object);

    object = new ObjectGate();
    object->respawn();
    object->setTeamID(Object::TEAM_R);
    object->setGateID(Object::GATE_RL);
    object->setPos(radius*cos(DEGREE_TO_RADIAN(-70)), 0, radius*sin(DEGREE_TO_RADIAN(-70)));
    _objectManager.addObject(object);

    // 게이트 사이즈
    for(int i=0;i<13;i++)
    {
        _objectManager._object[i]->setSize(70);
    }
    // 바위 * 9
    for(int i=0;i<4;i++)
    {
        object= new ObjectStatic();
        radius = 1500;
        radian = DEGREE_TO_RADIAN(10+i*16);//10~50도 
        object->setPos(radius*cos(radian), RANDOM_M(-50, 0), radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        object->setSize(280);
        _objectManager.addObject(object);
    }
    for(int i=0;i<5;i++)
    {
        object= new ObjectStatic();
        radius = 1700;
        radian = DEGREE_TO_RADIAN(i*15);//0~60도 
        object->setPos(radius*cos(radian), RANDOM_M(-50, 0), radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        object->setSize(280);
        _objectManager.addObject(object);
    }
    // 바위 * 9
    for(int i=0;i<4;i++)
    {
        object= new ObjectStatic();
        radius = 1500;
        radian = DEGREE_TO_RADIAN(-70-i*16);//-70~-110도 
        object->setPos(radius*cos(radian), RANDOM_M(-50, 0), radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        object->setSize(280);
        _objectManager.addObject(object);
    }
    for(int i=0;i<5;i++)
    {
        object= new ObjectStatic();
        radius = 1700;
        radian = DEGREE_TO_RADIAN(-60-i*15);//-60~-120도 
        object->setPos(radius*cos(radian), RANDOM_M(-50, 0), radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        object->setSize(280);
        _objectManager.addObject(object);
    }
    // 바위 * 9
    for(int i=0;i<4;i++)
    {
        object = new ObjectStatic();
        radius = 1500;
        radian = DEGREE_TO_RADIAN(130+i*16);//130~170도
        object->setPos(radius*cos(radian), RANDOM_M(-50, 0), radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        object->setSize(280);
        _objectManager.addObject(object);
    }
    for(int i=0;i<5;i++)
    {
        object= new ObjectStatic();
        radius = 1700;
        radian = DEGREE_TO_RADIAN(120+i*15);//-60~-120도 
        object->setPos(radius*cos(radian), RANDOM_M(-50, 0), radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        object->setSize(280);
        _objectManager.addObject(object);
    }


    radius = 2800;
    // PC 왼쪽 팀 L
    object = new ObjectMagician();
    radian = DEGREE_TO_RADIAN(210);
    object->setPos(radius*cos(radian), 0, radius*sin(radian));
    object->setTeamID(Object::TEAM_L);
    _objectManager.addObject(object);

    object = new ObjectMagician();
    radian = DEGREE_TO_RADIAN(210);
    object->setPos(radius*cos(radian), 0, radius*sin(radian));
    object->setTeamID(Object::TEAM_L);
    _objectManager.addObject(object);

    object = new ObjectMagician();
    radian = DEGREE_TO_RADIAN(210);
    object->setPos(radius*cos(radian), 0, radius*sin(radian));
    object->setTeamID(Object::TEAM_L);
    _objectManager.addObject(object);

    // PC 오른쪽 팀 R
    object = new ObjectMagician();
    radian = DEGREE_TO_RADIAN(-30);
    object->setPos(radius*cos(radian), 0, radius*sin(radian)); 
    object->setTeamID(Object::TEAM_R);
    _objectManager.addObject(object);

    object = new ObjectMagician();
    radian = DEGREE_TO_RADIAN(-30);
    object->setPos(radius*cos(radian), 0, radius*sin(radian)); 
    object->setTeamID(Object::TEAM_R);
    _objectManager.addObject(object);

    object = new ObjectMagician();
    radian = DEGREE_TO_RADIAN(-30);
    object->setPos(radius*cos(radian), 0, radius*sin(radian)); 
    object->setTeamID(Object::TEAM_R);
    _objectManager.addObject(object);

    // NPC 윗쪽 팀 N
    object = new ObjectMagician();
    radian = DEGREE_TO_RADIAN(90);
    object->setPos(radius*cos(radian), 0, radius*sin(radian)); 
    object->setTeamID(Object::TEAM_N);
    _objectManager.addObject(object);

    object = new ObjectMagician();
    radian = DEGREE_TO_RADIAN(90);
    object->setPos(radius*cos(radian), 0, radius*sin(radian)); 
    object->setTeamID(Object::TEAM_N);
    _objectManager.addObject(object);

    object = new ObjectMagician();
    radian = DEGREE_TO_RADIAN(90);
    object->setPos(radius*cos(radian), 0, radius*sin(radian)); 
    object->setTeamID(Object::TEAM_N);
    _objectManager.addObject(object);

    object = new ObjectMagician();
    radian = DEGREE_TO_RADIAN(90);
    object->setPos(radius*cos(radian), 0, radius*sin(radian)); 
    object->setTeamID(Object::TEAM_N);
    _objectManager.addObject(object);

    //▼미니언 30 / 30 / 30 / 30
    radius=2800;
    radian=0;
    //L팀
    for (int i = 0; i < 30; i++)
    {
        object = new ObjectMinion();
        radian = DEGREE_TO_RADIAN(210);
        object->setPos(radius*cos(radian), 0, radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        object->setTeamID(Object::TEAM_L);
        object->setGateID(Object::GATE_LC);
        _objectManager.addObject(object);
    }
    for (int i = 0; i < 30; i++)
    {
        object = new ObjectMinion();
        radian = DEGREE_TO_RADIAN(215);
        object->setPos(radius*cos(radian), 0, radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        object->setTeamID(Object::TEAM_L);
        object->setGateID(Object::GATE_LR);
        _objectManager.addObject(object);
    }
    for (int i = 0; i < 30; i++)
    {
        object = new ObjectMinion();
        radian = DEGREE_TO_RADIAN(205);
        object->setPos(radius*cos(radian), 0, radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        object->setTeamID(Object::TEAM_L);
        object->setGateID(Object::GATE_LN);
        _objectManager.addObject(object);
    }
    //R팀
    for (int i = 0; i < 30; i++)
    {
        object = new ObjectMinion();
        radian = DEGREE_TO_RADIAN(-30);
        object->setPos(radius*cos(radian), 0, radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        object->setTeamID(Object::TEAM_R);
        object->setGateID(Object::GATE_RC);
        _objectManager.addObject(object);
    }
    for (int i = 0; i < 30; i++)
    {
        object = new ObjectMinion();
        radian = DEGREE_TO_RADIAN(-35);
        object->setPos(radius*cos(radian), 0, radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        object->setTeamID(Object::TEAM_R);
        object->setGateID(Object::GATE_RL);
        _objectManager.addObject(object);
    }
    for (int i = 0; i < 30; i++)
    {
        object = new ObjectMinion();
        radian = DEGREE_TO_RADIAN(-25);
        object->setPos(radius*cos(radian), 0, radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        object->setTeamID(Object::TEAM_R);
        object->setGateID(Object::GATE_RN);
        _objectManager.addObject(object);
    }
    //N팀
    for (int i = 0; i < 30; i++)
    {
        object = new ObjectMinion();
        radian = DEGREE_TO_RADIAN(90);
        object->setPos(radius*cos(radian), 0, radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        object->setTeamID(Object::TEAM_N);
        object->setGateID(Object::GATE_NC);
        _objectManager.addObject(object);
    }
    for (int i = 0; i < 30; i++)
    {
        object = new ObjectMinion();
        radian = DEGREE_TO_RADIAN(95);
        object->setPos(radius*cos(radian), 0, radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        object->setTeamID(Object::TEAM_N);
        object->setGateID(Object::GATE_NL);
        _objectManager.addObject(object);
    }
    for (int i = 0; i < 30; i++)
    {
        object = new ObjectMinion();
        radian = DEGREE_TO_RADIAN(85);
        object->setPos(radius*cos(radian), 0, radius*sin(radian));
        object->setRotY(RANDOM_M(0, 360));
        object->setTeamID(Object::TEAM_N);
        object->setGateID(Object::GATE_NR);
        _objectManager.addObject(object);
    }
    //▼모든 오브젝트 전부 EXIST_FALSE
    //게이트 13개 바위 27개 = 총 40개는 처음부터 true
    for(int i=0;i<40;i++)
    {
        _objectManager._object[i]->setExist(true);
    }
    //게이트, 바위 외에는 모두 false
    for(int i=40;i<_objectManager.getObjectAmount();i++)
    {
        _objectManager._object[i]->setExist(false);
    }
    //PC 존재&컨트롤 플래그 ON
    


    //▼이펙트 매니저----------------------------------------------------------------------------------
    // 마법사 이펙트 (캐릭당 최대 10개)
    for(int i=0;i<30;i++)
    {
        object = new ObjectShuriken;
        object->setTeamID(Object::TEAM_L);
        object->setExist(false);
        _effectManager.addObject(object);
    }
    for(int i=0;i<30;i++)
    {
        object = new ObjectShuriken;
        object->setTeamID(Object::TEAM_R);
        object->setExist(false);
        _effectManager.addObject(object);
    }
    for(int i=0;i<40;i++)
    {
        object = new ObjectShuriken;
        object->setTeamID(Object::TEAM_N);
        object->setExist(false);
        _effectManager.addObject(object);
    }
    // 미니언 이펙트 (캐릭당 최대 1개)
    for(int i=0;i<90;i++)
    {
        object = new ObjectSlash;
        object->setTeamID(Object::TEAM_L);
        object->setExist(false);
        _effectManager.addObject(object);
    }
    for(int i=0;i<90;i++)
    {
        object = new ObjectSlash;
        object->setTeamID(Object::TEAM_R);
        object->setExist(false);
        _effectManager.addObject(object);
    }
    for(int i=0;i<90;i++)
    {
        object = new ObjectSlash;
        object->setTeamID(Object::TEAM_N);
        object->setExist(false);
        _effectManager.addObject(object);
    }
}
void SceneIngame::release()
{
    if (_ui)
    {
        delete _ui;
        _ui = NULL;
    }
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
//    SoundManager::getSingleton()->update();
    if (GetKeyState(VK_BACK) & 0x8000)
    {
        _state = Scene::STATE_INTRO;
        return;
    }
    _staticManager.updateCommon();

    //▼오브젝트 매니저
    for(int i=0;i<13;i++)
        _objectManager.updateCommon(i);
    _objectManager.updateCollision();

	//_objectManager._object[_ServerDef->player.id + 40]->setExist(true);
	//_objectManager._object[_ServerDef->player.id + 40]->setControl(true);

    if(_objectManager._object[_ServerDef->player.id+40]->getExist() == false)
    {
        _objectManager._object[_ServerDef->player.id+40]->respawn();
        _objectManager._object[_ServerDef->player.id+40]->setPos(_objectManager._object[5]->getPos());
    }

	if (true == _ServerDef->player.exist)
	{
		//std::cout << _ServerDef->player.id << std::endl;
		if (_ServerDef->player.id % 2 == 0)
		{
			_objectManager._object[_ServerDef->player.id + 40]->setExist(true);
			_objectManager._object[_ServerDef->player.id + 40]->setControl(true);
			_objectManager._object[_ServerDef->player.id + 40]->setPos(_ServerDef->player.x,
				_ServerDef->player.y, _ServerDef->player.z);
			_objectManager._object[_ServerDef->player.id + 40]->setRotY(_ServerDef->player.roty);
			_objectManager._object[_ServerDef->player.teamId + 40]->setTeamID(_ServerDef->player.teamId);
		}
		else
		{
			_objectManager._object[_ServerDef->player.id + 43]->setExist(true);
			_objectManager._object[_ServerDef->player.id + 43]->setControl(true);
			_objectManager._object[_ServerDef->player.id + 43]->setPos(_ServerDef->player.x,
				_ServerDef->player.y, _ServerDef->player.z);
			_objectManager._object[_ServerDef->player.id + 43]->setRotY(_ServerDef->player.roty);
			_objectManager._object[_ServerDef->player.teamId + 43]->setTeamID(_ServerDef->player.teamId);
		}
	}

	for (int i = 0; i < MAX_USER; i++)
	{
		if (_ServerDef->player.id == i)
			continue;
		if (true == _ServerDef->other_pc[i].exist)
		{
			if (i % 2 == 0)
			{
				_objectManager._object[i + 40]->setExist(true);
				_objectManager._object[i + 40]->setPos(_ServerDef->other_pc[i].x,
					_ServerDef->other_pc[i].y, _ServerDef->other_pc[i].z);
				_objectManager._object[i + 40]->setRotY(_ServerDef->other_pc[i].roty);
				_objectManager._object[i + 40]->setTeamID(_ServerDef->other_pc[i].teamId);
				_objectManager._object[i + 40]->setSize(30.0f);
			}
			else
			{
				_objectManager._object[i + 43]->setExist(true);
				_objectManager._object[i + 43]->setPos(_ServerDef->other_pc[i].x,
					_ServerDef->other_pc[i].y, _ServerDef->other_pc[i].z);
				_objectManager._object[i + 43]->setRotY(_ServerDef->other_pc[i].roty);
				_objectManager._object[i + 43]->setTeamID(_ServerDef->other_pc[i].teamId);
				_objectManager._object[i + 43]->setSize(30.0f);
			}
		}
	}

    // 게이트가 가진 맵 상태 업데이트
    for(int g=0;g<13;g++)
        for(int gg=0;gg<13;gg++)
            _objectManager._object[g]->updateMap(g, _objectManager._object[gg]->getTeamID());
    // 오브젝트 매니저 - 미니언 생성
    for(int t=0;t<2;t++)
    if(_minionTimer[t].isOn())
    {
        if(t==0)
            SoundManager::getSingleton()->_announcer.play(1);
        float radius=2800;
        float radian=0;

        int teamID;
        for(int g=0;g<13;g++)
        {
            if(g!=1 || g!= 5 || g!= 9)
            {
                teamID = _objectManager._object[g]->getTeamID();
                for(int i=50;i<320;i++)
                    if(_objectManager._object[i]->getExist() == false)
                    {
                        _objectManager._object[i]->setExist(true);
                        _objectManager._object[i]->respawn();
                        radian = DEGREE_TO_RADIAN(-30);
                        _objectManager._object[i]->setPos(_objectManager._object[g]->getPos());
                        _objectManager._object[i]->move(RANDOM_M(-10, 10), 0, RANDOM_M(-10, 10));
                        _objectManager._object[i]->setTeamID(teamID);
                        _objectManager._object[i]->setGateID(Object::GATE_RC);
                        break;
                    }
            }
        }

        // L팀 미니언 생성
        for(int i=50;i<140;i++)
        {
            if(false == _objectManager._object[i]->getExist())
            {
                _objectManager._object[i]->setExist(true);
                _objectManager._object[i]->respawn();
                radian = DEGREE_TO_RADIAN(210);
                _objectManager._object[i]->setPos(radius*cos(radian), 0, radius*sin(radian));
                _objectManager._object[i]->setRotY(RANDOM_M(0, 360));                
                _objectManager._object[i]->setTeamID(_objectManager._object[5]->getTeamID());
                _objectManager._object[i]->setGateID(Object::GATE_LC);
                break;
            }
        }
        for(int i=50;i<140;i++)
        {
            if(false == _objectManager._object[i]->getExist())
            {
                _objectManager._object[i]->setExist(true);
                _objectManager._object[i]->respawn();
                radian = DEGREE_TO_RADIAN(215);
                _objectManager._object[i]->setPos(radius*cos(radian), 0, radius*sin(radian));
                _objectManager._object[i]->setRotY(RANDOM_M(0, 360));
                _objectManager._object[i]->setTeamID(_objectManager._object[5]->getTeamID());
                _objectManager._object[i]->setGateID(Object::GATE_LR);                
                break;
            }
        }
        for(int i=50;i<140;i++)
        {
            if(false == _objectManager._object[i]->getExist())
            {
                _objectManager._object[i]->setExist(true);
                _objectManager._object[i]->respawn();
                radian = DEGREE_TO_RADIAN(205);
                _objectManager._object[i]->setPos(radius*cos(radian), 0, radius*sin(radian));
                _objectManager._object[i]->setRotY(RANDOM_M(0, 360));
                _objectManager._object[i]->setTeamID(_objectManager._object[5]->getTeamID());
                _objectManager._object[i]->setGateID(Object::GATE_LN);
                break;
            }
        }
        // R팀 미니언 생성
        for(int i=140;i<170;i++)
            if(false == _objectManager._object[i]->getExist())
            {
                _objectManager._object[i]->setExist(true);
                _objectManager._object[i]->respawn();
                radian = DEGREE_TO_RADIAN(-30);
                _objectManager._object[i]->setPos(radius*cos(radian), 0, radius*sin(radian));
                _objectManager._object[i]->setRotY(RANDOM_M(0, 360));
                _objectManager._object[i]->setTeamID(_objectManager._object[9]->getTeamID());
                _objectManager._object[i]->setGateID(Object::GATE_RC);
                break;
            }
        for(int i=170;i<200;i++)
            if(false == _objectManager._object[i]->getExist())
            {
                _objectManager._object[i]->setExist(true);
                _objectManager._object[i]->respawn();
                radian = DEGREE_TO_RADIAN(-35);
                _objectManager._object[i]->setPos(radius*cos(radian), 0, radius*sin(radian));
                _objectManager._object[i]->setRotY(RANDOM_M(0, 360));
                _objectManager._object[i]->setTeamID(_objectManager._object[9]->getTeamID());
                _objectManager._object[i]->setGateID(Object::GATE_RL);
                break;
            }
        for(int i=200;i<230;i++)
            if(false == _objectManager._object[i]->getExist())
            {
                _objectManager._object[i]->setExist(true);
                _objectManager._object[i]->respawn();
                radian = DEGREE_TO_RADIAN(-25);
                _objectManager._object[i]->setPos(radius*cos(radian), 0, radius*sin(radian));
                _objectManager._object[i]->setRotY(RANDOM_M(0, 360));
                _objectManager._object[i]->setTeamID(_objectManager._object[9]->getTeamID());
                _objectManager._object[i]->setGateID(Object::GATE_RN);
                break;
            }
        // N팀
        for(int i=230;i<260;i++)
            if(false == _objectManager._object[i]->getExist())
            {
                _objectManager._object[i]->setExist(true);
                _objectManager._object[i]->respawn();
                radian = DEGREE_TO_RADIAN(90);
                _objectManager._object[i]->setPos(radius*cos(radian), 0, radius*sin(radian));
                _objectManager._object[i]->setRotY(RANDOM_M(0, 360));
                _objectManager._object[i]->setTeamID(_objectManager._object[1]->getTeamID());
                _objectManager._object[i]->setGateID(Object::GATE_NC);
                break;
            }
        for(int i=260;i<290;i++)
            if(false == _objectManager._object[i]->getExist())
            {
                _objectManager._object[i]->setExist(true);
                _objectManager._object[i]->respawn();
                radian = DEGREE_TO_RADIAN(95);
                _objectManager._object[i]->setPos(radius*cos(radian), 0, radius*sin(radian));
                _objectManager._object[i]->setRotY(RANDOM_M(0, 360));
                _objectManager._object[i]->setTeamID(_objectManager._object[1]->getTeamID());
                _objectManager._object[i]->setGateID(Object::GATE_NL);
                break;
            }
        for(int i=290;i<320;i++)
            if(false == _objectManager._object[i]->getExist())
            {
                _objectManager._object[i]->setExist(true);
                _objectManager._object[i]->respawn();
                radian = DEGREE_TO_RADIAN(85);
                _objectManager._object[i]->setPos(radius*cos(radian), 0, radius*sin(radian));
                _objectManager._object[i]->setRotY(RANDOM_M(0, 360));
                _objectManager._object[i]->setTeamID(_objectManager._object[1]->getTeamID());
                _objectManager._object[i]->setGateID(Object::GATE_NR);
                break;
            }
    }
    // 미니언 공격프레임 시작시 슬래시 오브젝트
    for(int o=50;o<320;o++)
    {
        if(_objectManager._object[o]->getFrame() == FBXManager::MINION_ATTACK_A)
        for(int e=0;e<_effectManager._object.size();e++)
        {
            if(_effectManager._object[e]->getID() == Object::ID_SLASH)
            if(_effectManager._object[e]->getExist() == false)
            {
                _effectManager._object[e]->setExist(true);
                _effectManager._object[e]->setTeamID(_objectManager._object[o]->getTeamID());
                _effectManager._object[e]->setPos(_objectManager._object[o]->getPos());
                _effectManager._object[e]->setRot(_objectManager._object[o]->getRot());
                _effectManager._object[e]->rotZ(90);
                _effectManager._object[e]->setPosAway(60.0f);
            }
        }       
    }
    // 미니언 적 인식 후 공격for
    for(int a=50;a<320;a++)
    {
        // 공격 : 미니언▶마법사
        for(int b=40;b<50;b++)
        {
            if(a != b)
            if(_objectManager._object[a]->getExist())
            if(_objectManager._object[b]->getExist())
            if(_objectManager._object[a]->getTeamID() != _objectManager._object[b]->getTeamID())
            {
                if(_objectManager._object[a]->isInSight(*_objectManager._object[b]))
                {
                    //_objectManager._object[a]->setDesTarget(_objectManager._object[b]->getPos());
                    _objectManager._object[a]->setDes(_objectManager._object[b]->getPos());
                    if(_objectManager._object[a]->getState() != Object::STATE_ATTACK)
                        _objectManager._object[a]->setState(Object::STATE_ATTACK);
                    // 인식시 공격
                    if(FBXManager::MINION_ATTACK_A+10 <= _objectManager._object[a]->getFrame() && _objectManager._object[a]->getFrame() <= FBXManager::MINION_ATTACK_A+16)
                    {
                        _objectManager._object[a]->attack(*_objectManager._object[b]);
                    }
                }
            }
        }//공격for()
        // 공격 : 미니언▶게이트
        for(int b=0;b<13;b++)
        {
            if(a != b)
            if(_objectManager._object[a]->getExist())
            if(_objectManager._object[b]->getExist())
            if(_objectManager._object[a]->getTeamID() != _objectManager._object[b]->getTeamID())
            {
                if(_objectManager._object[a]->isInSight(*_objectManager._object[b]))
                {
                    //_objectManager._object[a]->setDesTarget(_objectManager._object[b]->getPos());
                    _objectManager._object[a]->setDes(_objectManager._object[b]->getPos());
                    if(_objectManager._object[a]->getState() != Object::STATE_ATTACK)
                        _objectManager._object[a]->setState(Object::STATE_ATTACK);
                    // 인식시 공격
                    if(FBXManager::MINION_ATTACK_A+10 <= _objectManager._object[a]->getFrame() && _objectManager._object[a]->getFrame() <= FBXManager::MINION_ATTACK_A+16)
                    {
                        _objectManager._object[a]->attack(*_objectManager._object[b]);
                        if(_objectManager._object[b]->getHP() <= 0)
                        {
                            SoundManager::getSingleton()->_gate.play(1);
                            _objectManager._object[b]->setTeamID(_objectManager._object[a]->getTeamID());
                            _objectManager._object[b]->respawn();
                        }

                    }
                }
            }
        }// 공격for()
        // 공격 : 미니언▶미니언
        for(int b=50;b<320;b++)
        {
            if(a != b)
            if(_objectManager._object[a]->getExist())
            if(_objectManager._object[b]->getExist())
            if(_objectManager._object[a]->getTeamID() != _objectManager._object[b]->getTeamID())
            {
                if(_objectManager._object[a]->isInSight(*_objectManager._object[b]))
                {
                    //_objectManager._object[a]->setDesTarget(_objectManager._object[b]->getPos());
                    _objectManager._object[a]->setDes(_objectManager._object[b]->getPos());
                    if(_objectManager._object[a]->getState() != Object::STATE_ATTACK)
                    {
                        _objectManager._object[a]->setState(Object::STATE_ATTACK);

                    }
                    // 인식시 공격
                    if(FBXManager::MINION_ATTACK_A+10 <= _objectManager._object[a]->getFrame() && _objectManager._object[a]->getFrame() <= FBXManager::MINION_ATTACK_A+16)
                    {
                        _objectManager._object[a]->attack(*_objectManager._object[b]);
                    }
                }
            }
        }// 공격for()
    }// 미니언 적 인식 후 공격for
    for(int o=50;o<320;o++)
    {
        if(_objectManager._object[o]->getExist() == true)
        if(_objectManager._object[o]->getFrame() == FBXManager::MINION_ATTACK_A)
        if(getDistanceXZ(_objectManager._object[40]->getPos(), _objectManager._object[o]->getPos())<200)
        {
            SoundManager::getSingleton()->_minion.play(RANDOM_M(1, 4));
        }

    }


    //▼이펙트 매니저
    // PC마법사 공격
    if(_objectManager._object[40]->getExist() == true)
    if( _objectManager._object[40]->getFrame() == FBXManager::MAGICIAN_ATTACK_A+1  ||
        _objectManager._object[40]->getFrame() == FBXManager::MAGICIAN_ATTACK_A+3  ||
        _objectManager._object[40]->getFrame() == FBXManager::MAGICIAN_ATTACK_A+6 ||
        _objectManager._object[40]->getFrame() == FBXManager::MAGICIAN_ATTACK_A+9)
    {
        if(_objectManager._object[40]->getFrame() == FBXManager::MAGICIAN_ATTACK_A+1)
            SoundManager::getSingleton()->_magician.play(RANDOM_M(1, 3));
        for(int rot=0;rot<5;rot++)
        for(int i=0;i<_effectManager._object.size();i++)
        {
            if(_effectManager._object[i]->getID() == Object::ID_SHURIKEN)
            if(_effectManager._object[i]->getExist() == false)
            {
                {
                    _effectManager._object[i]->setExist(true);
                    _effectManager._object[i]->respawn();
                    _effectManager._object[i]->setTeamID(Object::TEAM_L);
                    _effectManager._object[i]->setPos(_objectManager._object[40]->getPos());
                    _effectManager._object[i]->setRot(_objectManager._object[40]->getRot());
                    _effectManager._object[i]->rotY(rot*10 - 20);
                    _effectManager._object[i]->setPosAway(60.0f);
                }
                break;
            }
        }
    }

    for(int i=0;i<370;i++)
        if(_effectManager._object[i]->getExist())
            _effectManager._object[i]->update();


    //▼오브젝트&이펙트 충돌체크
    // 슈리켄이 튕길 때 자연스러운 회전
    D3DXVECTOR3 ePosBeforeCollision;
    D3DXVECTOR3 ePosAfterCollision;
    D3DXVECTOR3 ePos;
    D3DXVECTOR3 oPos;
    float distance;
    float sizeDistance;
    float rotation;
    bool isCollision = false;
    for(int e=0;e<100;e++)
    {
        isCollision = false;
        ePosBeforeCollision = _effectManager._object[e]->getPos();
        if(_effectManager._object[e]->getExist() == true)
        for(int o=0;o<_objectManager._object.size();o++)
        {
            // 충돌시
            if(_objectManager._object[o]->getExist() == true)
            if(_effectManager._object[e]->getTeamID() != _objectManager._object[o]->getTeamID())
            if(_effectManager._object[e]->isCollision(*_objectManager._object[o]))
            {
                // 자연스러운 회전
                ePos         = _effectManager._object[e]->getPos();
                oPos         = _objectManager._object[o]->getPos();
                distance     = sqrt(pow(oPos.x-ePos.x, 2)+pow(oPos.z-ePos.z, 2));
                sizeDistance = _effectManager._object[e]->getSize()+_objectManager._object[o]->getSize();
                if(distance<sizeDistance)
                {
                    _effectManager._object[e]->setPosX(oPos.x + sizeDistance*cos(atan2(ePos.z-oPos.z, ePos.x-oPos.x)));
                    _effectManager._object[e]->setPosZ(oPos.z + sizeDistance*sin(atan2(ePos.z-oPos.z, ePos.x-oPos.x)));
                    ePosAfterCollision = _effectManager._object[e]->getPos();
                    rotation = RADIAN_TO_DEGREE(atan2(ePosAfterCollision.z-ePosBeforeCollision.z, ePosAfterCollision.x-ePosBeforeCollision.x));
                    _effectManager._object[e]->setRotY(rotation);
                    _effectManager._object[e]->damage(1);
                    //if(_effectManager._object[e]->getHP()<=0)
                    //    _effectManager._object[e]->setExist(false);
                }
                // 게이트 공격
                isCollision = true;
                _effectManager._object[e]->attack(*_objectManager._object[o]);
                if(_objectManager._object[o]->getID() == Object::ID_GATE && _objectManager._object[o]->getHP() <= 0)
                {
                    SoundManager::getSingleton()->_gate.play(1);
                    _objectManager._object[o]->respawn();
                    _objectManager._object[o]->setTeamID(_effectManager._object[e]->getTeamID());

                }
            }
        }
    }

    for(int i=0;i<_objectManager._object.size();i++)
    {
        // 미니언 죽었을 때 효과음
        if(_objectManager._object[i]->getExist())
        if(_objectManager._object[i]->getHP()<=0)
        if(getDistanceXZ(_objectManager._object[40]->getPos(), _objectManager._object[i]->getPos())<600)
        {
            _objectManager._object[i]->setExist(false);
            SoundManager::getSingleton()->_minion.play(RANDOM_M(5, 7));
        }
    }
    for(int i=13;i<40;i++)
    {
        _objectManager._object[i]->setHP(100000);
        _objectManager._object[i]->setExist(true);
    }
    //▼승리조건
    static bool isVictory = false;
    if(_objectManager._object[1]->getTeamID() == Object::TEAM_L)
    if(_objectManager._object[5]->getTeamID() == Object::TEAM_L)
    if(_objectManager._object[9]->getTeamID() == Object::TEAM_L)
        isVictory=true;
    if(GetKeyState(VK_F6) & 0x8000)
        isVictory = true;
    if(isVictory == true)
    {
        static int shakeCount = 0;
        if(shakeCount==40)
        {
            SoundManager::getSingleton()->_gate.play(2);
        }
        if(shakeCount==80)
        {
            SoundManager::getSingleton()->_announcer.play(2);
        }
        if(shakeCount<=180)
        {
            shakeCount++;
            _cameraPC->setDes(D3DXVECTOR3(0, 0, 0));
            _cameraPC->rot((float)RANDOM_M(-50, 50)/240, (float)RANDOM_M(-50, 50)/240, (float)RANDOM_M(-50, 50)/240);
        }
        else
        {
            MessageBox(NULL, L"모든 본진게이트를 점령하여 승리하셨습니다.", L"Victory", NULL);
            exit(0);
        }
    }
    //▼카메라
    if(isVictory == false)
    {
		if(_ServerDef->player.id%2==0)
			_cameraPC->setDes(_objectManager._object[_ServerDef->player.id + 40]->getPos());
		else
			_cameraPC->setDes(_objectManager._object[_ServerDef->player.id + 43]->getPos());

		_cameraPC->update();
    }
}
void SceneIngame::render()
{
//    _cameraPC->setRot(0, 90, 0);
    // 셰이더에서 필요한 것들 한 번씩만 초기화
    _shader->setMtxCameraView(_cameraPC->getMtxView());
    _shader->setMtxProjection();

    // UI 라이트 설정
    _light->setDiffuseColor(0.75f, 0.75f, 0.75f, 1.0f);
    _light->setAmbientColor(0.25f, 0.25f, 0.25f, 1.0f);
    _light->setDirection(0.0f, 0.0f, 1.f);
    _shader->setLight(_light);

    //▼_uiManager
    // 메인UI
    for(int i=0;i<5;i++)
    {
        _uiManager._object[i]->setRot(0, -90, 0);
        _uiManager._object[i]->setPos(_cameraPC->getPosition());
        _uiManager._object[i]->rot(-45, 0, 0);
        _uiManager._object[i]->move(0, -500, 500);
    }
    // 상단 중앙 타임바
    _uiManager._object[0]->setRot(-60+20, -90, 0);
    _uiManager._object[0]->setPos(_cameraPC->getPosition());
    _uiManager._object[0]->move(0, -160, 300);

    // 하단 좌측
    _uiManager._object[1]->setRot(-60, -90, -10);
    _uiManager._object[1]->setPos(_cameraPC->getPosition());
    _uiManager._object[1]->move(-190, -300, 110);

    // 하단 중앙
    _uiManager._object[2]->setRot(-60, -90, 0);
    _uiManager._object[2]->setPos(_cameraPC->getPosition());
    _uiManager._object[2]->move(0, -300, 100);

    // 하단 우측 미니맵
    _uiManager._object[3]->setRot(-60, -90, 0);
    _uiManager._object[3]->setPos(_cameraPC->getPosition());
    _uiManager._object[3]->move(185, -300, 110);

    // 플레이어 체력바
    _uiManager._object[4]->setPos(_objectManager._object[40]->getPos());
    _uiManager._object[4]->setRot(-45, -90, 0);
    _uiManager._object[4]->move(0, 300, 0);

    //printf("%.2f %.2f %.2f\n", _uiManager._object[0]->getPosX(), _uiManager._object[0]->getPosY(), _uiManager._object[0]->getPosZ());
    //printf("%.2f %.2f %.2f\n\n", _uiManager._object[0]->getRotX(), _uiManager._object[0]->getRotY(), _uiManager._object[0]->getRotZ());
    for(int i=1;i<5;i++)
    {
        if(i==2)
            continue;
        _shader->target(FBXManager::UI_MINIMAP, TextureManager::WALL);
        FBXManager::getSingleton()->render(FBXManager::UI_MINIMAP);
        if(i==1)
        {
            _shader->target(FBXManager::UI_MINIMAP, TextureManager::UI_PROFILE);
            FBXManager::getSingleton()->render(FBXManager::UI_MINIMAP);
        }
        if(i==3)
        {
            _shader->target(FBXManager::UI_MINIMAP, TextureManager::UI_MINIMAP);
            FBXManager::getSingleton()->render(FBXManager::UI_MINIMAP);
        }
        if(i==4)
        {
            _shader->target(FBXManager::UI_HP_BAR, TextureManager::UI_HP_BAR);
            FBXManager::getSingleton()->render(FBXManager::UI_HP_BAR);
        }
        _shader->render(_uiManager.getWorldMatrix(i));
    }
    // 플레이어 초록색 체력칸
    _uiManager._object[4]->setPos(_objectManager._object[_ServerDef->player.id+40]->getPos());
    _uiManager._object[4]->setRot(-45, -90, 0);
    _uiManager._object[4]->move(-30, 300, 0);
    for(int i=0;i<_objectManager._object[40]->getHP()/4;i++)
    {
        _uiManager._object[4]->move(1.2f, 0, 0);
        _shader->target(FBXManager::HP_GREEN, TextureManager::HP_GREEN);
        FBXManager::getSingleton()->render(FBXManager::HP_GREEN);
        _shader->render(_uiManager.getWorldMatrix(4));
    }


    for(int gate=0;gate<13;gate++)
    {
        // 게이트 체력바
        _uiManager._object[4]->setPos(_objectManager._object[gate]->getPos());
        _uiManager._object[4]->setRot(-45, -90, 0);
        _uiManager._object[4]->move(0, 350, 0);
        _shader->target(FBXManager::UI_HP_BAR, TextureManager::UI_HP_BAR);
        FBXManager::getSingleton()->render(FBXManager::UI_HP_BAR);
        _shader->render(_uiManager.getWorldMatrix(4));
        // 게이트 초록색 체력칸
        _uiManager._object[4]->setPos(_objectManager._object[gate]->getPos());
        _uiManager._object[4]->setRot(-45, -90, 0);
        _uiManager._object[4]->move(-30, 350, 0);
        for(int i=0;i<_objectManager._object[gate]->getHP()/80;i++)
        {
            _uiManager._object[4]->move(1.2f, 0, 0);
            _shader->target(FBXManager::HP_GREEN, TextureManager::HP_GREEN);
            FBXManager::getSingleton()->render(FBXManager::HP_GREEN);
            _shader->render(_uiManager.getWorldMatrix(4));
        }
    }



    // 미니맵 마크
    static float centerX = +128;
    static float centerY = -190;
    static float centerZ = +100;
    _uiManager._object[5]->setRot(0, -90, 0);
    static float rotY = 0;
    rotY += 4.5;
    _uiManager._object[5]->rot(-45, 0+rotY, 0+rotY);
    for(int i=0;i<13;i++){//게이트
        if(_objectManager._object[i]->getExist() == true){
            switch(_objectManager._object[i]->getTeamID()){
            case Object::TEAM_L:
                _shader->target(FBXManager::UI_MINIMAP_MARK_GATE, TextureManager::COLOR_BLUE);
                break;
            case Object::TEAM_R:
                _shader->target(FBXManager::UI_MINIMAP_MARK_GATE, TextureManager::COLOR_RED);
                break;
            case Object::TEAM_N:
                _shader->target(FBXManager::UI_MINIMAP_MARK_GATE, TextureManager::COLOR_YELLOW);
                break;
            }//switch
            _uiManager._object[5]->setPos(_cameraPC->getPosition());
            _uiManager._object[5]->move(centerX + 20*_objectManager._object[i]->getPosX()/3150 - 20*_objectManager._object[i]->getPosZ()/3150, centerY + 40*_objectManager._object[i]->getPosZ()/3150, centerZ);
            FBXManager::getSingleton()->render(FBXManager::UI_MINIMAP_MARK_GATE);
            _shader->render(_uiManager.getWorldMatrix(5));
        }//if
    }//for
    for(int i=40;i<_objectManager._object.size();i++){// 오브젝트들
        if(_objectManager._object[i]->getExist() == true){
            switch(_objectManager._object[i]->getTeamID()){
            case Object::TEAM_L:
                _shader->target(FBXManager::UI_MINIMAP_MARK, TextureManager::COLOR_BLUE);
                break;
            case Object::TEAM_R:
                _shader->target(FBXManager::UI_MINIMAP_MARK, TextureManager::COLOR_RED);
                break;
            case Object::TEAM_N:
                _shader->target(FBXManager::UI_MINIMAP_MARK, TextureManager::COLOR_YELLOW);
                break;
            }//switch
            FBXManager::getSingleton()->render(FBXManager::UI_MINIMAP_MARK);
            _uiManager._object[5]->setPos(_cameraPC->getPosition());
            _uiManager._object[5]->move(centerX + 20*_objectManager._object[i]->getPosX()/3150 - 20*_objectManager._object[i]->getPosZ()/3150, centerY + 40*_objectManager._object[i]->getPosZ()/3150, centerZ);
            _shader->render(_uiManager.getWorldMatrix(5));
        }//if
    }//for

    //// +1*x == -2*y 라고 생각하자.
    //// 중앙, 왼쪽, 오른쪽, 위, 아래 좌표
    //_uiManager._object[5]->setPos(_cameraPC->getPosition());
    //_uiManager._object[5]->move(centerX, centerY, centerZ);
    //_shader->render(_uiManager.getWorldMatrix(5));

    //_uiManager._object[5]->setPos(_cameraPC->getPosition());
    //_uiManager._object[5]->move(centerX-20, centerY, centerZ);
    //_shader->render(_uiManager.getWorldMatrix(5));

    //_uiManager._object[5]->setPos(_cameraPC->getPosition());
    //_uiManager._object[5]->move(centerX+20, centerY, centerZ);
    //_shader->render(_uiManager.getWorldMatrix(5));

    //_uiManager._object[5]->setPos(_cameraPC->getPosition());
    //_uiManager._object[5]->move(centerX+20, centerY-40, centerZ);
    //_shader->render(_uiManager.getWorldMatrix(5));

    //_uiManager._object[5]->setPos(_cameraPC->getPosition());
    //_uiManager._object[5]->move(centerX-20, centerY+40, centerZ);
    //_shader->render(_uiManager.getWorldMatrix(5));


    // 기본 라이트 설정
    _light->setDiffuseColor(0.75f, 0.75f, 0.75f, 1.0f);
    _light->setAmbientColor(0.25f, 0.25f, 0.25f, 1.0f);
    _light->setDirection(-1.f, -1.f, -1.f);
    _shader->setLight(_light);
    //▼_staticManager
    // 스카이박스
    _shader->target(FBXManager::SKY, TextureManager::SKY);
    FBXManager::getSingleton()->render(FBXManager::SKY);
    _shader->render(_staticManager.getWorldMatrix(0));
    // 땅
    FBXManager::getSingleton()->render(FBXManager::GROUND);
    _shader->target(FBXManager::GROUND, TextureManager::GROUND);
    _shader->render(_staticManager.getWorldMatrix(1));
    // 벽
    FBXManager::getSingleton()->render(FBXManager::WALL);
    _shader->target(FBXManager::WALL, TextureManager::WALL);
    _shader->render(_staticManager.getWorldMatrix(2));
    //바깥->안쪽 나무
    FBXManager::getSingleton()->render(FBXManager::TREE);
    _shader->target(FBXManager::TREE, TextureManager::STONE_TREE);
    for(int i=3;i<1500;i++)
    {
        _shader->render(_staticManager.getWorldMatrix(i));
    }

    FBXManager::getSingleton()->render(FBXManager::GATE_CORE);
    _shader->target(FBXManager::GATE_CORE, TextureManager::GATE_CORE);
    for(int i=1500;i<1800;i++)
    {
        _shader->render(_staticManager.getWorldMatrix(i));
    }

    //▼_objectManager
    // 게이트
    // 메시 준비
    for(int i=0;i<13;i++)
    {
        if(i==0 || i==1 || i==5 || i==9)
        {
            FBXManager::getSingleton()->render(FBXManager::MAIN_GATE);
            if(_objectManager._object[i]->getTeamID() == Object::TEAM_N)
                _shader->target(FBXManager::MAIN_GATE, TextureManager::GATE_GREEN);
            if(_objectManager._object[i]->getTeamID() == Object::TEAM_L)
                _shader->target(FBXManager::MAIN_GATE, TextureManager::GATE_BLUE);
            if(_objectManager._object[i]->getTeamID() == Object::TEAM_R)
                _shader->target(FBXManager::MAIN_GATE, TextureManager::GATE_RED);
        }
        else
        {
            FBXManager::getSingleton()->render(FBXManager::GATE);
            if(_objectManager._object[i]->getTeamID() == Object::TEAM_N)
                _shader->target(FBXManager::GATE, TextureManager::GATE_GREEN);
            if(_objectManager._object[i]->getTeamID() == Object::TEAM_L)
                _shader->target(FBXManager::GATE, TextureManager::GATE_BLUE);
            if(_objectManager._object[i]->getTeamID() == Object::TEAM_R)
                _shader->target(FBXManager::GATE, TextureManager::GATE_RED);
        }


        // 팀 ID에 따른 텍스쳐 준비

        // 준비된 메시&텍스쳐 렌더
        if(true == _objectManager._object[i]->getExist())
            _shader->render(_objectManager.getWorldMatrix(i));
    }
    //// 스톤
    FBXManager::getSingleton()->render(FBXManager::STONE);
    _shader->target(FBXManager::STONE, TextureManager::STONE);
    for(int i=13;i<40;i++)
    {
        _shader->render(_objectManager.getWorldMatrix(i));
    }
    // 마법사
    // fbx, direct, objectManager, camera, light
    static int magicianFrame = FBXManager::MAGICIAN_MOVE_A;
    if(FBXManager::MAGICIAN_MOVE_Z < magicianFrame)
        magicianFrame = FBXManager::MAGICIAN_MOVE_A;
    magicianFrame++;

    _shader->target(FBXManager::MAGICIAN, TextureManager::MAGICIAN_BLUE);
    for(int i=40;i<43;i++)
    {
        FBXManager::getSingleton()->renderAni(FBXManager::MAGICIAN, _objectManager._object[i]->getFrame());
        if(true == _objectManager._object[i]->getExist())
            _shader->render(_objectManager.getWorldMatrix(i));
    }
    _shader->target(FBXManager::MAGICIAN, TextureManager::MAGICIAN_RED);
    for(int i=43;i<46;i++)
    {
        if(true == _objectManager._object[i]->getExist())
            _shader->render(_objectManager.getWorldMatrix(i));
    }
    _shader->target(FBXManager::MAGICIAN, TextureManager::MAGICIAN_GREEN);
    for(int i=46;i<50;i++)
    {
        if(true == _objectManager._object[i]->getExist())
            _shader->render(_objectManager.getWorldMatrix(i));
    }

    ////미니언
    //FBXManager::getSingleton()->render(FBXManager::MINION);
    //_shader->target(FBXManager::MINION, FBXManager::MINION);
    //for(int i=44;i<164;i++)
    //{
    //    if(true == _objectManager._object[i]->getExist())
    //        _shader->render(_objectManager.getWorldMatrix(i));
    //}

    for(int i=50;i<320;i++)
    {
        switch(_objectManager._object[i]->getTeamID())
        {
        case Object::TEAM_L:
            _shader->target(FBXManager::MINION, TextureManager::MINION_BLUE);
            break;
        case Object::TEAM_R:
            _shader->target(FBXManager::MINION, TextureManager::MINION_RED);
            break;
        case Object::TEAM_N:
            _shader->target(FBXManager::MINION, TextureManager::MINION_GREEN);
            break;
        }
        if(true == _objectManager._object[i]->getExist())
        {
            FBXManager::getSingleton()->renderAni(FBXManager::MINION, _objectManager._object[i]->getFrame());
            _shader->render(_objectManager.getWorldMatrix(i));
        }
    }

    //▼이펙트 매니저
    FBXManager::getSingleton()->render(FBXManager::SHURIKEN);
    _shader->target(FBXManager::SHURIKEN, TextureManager::SHURIKEN);// 카운트 관련이기 때문에 얜 상관없음
    for(int i=0;i<100;i++)
    {
        if(true == _effectManager._object[i]->getExist())
            _shader->render(_effectManager.getWorldMatrix(i));
    }
    FBXManager::getSingleton()->render(FBXManager::SLASH);
    _shader->target(FBXManager::SLASH, TextureManager::SLASH);// 카운트 관련이기 때문에 얜 상관없음
    for(int i=100;i<370;i++)
    {
        if(true == _effectManager._object[i]->getExist())
            _shader->render(_effectManager.getWorldMatrix(i));
    }





    ////▼2d
    ////2d초기화
    //D3DXMATRIX mtxWorld, mtxView, mtxProjection, mtxOrtho;
    //mtxView       = _cameraPC->getMtxView();
    //mtxWorld      = Direct::getSingleton()->getMtxWorld();
    //mtxProjection = Direct::getSingleton()->getMtxProjection();
    //mtxOrtho      = Direct::getSingleton()->getMtxOrtho();

    ////2d렌더
    //Direct::getSingleton()->setZBufferOff();
    ////POINT uiPos;
    ////uiPos.x = 250;
    ////uiPos.y = 250;
    ////_ui->render(Direct::getSingleton()->getDeviceContext(),  _cameraPC->getPosition().x+uiPos.x, -_cameraPC->getPosition().y-_cameraPC->getPosition().z+uiPos.y/1.414);
    ////_uiShader->render(Direct::getSingleton()->getDeviceContext(), _ui->getIndexCount(), mtxWorld, mtxView, mtxOrtho, _ui->getTexture());
    //Direct::getSingleton()->setZBufferOn();
}