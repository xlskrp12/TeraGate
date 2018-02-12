#include "AmbientLightShader.h"

AmbientLightShader::AmbientLightShader()
{
    _fbxManager     = FBXManager::getSingleton();
    _textureManager = TextureManager::getSingleton();
    _dc           = Direct::getSingleton()->getDeviceContext();
    _vs           = NULL;
    _ps           = NULL;
    _inputLayout  = NULL;
    _samplerState = NULL;
    _vsCBuffer    = NULL;
    _psCBuffer    = NULL;
}

AmbientLightShader::~AmbientLightShader()
{
}

void AmbientLightShader::init(ID3D11Device* device, HWND hwnd)
{
    initShader(device, hwnd, L"Shader/AmbientLightVS.hlsl", L"Shader/AmbientLightPS.hlsl");
}
void AmbientLightShader::release()
{
    releaseShader();
}
// 스왑체인 한 번 당 한 번씩 밖에 바뀌지 않는 친구들이라 셰이더 render()전에 한 번씩만 호출하면 된다.
void AmbientLightShader::setMtxCameraView(D3DXMATRIX mtxCameraView)
{
    _mtxCameraView = mtxCameraView;
}
// 스왑체인 한 번 당 한 번씩 밖에 바뀌지 않는 친구들이라 셰이더 render()전에 한 번씩만 호출하면 된다.
void AmbientLightShader::setMtxProjection()
{
    _mtxProjection = Direct::getSingleton()->getMtxProjection();
}
// 스왑체인 한 번 당 한 번씩 밖에 바뀌지 않는 친구들이라 셰이더 render()전에 한 번씩만 호출하면 된다.
void AmbientLightShader::setLight(Light* light)
{
    _light = light;
}
// fbx종류당 한 번씩 호출해주면 된다. fbxManager에서 같은 종류는 같은 종류끼리 뭉쳐서 정렬시켜줄거다.
void AmbientLightShader::target(int fbxID, int textureID)
{
    _indexCount = _fbxManager->getIndexCount(fbxID);
    _texture    = _textureManager->getTexture(textureID);
}
// 백버퍼 드로우 전에 텍스쳐를 설정해야 함을 주의.
void AmbientLightShader::render(D3DXMATRIX mtxWorld)
{
    HRESULT                  result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    unsigned int             bufferNumber;
    VSCBuffer                *vsCBuffer;
    PSCBuffer                *psCBuffer;

    //▼각 행렬들을 전치(transpose)해 셰이더에서 사용할 수 있도록 준비
    D3DXMATRIX mtxCameraView = _mtxCameraView;
    D3DXMATRIX mtxProjection = _mtxProjection;
    D3DXMatrixTranspose(&mtxWorld, &mtxWorld);
    D3DXMatrixTranspose(&mtxCameraView, &mtxCameraView);
    D3DXMatrixTranspose(&mtxProjection, &mtxProjection);

    //▼정점셰이더 상수버퍼 set
    // 상수버퍼를 map해 쓰기모드로 전환
    result = _dc->Map(_vsCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11DeviceContext::Map()", L"Error", NULL);    
    // 상수버퍼로 데이터를 카피
    vsCBuffer = (VSCBuffer*)mappedResource.pData;// 상수버퍼 데이터에 대한 포인터를 가져온다.
    vsCBuffer->mtxWorld      = mtxWorld;
    vsCBuffer->mtxView       = mtxCameraView;
    vsCBuffer->mtxProjection = mtxProjection;
    // 상수버퍼를 unmap해 읽기모드로 전환
    _dc->Unmap(_vsCBuffer, 0);
    // 정점셰이더 상수버퍼 set
    bufferNumber = 0;// 셰이더에 있는 상수버퍼 포지션을 0으로
    _dc->VSSetConstantBuffers(bufferNumber, 1, &_vsCBuffer);

    //▼픽셀셰이더에 텍스쳐 리소스 set
    _dc->PSSetShaderResources(0, 1, &_texture);

    //▼픽셀셰이더 상수버퍼 set
    // 상수버퍼를 map해 쓰기모드로 전환
    result = _dc->Map(_psCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11DeviceContext::Map()", L"Error", NULL);
    // 상수버퍼로 데이터를 카피
    psCBuffer = (PSCBuffer*)mappedResource.pData;// 상수버퍼 데이터에 대한 포인터를 가져온다.
    psCBuffer->ambientColor   = _light->getAmbientColor();
    psCBuffer->diffuseColor   = _light->getDiffuseColor();
    psCBuffer->lightDirection = _light->getDirection();
    psCBuffer->padding        = 0.f;
    // 상수버퍼를 unmap해 읽기모드로 전환
    _dc->Unmap(_psCBuffer, 0);
    bufferNumber = 0;// 셰이더에 있는 상수버퍼 포지션을 0으로
    _dc->PSSetConstantBuffers(bufferNumber, 1, &_psCBuffer);



    //▼백버퍼에 draw
    _dc->IASetInputLayout(_inputLayout);     // 입력 레이아웃         set
    _dc->VSSetShader(_vs, NULL, 0);          // 정점셰이더            set
    _dc->PSSetShader(_ps, NULL, 0);          // 픽셀셰이더            set
    _dc->PSSetSamplers(0, 1, &_samplerState);// 픽셀셰이더 샘플러 상태 set 
    _dc->DrawIndexed(_indexCount, 0, 0);      // 그리기
}
void AmbientLightShader::initShader(
    ID3D11Device *device,
    HWND         hwnd,
    WCHAR        *vsFileName,
    WCHAR        *psFileName)
{
    HRESULT                  result;
    ID3D10Blob               *errorMessage = NULL;
    ID3D10Blob               *vs           = NULL;
    ID3D10Blob               *ps           = NULL;
    D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[3];
    unsigned int             inputNumber;
    D3D11_SAMPLER_DESC       samplerDesc;
    D3D11_BUFFER_DESC        vsCBufferDesc;
    D3D11_BUFFER_DESC        psCBufferDesc;
    //▼정점셰이더 생성
    // 셰이더 파일로드
    result = D3DX11CompileFromFile(vsFileName, NULL, NULL, "AmbientLightVS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vs, &errorMessage, NULL);
    if (FAILED(result))
    {
        if (errorMessage)
        {
            outputShaderErrorMessage(errorMessage, hwnd, vsFileName);
            MessageBox(NULL, L"D3DX11CompileFromFile()", L"Error", NULL);
        }
        else
        {
            // 에러메시지가 없다면 파일을 찾지 못한 것.
            WCHAR wstr[100];
            wsprintf(wstr, L"D3DX11ComfileFromFile()\n지정한 파일을 찾지 못했습니다.\n%s\n", vsFileName);
            MessageBox(NULL, wstr, L"Error", NULL);
        }
    }
    // 셰이더 생성
    result = device->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), NULL, &_vs);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11Device::CreateVertexShader()", L"Error", NULL);
    //▼픽셀셰이더 생성
    // 셰이더 파일로드
    result = D3DX11CompileFromFile(psFileName, NULL, NULL, "AmbientLightPS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &ps, &errorMessage, NULL);
    if (FAILED(result))
    {
        if (errorMessage)
        {
            outputShaderErrorMessage(errorMessage, hwnd, psFileName);
        }
        else
        {
            // 에러메시지가 없다면 파일을 찾지 못한 것.
            WCHAR wstr[100];
            wsprintf(wstr, L"D3DX11ComfileFromFile()\n지정한 파일을 찾지 못했습니다.\n%s\n", psFileName);
            MessageBox(NULL, wstr, L"Error", NULL);
        }
    }
    // 셰이더 생성
    result = device->CreatePixelShader(ps->GetBufferPointer(), ps->GetBufferSize(), NULL, &_ps);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11Device::CreatePixelShader()", L"Error", NULL);

    //▼정점 입력레이아웃 생성
    inputLayoutDesc[0].SemanticName         = "POSITION";//SemanticName값으로는 POSITION, COLOR, TEXCOORD, NORMAL 등이 올 수 있다.
    inputLayoutDesc[0].SemanticIndex        = 0;
    inputLayoutDesc[0].Format               = DXGI_FORMAT_R32G32B32_FLOAT; 
    inputLayoutDesc[0].InputSlot            = 0;
    inputLayoutDesc[0].AlignedByteOffset    = 0;
    inputLayoutDesc[0].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
    inputLayoutDesc[0].InstanceDataStepRate = 0;
    inputLayoutDesc[1].SemanticName         = "TEXCOORD";
    inputLayoutDesc[1].SemanticIndex        = 0 ;
    inputLayoutDesc[1].Format               = DXGI_FORMAT_R32G32_FLOAT;// DXGI_FORMAT_R32G32B32_FLOAT, DXGI_FORMAT_R32G32_FLOAT
    inputLayoutDesc[1].InputSlot            = 0;
    inputLayoutDesc[1].AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT; // 버퍼에 데이터가 어떻게 배열되는지를 알려줘야하는데, 자동으로 알아내라는 뜻
    inputLayoutDesc[1].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
    inputLayoutDesc[1].InstanceDataStepRate = 0;
    inputLayoutDesc[2].SemanticName         = "NORMAL";
    inputLayoutDesc[2].SemanticIndex        = 0;
    inputLayoutDesc[2].Format               = DXGI_FORMAT_R32G32B32_FLOAT;// DXGI_FORMAT_R32G32B32_FLOAT, DXGI_FORMAT_R32G32_FLOAT
    inputLayoutDesc[2].InputSlot            = 0;
    inputLayoutDesc[2].AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT; // 버퍼에 데이터가 어떻게 배열되는지를 알려줘야하는데, 자동으로 알아내라는 뜻
    inputLayoutDesc[2].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
    inputLayoutDesc[2].InstanceDataStepRate = 0;
    inputNumber = sizeof(inputLayoutDesc) / sizeof(inputLayoutDesc[0]);
    result = device->CreateInputLayout(inputLayoutDesc, inputNumber, vs->GetBufferPointer(), vs->GetBufferSize(), &_inputLayout);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11Device::CreateInputLayout()", L"Error", NULL);

    //▼정점&픽셀 셰이더, 정점 입력레이아웃 생성 후 사용하지 않는 자원 해제
    vs->Release();
    vs = NULL;
    ps->Release();
    ps = NULL;

    //▼정점셰이더 상수버퍼 생성
    vsCBufferDesc.Usage                = D3D11_USAGE_DYNAMIC;
    vsCBufferDesc.ByteWidth            = sizeof(VSCBuffer);
    vsCBufferDesc.BindFlags            = D3D11_BIND_CONSTANT_BUFFER;// 상수버퍼를 이 버퍼로 한다고 설정.
    vsCBufferDesc.CPUAccessFlags       = D3D11_CPU_ACCESS_WRITE;    // 상수버퍼 용도에 맞는 값
    vsCBufferDesc.MiscFlags            = 0;
    vsCBufferDesc.StructureByteStride  = 0;
    result = device->CreateBuffer(&vsCBufferDesc, NULL, &_vsCBuffer);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11Device::CreateBuffer()", L"Error", NULL);
    
    //▼픽셀셰이더 상수버퍼 생성
    psCBufferDesc.Usage               = D3D11_USAGE_DYNAMIC;
    psCBufferDesc.ByteWidth           = sizeof(PSCBuffer);
    psCBufferDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
    psCBufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
    psCBufferDesc.MiscFlags           = 0;
    psCBufferDesc.StructureByteStride = 0;
    result = device->CreateBuffer(&psCBufferDesc, NULL, &_psCBuffer);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11Device::CreateBuffer()", L"Error", NULL);

    //▼샘플러 스테이트 생성
    samplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;// 샘플러가 축소확대+밉레벨 샘플링시 선형보간(linear interpolation)사용 // 처리량은 좀 무겁지만 퀄리티가 좋다.
    samplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias     = 0.0f;
    samplerDesc.MaxAnisotropy  = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD         = 0;
    samplerDesc.MaxLOD         = D3D11_FLOAT32_MAX;
    result = device->CreateSamplerState(&samplerDesc, &_samplerState);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11Device::CreateSamplerState()", L"Error", NULL);
}
void AmbientLightShader::releaseShader()
{
    if (_psCBuffer)
    {
        _psCBuffer->Release();
        _psCBuffer = NULL;
    }
    if (_vsCBuffer)
    {
        _vsCBuffer->Release();
        _vsCBuffer = NULL;
    }
    if (_samplerState)
    {
        _samplerState->Release();
        _samplerState = NULL;
    }
    if (_inputLayout)
    {
        _inputLayout->Release();
        _inputLayout = NULL;
    }
    if (_ps)
    {
        _ps->Release();
        _ps = NULL;
    }
    if (_vs)
    {
        _vs->Release();
        _vs = NULL;
    }
}
void AmbientLightShader::outputShaderErrorMessage(ID3D10Blob *errorMessage, HWND hwnd, WCHAR *shaderFileName)
{
    using std::ofstream;
    char*         compileError;
    unsigned long bufferSize;
    ofstream      file;

    compileError = (char*)(errorMessage->GetBufferPointer());
    bufferSize = errorMessage->GetBufferSize();
    file.open("shader-error.txt");
    for (int i = 0; i < bufferSize; i++)
    {
        file << compileError[i];
    }
    file.close();
    errorMessage->Release();
    errorMessage = NULL;

    WCHAR wstr[100];
    wsprintf(wstr, L"%s\n셰이더 파일을 컴파일하는데 문제가 생겼습니다.\n", shaderFileName);
    MessageBox(hwnd, wstr, L"Error", NULL);
}