#include "SceneResult.h"
SceneResult::SceneResult()
{
}
SceneResult::~SceneResult()
{
}
void SceneResult::enterScene()
{
    _state = Scene::STATE_INTRO;
    // ��� bgm �����
    PlaySound(L"Resource/Sound/bgmUntitledPiano.wav", NULL, SND_ASYNC | SND_LOOP);
}
void SceneResult::init()
{
    enterScene();

    //��ī�޶�
    _camera = new Camera;
    if (!_camera) MessageBox(NULL, L"new Camera", L"Error", NULL);
    _camera->setPos(D3DXVECTOR3(0, 0, -300));
    _camera->setRot(D3DXVECTOR3(0, 90, 0));

    //��FBXManager
    FBXManager::getSingleton();


    //����̴�
    _shader = new AmbientLightShader;
    if (!_shader) MessageBox(NULL, L"new LightShader", L"Error", NULL);
    _shader->init(Direct::getSingleton()->getDevice(), Direct::getSingleton()->getHWND());
    
    //������
    _light = new Light;
    if (!_light) MessageBox(NULL, L"new Light", L"Error", NULL);
    _light->setDiffuseColor(0.7f, 0.7f, 0.7f, 1.0f);
    _light->setAmbientColor(0.3f, 0.3f, 0.3f, 1.0f);
    _light->setDirection(0.0f, 0.0f, 1.f);

    //�������Ʈ �Ŵ���
    Object* object;

    // ��ī�̹ڽ� * 1
    object = new ObjectStatic();
    object->setPos(0, 0, 0);
    _objectManager.addObject(object);

    // �ΰ� * 1
    object= new ObjectStatic();
    object->setPos(0, 0, 0);
    object->setRot(0, 90, 0);
    _objectManager.addObject(object);
}
void SceneResult::release()
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
