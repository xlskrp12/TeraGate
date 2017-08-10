#include"Renderer.h"
Renderer* Renderer::instance = nullptr;

Renderer::Renderer()
{
    _direct = NULL;
    _camera = NULL;
    _shader = NULL;
    _light  = NULL;
}
Renderer::Renderer(const Renderer& renderer)
{

}
Renderer::~Renderer()
{

}
void Renderer::init(HWND hwnd, int screenWidth, int screenHeight)
{
    //��ٷ�
    _direct = new Direct;
    if (!_direct) MessageBox(NULL, L"new Direct", L"Error", NULL);
    _direct->init(hwnd, FULL_SCREEN, VSYNC_ENABLED, screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
    
    //��ī�޶�
    _camera = new Camera;
    if (!_camera) MessageBox(NULL, L"new Camera", L"Error", NULL);
    _camera->setPos(D3DXVECTOR3(0, 0, -1250));
    _camera->setRot(D3DXVECTOR3(45, 0, 0));


    /*
        ���Ŀ� _fbx �����͵���
        _object�ֵ� ��������� �Ѱܰ�����,
        render�� _object���� �ٷ� �� �� �ֵ��� ����ϰ� �������ؾ߰ڴ�
    */
    //���
    //��
    _fbx[0] = new FBX;
    if (!_fbx[0]) MessageBox(NULL, L"new FBX", L"Error", NULL);
    _fbx[0]->init(_direct->getDevice(), "Resource/FBX/Ground.fbx", L"Resource/DDS/Ground.dds");

    _fbx[1] = new FBX;
    if (!_fbx[1]) MessageBox(NULL, L"new FBX", L"Error", NULL);
    _fbx[1]->init(_direct->getDevice(), "Resource/FBX/wall.fbx", L"Resource/DDS/wall.dds");

    //��ī�̹ڽ�
    _fbx[2] = new FBX;
    if (!_fbx[2]) MessageBox(NULL, L"new FBX", L"Error", NULL);
    _fbx[2]->init(_direct->getDevice(), "Resource/FBX/skybox.fbx", L"Resource/DDS/skybox.dds");
    //������Ʈ ����Ʈ
    _fbx[3] = new FBX;
    if (!_fbx[3]) MessageBox(NULL, L"new FBX", L"Error", NULL);
    _fbx[3]->init(_direct->getDevice(), "Resource/FBX/gate.fbx", L"Resource/DDS/gate.dds");
    //������Ʈ ����
    _fbx[4] = new FBX;
    if (!_fbx[4]) MessageBox(NULL, L"new FBX", L"Error", NULL);
    _fbx[4]->init(_direct->getDevice(), "Resource/FBX/stone.fbx", L"Resource/DDS/stone.dds");
    //������Ʈ �̴Ͼ�
    _fbx[5] = new FBX;
    if (!_fbx[5]) MessageBox(NULL, L"new FBX", L"Error", NULL);
    _fbx[5]->init(_direct->getDevice(), "Resource/FBX/minion.fbx", L"Resource/DDS/minion.dds");

    //������ƮPC
    _fbx[6] = new FBX;
    if (!_fbx[6])MessageBox(NULL, L"new FBX", L"Error", NULL);
    _fbx[6]->init(_direct->getDevice(), "Resource/FBX/hengelBlack.fbx", L"Resource/DDS/hengelBlack.dds");


    //�������Ʈ �Ŵ���
    //Object* object;
    // �� * 1
    object= new ObjectGround();
    object->setPos(0, 0, 0);
    _objectManager.addObject(object);

    // �� * 1
    object= new ObjectGround();
    object->setPos(0, 0, 0);
    _objectManager.addObject(object);

    // ��ī�̹ڽ� * 1
    object = new ObjectGround();
    object->setPos(0, 0, 0);
    _objectManager.addObject(object);

    // ����Ʈ * 4
    object = new ObjectGate();
    object->setPos(0, 0, 0);
    _objectManager.addObject(object);

    object = new ObjectGate();
    object->setPos(-600, 0, -600);
    _objectManager.addObject(object);

    object = new ObjectGate();
    object->setPos(600, 0, -600);
    _objectManager.addObject(object);

    object = new ObjectGate();
    object->setPos(0, 0, 800);
    _objectManager.addObject(object);
    // ���� * N
    for(int i=0;i<100;i++)
    {
        object= new ObjectStone();
        SRAND_SEED(i);
        object->setPos(RANDOM_E(400, 1000), 0, RANDOM_E(400, 1000));
        object->setRotY(RANDOM_M(0, 360));
        static float r = 1400;
        if (r < object->getPosX())
            object->setPosX(r);
        if (object->getPosX() < -1*r)
            object->setPosX(-1 * r);
        if (r < object->getPosZ())
            object->setPosZ(r);
        if (object->getPosZ() < -1 * r)
            object->setPosZ(-1 * r);

        _objectManager.addObject(object);
    }
    // �̴Ͼ� * N
    /*for (int i = 0; i < 200; i++)
    {
        object = new ObjectMinion();
        object->setPos(0, 0, 0);
        _objectManager.addObject(object);
    }*/
	for (int i = 0; i < NUM_OF_NPC - NPC_START; i++)
	{
		NPCObject[i] = new ObjectMinion();
		NPCObject[i]->_exist = false;
		NPCObject[i]->setPos(0, 0, 0);
		_objectManager.addObject(NPCObject[i]);
	}

    // PC * MAX_USER
	for (int i = 0; i < MAX_USER; i++)
	{
		playerObject[i] = new ObjectPC();
		playerObject[i]->_exist = false;
		playerObject[i]->setPos(0, 0, 0);
		_objectManager.addObject(playerObject[i]);
	}


    //����̴�
    _shader = new AmbientLightShader;
    if (!_shader) MessageBox(NULL, L"new LightShader", L"Error", NULL);
    _shader->init(_direct->getDevice(), hwnd);
    
    //������
    _light = new Light;
    if (!_light) MessageBox(NULL, L"new Light", L"Error", NULL);
    _light->setDiffuseColor(0.2f, 0.2f, 0.2f, 1.0f);
    _light->setAmbientColor(0.9f, 0.9f, 0.9f, 1.0f);
    _light->setDirection(0.f, -1.f, 0.f);
#ifdef _DEBUG
    printf("Renderer::init();\n");
#endif
}
void Renderer::release()
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

    for (int i = 0; i < 10; i++)
    {
        if (_fbx[i])
        {
            _fbx[i]->release();
            delete _fbx[i];
            _fbx[i] = NULL;
        }
    }

    if (_camera)
    {
        delete _camera;
        _camera = NULL;
    }    
    if (_direct)
    {
        _direct->release();
        delete _direct;
        _direct = NULL;
    }
}
void Renderer::render()
{
    //������ Ŭ����
    _direct->clearBuffer(0.1f, 0.1f, 0.4f, 1.0f);
    //���Ϸ��� ��������
    // Model���� �ٷ�DC�� �־
    // DC::IASetVertexBuffers(�� ���ؽ� ����);
    // DC::IASetIndexBuffer(�� �ε��� ����);
    // DC::IASetPrimitiveTopology(�ﰢ��);

    // ��
    _fbx[0]->render(_direct->getDeviceContext());
    _shader->render(
        _direct->getDeviceContext(),
        _fbx[0]->getIndexCount(),
        _objectManager.getWorldMatrix(0),// �߿�..
        _camera->getMtxView(), _direct->getMtxProjection(),
        _fbx[0]->getTexture(),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    // ��
    _fbx[1]->render(_direct->getDeviceContext());
    _shader->render(
        _direct->getDeviceContext(),
        _fbx[1]->getIndexCount(),
        _objectManager.getWorldMatrix(1),// �߿�..
        _camera->getMtxView(), _direct->getMtxProjection(),
        _fbx[1]->getTexture(),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    // ��ī�̹ڽ�
    _fbx[2]->render(_direct->getDeviceContext());
    _shader->render(
        _direct->getDeviceContext(),
        _fbx[2]->getIndexCount(),
        _objectManager.getWorldMatrix(2),// �߿�..
        _camera->getMtxView(), _direct->getMtxProjection(),
        _fbx[2]->getTexture(),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    // ����Ʈ
    _fbx[3]->render(_direct->getDeviceContext());
    _shader->render(
        _direct->getDeviceContext(),
        _fbx[3]->getIndexCount(),
        _objectManager.getWorldMatrix(3),
        _camera->getMtxView(), _direct->getMtxProjection(),
        _fbx[3]->getTexture(),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    _shader->render(
        _direct->getDeviceContext(),
        _fbx[3]->getIndexCount(),
        _objectManager.getWorldMatrix(4),
        _camera->getMtxView(), _direct->getMtxProjection(),
        _fbx[3]->getTexture(),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    _shader->render(
        _direct->getDeviceContext(),
        _fbx[3]->getIndexCount(),
        _objectManager.getWorldMatrix(5),
        _camera->getMtxView(), _direct->getMtxProjection(),
        _fbx[3]->getTexture(),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    _shader->render(
        _direct->getDeviceContext(),
        _fbx[3]->getIndexCount(),
        _objectManager.getWorldMatrix(6),
        _camera->getMtxView(), _direct->getMtxProjection(),
        _fbx[3]->getTexture(),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    // ����
    _fbx[4]->render(_direct->getDeviceContext());
    for(int i=7;i<107;i++)
    {
        _shader->render(
            _direct->getDeviceContext(),
            _fbx[4]->getIndexCount(),
            _objectManager.getWorldMatrix(i),
            _camera->getMtxView(), _direct->getMtxProjection(),
            _fbx[4]->getTexture(),
            _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    }

    // �̴Ͼ�
    _fbx[5]->render(_direct->getDeviceContext());
    for(int i=107;i<307;i++)
    {
		if (true == NPCObject[i - 107]->_exist)
		{
			_shader->render(
				_direct->getDeviceContext(),
				_fbx[5]->getIndexCount(),
				_objectManager.getWorldMatrix(i),
				_camera->getMtxView(), _direct->getMtxProjection(),
				_fbx[5]->getTexture(),
				_light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
		}
    }
    // pc
	
    _fbx[6]->render(_direct->getDeviceContext());
	for (int i = 307; i < 307 + MAX_USER; i++)
	{
		if(true == playerObject[i-307]->_exist)
		{
			_shader->render(
				_direct->getDeviceContext(),
				_fbx[6]->getIndexCount(),
				_objectManager.getWorldMatrix(i),
				_camera->getMtxView(), _direct->getMtxProjection(),
				_fbx[6]->getTexture(),
				_light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
		}
	}
    

    //������ ����ü��
    _direct->swapchain();
}
void Renderer::update()
{
    _objectManager.update();
    _camera->update();
}