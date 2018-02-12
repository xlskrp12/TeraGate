#include "ColorShader.h"
ColorShader::ColorShader()
{
    _vertexShader   = NULL;
    _pixelShader    = NULL;
    _inputLayout         = NULL;
    _cBuffer      = NULL;
}
ColorShader::ColorShader(const ColorShader&)
{

}
ColorShader::~ColorShader()
{
}
void ColorShader::init(ID3D11Device* device, HWND hwnd)
{
    initShader(device, hwnd, L"Shader/color.vs", L"Shader/color.ps");
#ifdef _DEBUG
    printf("ColorShader::init()\n");
#endif
}
void ColorShader::release()
{
    releaseShader();
}
void ColorShader::render(
    ID3D11DeviceContext* deviceContext,
    int                  indexCount,
    D3DXMATRIX           mtxWorld,
    D3DXMATRIX           mtxView,
    D3DXMATRIX           mtxProjection)
{
    // 렌더시 사용할 셰이더 인자 입력
    setShaderParameter(deviceContext, mtxWorld, mtxView, mtxProjection);
    // 셰이더로 준비된 버퍼를 렌더
    renderShader(deviceContext, indexCount);
}
void ColorShader::initShader(
    ID3D11Device* device,
    HWND          hwnd,
    WCHAR*        vsFileName,
    WCHAR*        psFileName)
{
    HRESULT                  result;
    ID3D10Blob*              errorMessage       = NULL;
    ID3D10Blob*              vertexShaderBuffer = NULL;
    ID3D10Blob*              pixelShaderBuffer  = NULL;
    D3D11_INPUT_ELEMENT_DESC inputDesc[2];
    unsigned int             inputNumber;
    D3D11_BUFFER_DESC        cBufferDesc;
    //▼정점셰이더 생성
    result = D3DX11CompileFromFile(vsFileName, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage,NULL);
    if (FAILED(result))
    {
        if (errorMessage)
        {
            outputShaderErrorMessage(errorMessage, hwnd, vsFileName);
            MessageBox(NULL, L"D3DX11CompileFromFile()", L"Error", NULL);
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
    //▼ ID3D11Device::CreateVertexShader(
    // [in]            const void*          pHsaderByteCode, // 컴파일된 셰이더를 가리키는 포인터
    // [in]            SIZE_T               VytecodeLength,  // 컴파일된 버텍스 셰이더 크기
    // [in, optional ] ID3D11ClassLinkage*  pClassLinkage,   // 연결 인터페이스(?)를 위한 포인터
    // [out, optional] ID3D11VertexShader** ppVertexShader   // ID3D11VertexShader 인터페이스를 가리킬 포인터.
    // );
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11Device::CreateVertexShader()", L"Error", NULL);
    //▼픽셀셰이더 생성
    result = D3DX11CompileFromFile(psFileName, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
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
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11Device::CreatePixelShader()", L"Error", NULL);
    //▼셰이더가 사용할 인풋 레이아웃 생성
    inputDesc[0].SemanticName         = "POSITION";
    inputDesc[0].SemanticIndex        = 0;
    inputDesc[0].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
    inputDesc[0].InputSlot            = 0;
    inputDesc[0].AlignedByteOffset    = 0;
    inputDesc[0].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
    inputDesc[0].InstanceDataStepRate = 0;
    inputDesc[1].SemanticName         = "COLOR";
    inputDesc[1].SemanticIndex        = 0 ;
    inputDesc[1].Format               = DXGI_FORMAT_R32G32B32A32_FLOAT;
    inputDesc[1].InputSlot            = 0;
    inputDesc[1].AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT; // 버퍼에 데이터가 어떻게 배열되는지를 알려줘야하는데, 자동으로 알아내라는 뜻
    inputDesc[1].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
    inputDesc[1].InstanceDataStepRate = 0;
    //▼인풋 디스크립션으로 인풋 레이아웃 생성
    // 레이아웃 생성 후 정점&픽셀 셰이더 버퍼는 사용하지 않으므로 해제
    inputNumber = sizeof(inputDesc) / sizeof(inputDesc[0]);
    result = device->CreateInputLayout(inputDesc, inputNumber, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_inputLayout);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11Device::CreateInputLayout()", L"Error", NULL);
    vertexShaderBuffer->Release();
    vertexShaderBuffer = NULL;
    pixelShaderBuffer->Release();
    pixelShaderBuffer  = NULL;
    // 셰이더를 사용하기 위해 상수버퍼(constant buffer)
    // 정점셰이더에 있는 행렬 상수버퍼 디스크립션 작성
    cBufferDesc.Usage                = D3D11_USAGE_DYNAMIC;
    cBufferDesc.ByteWidth            = sizeof(CBuffer);
    cBufferDesc.BindFlags            = D3D11_BIND_CONSTANT_BUFFER;// 상수버퍼를 이 버퍼로 한다고 설정.
    cBufferDesc.CPUAccessFlags       = D3D11_CPU_ACCESS_WRITE;    // 상수버퍼 용도에 맞는 값
    cBufferDesc.MiscFlags            = 0;
    cBufferDesc.StructureByteStride  = 0;
    // 상수버퍼 포인터(_cBuffer)를 만들어 이 클래스에서 정점 셰이더 상수버퍼에 접근할 수 있게
    result = device->CreateBuffer(&cBufferDesc, NULL, &_cBuffer);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11Device::CreateBuffer()", L"Error", NULL);
}
void ColorShader::releaseShader()
{
    // 상수버퍼, 레이아웃, 픽셀&버텍스 셰이더 등 각각 해제
    if (_cBuffer)
    {
        _cBuffer->Release();
        _cBuffer = NULL;
    }
    if (_inputLayout)
    {
        _inputLayout->Release();
        _inputLayout = NULL;
    }
    if (_pixelShader)
    {
        _pixelShader->Release();
        _pixelShader = NULL;
    }
    if (_vertexShader)
    {
        _vertexShader->Release();
        _vertexShader = NULL;
    }
}
void ColorShader::outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd,  WCHAR* shaderFile)
{
    using std::ofstream;
    char*         compileError;
    unsigned long bufferSize;
    ofstream      file;

    compileError = (char*)(errorMessage->GetBufferPointer());
    bufferSize   = errorMessage->GetBufferSize();
    file.open("shader-error.txt");
    for (int i = 0; i < bufferSize; i++)
    {
        file << compileError[i];
    }
    file.close();
    errorMessage->Release();
    errorMessage = NULL;

    WCHAR wstr[100];
    wsprintf(wstr, L"%s\n셰이더 파일을 컴파일하는데 문제가 생겼습니다.\n", shaderFile);
    MessageBox(hwnd, wstr, L"Error", NULL);
}
void ColorShader::setShaderParameter(ID3D11DeviceContext* deviceContext, D3DXMATRIX mtxWorld, D3DXMATRIX mtxView, D3DXMATRIX mtxProjection)
{
    HRESULT                  result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    CBuffer*                 cBuffer;
    unsigned int             bufferNumber;

    // transpose (전치행렬)
    // 행렬을 transpose해 셰이더에서 사용할 수 있게 만든다.
    D3DXMatrixTranspose(&mtxWorld, &mtxWorld);
    D3DXMatrixTranspose(&mtxView, &mtxView);
    D3DXMatrixTranspose(&mtxProjection, &mtxProjection);

    // 상수버퍼의 내용을 사용할 수 있도록 잠금
    result = deviceContext->Map(_cBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if(FAILED(result))
        MessageBox(NULL, L"ID3D11DeviceContext::Map()", L"Error", NULL);

    // 상수버퍼의 데이터에 대한 포인터를 가져온다.
    cBuffer                = (CBuffer*)mappedResource.pData;
    cBuffer->mtxWorld      = mtxWorld;
    cBuffer->mtxView       = mtxView;
    cBuffer->mtxProjection = mtxProjection;

    // 상수버퍼의 잠금을 푼다.
    deviceContext->Unmap(_cBuffer, 0);

    // 정점 셰이더에서의 상수버퍼의 위치를 설정한다.
    bufferNumber = 0;

    // 정점셰이더의 상수버퍼를 바뀐 값으로 바꾼다.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_cBuffer);
}
void ColorShader::renderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
    deviceContext->IASetInputLayout(_inputLayout);     // 정점 입력 레이아웃 생성
    deviceContext->VSSetShader(_vertexShader, NULL, 0);// 정점&픽셀 셰이더 생성
    deviceContext->PSSetShader(_pixelShader, NULL, 0); // 정점&픽셀 셰이더 생성
    deviceContext->DrawIndexed(indexCount, 0, 0);      // 그리기
}