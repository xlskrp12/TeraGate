/*****************************************************************************************************
FileName : Graphic.cpp
*****************************************************************************************************/
#include"Graphic.h"
Graphic::Graphic()
{
    _direct      = 0;
    _camera      = 0;
    _model       = 0;
    _colorShader = 0;
}
Graphic::Graphic(const Graphic& graphicManager)
{

}
Graphic::~Graphic()
{

}
bool Graphic::init(int screenWidth, int screenHeight, HWND window)
{
    bool result;

    /*****************************************************************************************************
    Direct 생성, 초기화
    *****************************************************************************************************/
    _direct = new Direct;
    if (!_direct)
        return false;
    result = _direct->init(
        screenWidth, screenHeight,
        VSYNC_ENABLED, window,
        FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
    if (!result)
    {
        MessageBox(window, L"Direct3D를 초기화하지 못했습니다.", L"Error", MB_OK);
        return false;
    }
    /*****************************************************************************************************
    Camera 생성, 초기화
    *****************************************************************************************************/
    _camera = new Camera;
    if (!_camera)
        return false;
    _camera->setPosition(0.0f, 0.0f, -10.0f);
    /*****************************************************************************************************
    Model 오브젝트 생성, 초기화
    *****************************************************************************************************/
    _model = new Model;
    if (!_model)
    {
        MessageBox(window, L"Model 객체를 생성하지 못했습니다.", L"Error", MB_OK);
        return false;
    }
    result = _model->init(_direct->getDevice());
    if (!result)
    {
        MessageBox(window, L"Model 객체를 초기화하지 못했습니다.", L"Error", MB_OK);
        return false;
    }
    /*****************************************************************************************************
    Shader 생성, 초기화
    *****************************************************************************************************/
    _colorShader = new ColorShader;
    if (!_colorShader)
    {
        MessageBox(window, L"ColorShader 객체를 생성하지 못했습니다.", L"Error", MB_OK);
        return false;
    }
    result = _colorShader->init(_direct->getDevice(), window);
    if (!result)
    {
        MessageBox(window, L"ColorShadaer 객체를 초기화하지 못했습니다.", L"Error", MB_OK);
        return false;
    }
    // 모든 초기화 성공
    return true;
}
void Graphic::shut()
{
    if (_colorShader)
    {
        _colorShader->shut();
        delete _colorShader;
        _colorShader = 0;
    }
    if (_model)
    {
        _model->shut();
        delete _model;
        _model = 0;
    }
    if (_camera)
    {
        delete _camera;
        _camera = 0;
    }
    if (_direct)
    {
        _direct->shut();
        delete _direct;
        _direct = 0;
    }
    return;
}
bool Graphic::frame()
{
    bool result;
    result = render();
    if (!result)
        return false;
    return true;
}
bool Graphic::render()
{
    // 씬을 그리기 위해 버퍼의 내용을 지운다.
    _direct->beginScene(0.0f, 0.0f, 0.5f, 1.0f);
    // ▼ 렌더 코드
    // 월드 & 뷰 & 투영 행렬
    D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
    bool result;
    // 카메라 위치에 기반한 뷰 행렬 생성
    _camera->render();
    
    // 카메라 & 다이렉트 객체로부터 월드 & 뷰 & 투영 행렬 얻기
    _camera->getViewMatrix(viewMatrix);
    _direct->getWorldMatrix(worldMatrix);
    _direct->getProjectionMatrix(projectionMatrix);

    // 모델의 정점&인덱스 버퍼를 그래픽 파이프라인에 넣어 렌더를 준비시킨다.
    _model->render(_direct->getDeviceContext());

    // 컬러 셰이더를 이용해 모델 렌더
    result = _colorShader->render(
        _direct->getDeviceContext(),
        _model->getIndexCount(),
        worldMatrix, viewMatrix, projectionMatrix);
    if (!result)
        return false;
    // ▲ 렌더 코드
    // 버퍼에 그려진 씬을 화면에 표시한다.
    _direct->endScene();
    return true;
}