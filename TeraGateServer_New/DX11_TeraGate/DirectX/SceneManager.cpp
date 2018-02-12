#include"SceneManager.h"
SceneManager::SceneManager()
{
}
SceneManager::SceneManager(const SceneManager& renderer)
{

}
SceneManager::~SceneManager()
{

}
//void SceneManager::init(HWND hwnd, int screenWidth, int screenHeight)
//-----------------------------------------------------------------------------------------------
void SceneManager::init(HWND hwnd, int screenWidth, int screenHeight, Keyboard* keyboard)
//-----------------------------------------------------------------------------------------------
{
    //��ٷ�
    if (!Direct::getSingleton()) MessageBox(NULL, L"Direct::getSingleton()", L"Error", NULL);
    Direct::getSingleton()->init(hwnd, FULL_SCREEN, VSYNC_ENABLED, screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
    
    //��FBXManager
    if (!FBXManager::getSingleton()) MessageBox(NULL, L"FBXManager::getSingleton()", L"Error", NULL);
    FBXManager::getSingleton()->init(hwnd, Direct::getSingleton());
    TextureManager::getSingleton()->init(Direct::getSingleton());

	//--------------------------------------------------------------------------
	_keyboard = keyboard;
	//--------------------------------------------------------------------------

    //���
    _scene[Scene::STATE_INGAME] = new SceneIngame;
    _scene[Scene::STATE_INTRO]  = new SceneIntro;
#ifdef _DEBUG
    printf("Renderer::init();\n");
#endif
}
void SceneManager::release()
{
    for(int i=0;i<Scene::STATE_VOLUME;i++)
    {
       delete [] _scene[i];
       _scene[i] = NULL;
    }
}
void SceneManager::render()
{
    //������ Ŭ����
    Direct::getSingleton()->clearBuffer(0.1f, 0.1f, 0.4f, 1.0f);
    //���Ϸ��� ��������
    // Model���� �ٷ�DC�� �־
    // DC::IASetVertexBuffers(�� ���ؽ� ����);
    // DC::IASetIndexBuffer(�� �ε��� ����);
    // DC::IASetPrimitiveTopology(�ﰢ��);   

    _scene[_state]->render();

    //������ ����ü��
    Direct::getSingleton()->swapchain();
}
void SceneManager::update()
{
    if(_state != _scene[_state]->getSceneState())
    {
        _state = _scene[_state]->getSceneState();
        _scene[_state]->enterScene();
    }
    _scene[_state]->update();

    if (GetKeyState(VK_ESCAPE) & 0x8000)
        PostQuitMessage(1);
}