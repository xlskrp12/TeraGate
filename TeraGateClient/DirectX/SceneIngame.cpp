#include "SceneIngame.h"
SceneIngame::SceneIngame()
{
    init();
}
SceneIngame::~SceneIngame()
{
    release();
}
void SceneIngame::init()
{
    //▼카메라
    _camera = new Camera;
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
    _light->setDiffuseColor(0.2f, 0.2f, 0.2f, 1.0f);
    _light->setAmbientColor(0.9f, 0.9f, 0.9f, 1.0f);
    _light->setDirection(0.f, -1.f, 0.f);

    //▼오브젝트 매니저
    Object* object;
    // 땅 * 1
    object= new ObjectGround();
    object->setPos(0, 0, 0);
    _objectManager.addObject(object);

    // 벽 * 1
    object= new ObjectGround();
    object->setPos(0, 0, 0);
    _objectManager.addObject(object);

    // 스카이박스 * 1
    object = new ObjectGround();
    object->setPos(0, 0, 0);
    _objectManager.addObject(object);

    // 게이트 * 4
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
    // 바위 * N
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
    // 미니언 * N
    for (int i = 0; i < 200; i++)
    {
        object = new ObjectMinion();
        object->setPos(0, 0, 0);
        _objectManager.addObject(object);
    }
    // PC * 1
    object = new ObjectPC();
    object->setPos(0, 0, 0);
    _objectManager.addObject(object);
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
}
void SceneIngame::proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{

}
void SceneIngame::update()
{
    _objectManager.update();
    _camera->update();
}
void SceneIngame::render()
{
    // 땅
    FBXManager::getSingleton()->render(FBXManager::GROUND);
    _shader->render(
        Direct::getSingleton()->getDeviceContext(),
        FBXManager::getSingleton()->getIndexCount(FBXManager::GROUND),
        _objectManager.getWorldMatrix(0),// 중요..
        _camera->getMtxView(), Direct::getSingleton()->getMtxProjection(),
        FBXManager::getSingleton()->getTexture(FBXManager::GROUND),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    //// 벽
    FBXManager::getSingleton()->render(FBXManager::WALL);
    _shader->render(
        Direct::getSingleton()->getDeviceContext(),
        FBXManager::getSingleton()->getIndexCount(FBXManager::WALL),
        _objectManager.getWorldMatrix(1),// 중요..
        _camera->getMtxView(), Direct::getSingleton()->getMtxProjection(),
        FBXManager::getSingleton()->getTexture(FBXManager::WALL),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    // 스카이박스
    FBXManager::getSingleton()->render(FBXManager::SKY);
    _shader->render(
        Direct::getSingleton()->getDeviceContext(),
        FBXManager::getSingleton()->getIndexCount(FBXManager::SKY),
        _objectManager.getWorldMatrix(2),// 중요..
        _camera->getMtxView(), Direct::getSingleton()->getMtxProjection(),
        FBXManager::getSingleton()->getTexture(FBXManager::SKY),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    // 게이트
    FBXManager::getSingleton()->render(FBXManager::GATE);
    _shader->render(
        Direct::getSingleton()->getDeviceContext(),
        FBXManager::getSingleton()->getIndexCount(FBXManager::GATE),
        _objectManager.getWorldMatrix(3),
        _camera->getMtxView(), Direct::getSingleton()->getMtxProjection(),
        FBXManager::getSingleton()->getTexture(FBXManager::GATE),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    _shader->render(
        Direct::getSingleton()->getDeviceContext(),
        FBXManager::getSingleton()->getIndexCount(FBXManager::GATE),
        _objectManager.getWorldMatrix(4),
        _camera->getMtxView(), Direct::getSingleton()->getMtxProjection(),
        FBXManager::getSingleton()->getTexture(FBXManager::GATE),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    _shader->render(
        Direct::getSingleton()->getDeviceContext(),
        FBXManager::getSingleton()->getIndexCount(FBXManager::GATE),
        _objectManager.getWorldMatrix(5),
        _camera->getMtxView(), Direct::getSingleton()->getMtxProjection(),
        FBXManager::getSingleton()->getTexture(FBXManager::GATE),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    _shader->render(
        Direct::getSingleton()->getDeviceContext(),
        FBXManager::getSingleton()->getIndexCount(FBXManager::GATE),
        _objectManager.getWorldMatrix(6),
        _camera->getMtxView(), Direct::getSingleton()->getMtxProjection(),
        FBXManager::getSingleton()->getTexture(FBXManager::GATE),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    //// 스톤
    FBXManager::getSingleton()->render(FBXManager::STONE);
    for(int i=7;i<107;i++)
    {
        _shader->render(
            Direct::getSingleton()->getDeviceContext(),
            FBXManager::getSingleton()->getIndexCount(FBXManager::STONE),
            _objectManager.getWorldMatrix(i),
            _camera->getMtxView(), Direct::getSingleton()->getMtxProjection(),
            FBXManager::getSingleton()->getTexture(FBXManager::STONE),
            _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    }

    // 미니언
    FBXManager::getSingleton()->render(FBXManager::WORM);
    for(int i=107;i<307;i++)
    {
        _shader->render(
            Direct::getSingleton()->getDeviceContext(),
            FBXManager::getSingleton()->getIndexCount(FBXManager::WORM),
            _objectManager.getWorldMatrix(i),
            _camera->getMtxView(), Direct::getSingleton()->getMtxProjection(),
            FBXManager::getSingleton()->getTexture(FBXManager::WORM),
            _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
    }
    // pc
    // fbx, direct, objectManager, camera, light
    FBXManager::getSingleton()->render(FBXManager::PC);
    _shader->render(
        Direct::getSingleton()->getDeviceContext(),
        FBXManager::getSingleton()->getIndexCount(FBXManager::PC),
        _objectManager.getWorldMatrix(307),
        _camera->getMtxView(), Direct::getSingleton()->getMtxProjection(),
        FBXManager::getSingleton()->getTexture(FBXManager::PC),
        _light->getDirection(), _light->getDiffuseColor(), _light->getAmbientColor());
}