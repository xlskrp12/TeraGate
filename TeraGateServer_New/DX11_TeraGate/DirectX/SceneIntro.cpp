#include "SceneIntro.h"
SceneIntro::SceneIntro()
{
    init();
    _logoTimer.setCooltime(0.04);
}
SceneIntro::~SceneIntro()
{
    release();

}
void SceneIntro::enterScene()
{
    _state = Scene::STATE_INTRO;
    // 브금 bgm 배경음
    PlaySound(L"Resource/Sound/bgmUntitledPiano.wav", NULL, SND_ASYNC | SND_LOOP);
}
void SceneIntro::init()
{
    enterScene();

    //▼카메라
    _camera = new Camera;
    if (!_camera) MessageBox(NULL, L"new Camera", L"Error", NULL);
    _camera->setPos(D3DXVECTOR3(0, 0, -300));
    _camera->setRot(D3DXVECTOR3(0, 90, 0));

    //▼FBXManager
    FBXManager::getSingleton();


    //▼셰이더
    _shader = new AmbientLightShader;
    if (!_shader) MessageBox(NULL, L"new LightShader", L"Error", NULL);
    _shader->init(Direct::getSingleton()->getDevice(), Direct::getSingleton()->getHWND());
    
    //▼조명
    _light = new Light;
    if (!_light) MessageBox(NULL, L"new Light", L"Error", NULL);
    _light->setDiffuseColor(0.7f, 0.7f, 0.7f, 1.0f);
    _light->setAmbientColor(0.3f, 0.3f, 0.3f, 1.0f);
    _light->setDirection(0.0f, 0.0f, 1.f);

    //▼오브젝트 매니저
    Object* object;

    // 스카이박스 * 1
    object = new ObjectStatic();
    object->setPos(0, 0, 0);
    _objectManager.addObject(object);

    // 로고 * 1
    object= new ObjectStatic();
    object->setPos(0, 0, 0);
    object->setRot(0, 90, 0);
    _objectManager.addObject(object);
}
void SceneIntro::release()
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
void SceneIntro::proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{

}
void SceneIntro::update()
{
    if (GetKeyState(VK_F2) & 0x8000)
    {
        _state = Scene::STATE_INGAME;

		_ServerDef->Login();
        return;
    }


    _objectManager.updateCommon();
    _camera->update();

    const static float rotMinus  = -5;
    const static float rotPlus   = 5;
    const static float rotUnit   = +0.4f;
    const static int   plus      = +1;
    const static int   minus     = -1;
    static int   signState = plus;
    static float rotY      = rotMinus;
    if(_logoTimer.isOn()){
        switch(signState){
        case plus:
            rotY += rotUnit * signState;
            if(rotPlus < rotY)
                signState = minus;
            break;
        case minus:
            rotY += rotUnit * minus;
            if(rotY < rotMinus)
                signState = plus;
            break;
        }// switch
    }// if
    _objectManager._object[1]->setRotY(rotY+90);
}
void SceneIntro::render()
{
    // 셰이더에서 필요한 것들 한 번씩만 초기화
    _shader->setMtxCameraView(_camera->getMtxView());
    _shader->setMtxProjection();

    _light->setDiffuseColor(0.7f, 0.7f, 0.7f, 1.0f);
    _light->setAmbientColor(0.3f, 0.3f, 0.3f, 1.0f);
    _light->setDirection(0.0f, 0.0f, 1.f);
    _shader->setLight(_light);

    // 스카이박스
    _shader->target(FBXManager::SKY_IRON, TextureManager::IRON);
    FBXManager::getSingleton()->render(FBXManager::SKY_IRON);
    _shader->render(_objectManager.getWorldMatrix(0));


    // 로고
    _light->setDiffuseColor(0.8f, 0.8f, 0.8f, 1.0f);
    _light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
    _light->setDirection(0.0, 0.0, 1.0);
    _shader->setLight(_light);
    FBXManager::getSingleton()->render(FBXManager::LOGO);
    _shader->target(FBXManager::LOGO, TextureManager::LOGO);
    _shader->render(_objectManager.getWorldMatrix(1));
}