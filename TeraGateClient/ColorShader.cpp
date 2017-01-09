#include"ColorShader.h"
ColorShader::ColorShader()
{
    _vertexShader = 0;
    _pixelShader  = 0;
    _layout       = 0;
    _matrixBuffer = 0;
}
ColorShader::ColorShader(const ColorShader& other)
{

}
ColorShader::~ColorShader()
{

}

/*****************************************************************************************************
init() 함수는 셰이더의 초기화를 수행하는 함수를 호출한다.
color.vs, color.ps의 HLSL 셰이더 파일의 이름을 넘겨준다.
*****************************************************************************************************/
bool ColorShader::init(ID3D11Device* device, HWND window)
{
    bool result;
    result = initShader(device, window, L"Color.vs", L"Color.ps");
    if (!result)
    {
        MessageBox(window, L"셰이더 초기화 실패", L"Error", MB_OK);
        return false;
    }

    return true;
}
// 정점&픽셀 셰이더 및 그와 관련된 것들을 반환
void ColorShader::shut()
{
    shutShader();
    return;
}
bool ColorShader::render(
    ID3D11DeviceContext* deviceContext,
    int                  indexCount,
    D3DXMATRIX           worldMatrix,
    D3DXMATRIX           viewMatrix,
    D3DXMATRIX           projectionMatrix)
{
    bool result;

    // 렌더링에 사용할 셰이더의 인자를 입력
    result = setShaderParameter(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
    if (!result)
        return false;

    // 셰이더를 이용하여 준비된 버퍼를 그린다.
    renderShader(deviceContext, indexCount);

    return true;
}
/*****************************************************************************************************
initShader() 함수 내에서 실제로 셰이더 파일을 불러오고
DirectX와 GPU에서 사용가능하도록 하는 일을 한다.
어떻게 정점 버퍼의 데이터가 GPU에서 어떠헥 사용되는지 볼 수 있다.
또한 DirectX와 GPU에서 사용가능하도록 한다.
이 레이아웃은 Model 클래스 뿐 아니라 color.vs에 선언된 VertexType과 일치할 필요가 있다.
*****************************************************************************************************/
bool ColorShader::initShader(ID3D11Device* device, HWND window, WCHAR* vsFileName, WCHAR* psFileName)
{
    HRESULT                  result;
    ID3D10Blob*              errorMessage;
    ID3D10Blob*              vertexShaderBuffer;
    ID3D10Blob*              pixelShaderBuffer;
    D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
    unsigned int             numElements;
    D3D11_BUFFER_DESC        matrixBufferDesc;

    // 이 함수에서 사용하는 포인터들을 null로 설정한다.
    errorMessage       = 0;
    vertexShaderBuffer = 0;
    pixelShaderBuffer  = 0;

    /*****************************************************************************************************
    셰이더 프로그램을 버퍼로 컴파일한다.
    셰이더 파일 이름, 셰이더 이름, 셰이더의 버전(DirectX11에서는 5.0), 셰이더가 컴파일될 버퍼를 인자로 넘겨준다.
    만약 컴파일이 실패하면 에러 메시지를 errorMessage문자열에 출력하고 이것을 다른 함수로 볼 수 있다.
    만약 errorMessage없이 실패한다면 셰이더 파일을 찾지 못했다는 뜻이다.
    *****************************************************************************************************/
    // 정점 셰이더를 컴파일
    result = D3DX11CompileFromFile(
        vsFileName, NULL, NULL,
        "ColorVertexShader", "vs_5_0",
        D3D10_SHADER_ENABLE_STRICTNESS,
        0, NULL,
        &vertexShaderBuffer,
        &errorMessage, NULL);
    if (FAILED(result))
    {
        // 셰이더가 컴파일에 실패하면 에러 메시지를 기록
        if (errorMessage)
            outputShaderErrorMessage(errorMessage, window, vsFileName);
        // 에러 메시지가 없다면 셰이더 파일을 찾지 못한 것
        else
            MessageBox(window, vsFileName, L"Missing Shader File", MB_OK);
        return false;
    }
    // 픽셀 셰이더를 컴파일
    result = D3DX11CompileFromFile(
        psFileName, NULL, NULL,
        "ColorPixelShader", "ps_5_0",
        D3D10_SHADER_ENABLE_STRICTNESS,
        0, NULL,
        &pixelShaderBuffer,
        &errorMessage, NULL);
    if (FAILED(result))
    {
        // 셰이더 컴파일이 실패하면 에러 메시지를 기록
        if (errorMessage)
            outputShaderErrorMessage(errorMessage, window, psFileName);
        // 에러 메시지가 없다면 셰이더 파일을 찾지 못한 것
        else
            MessageBox(window, psFileName, L"Missing Shader File", MB_OK);
        return false;
    }

    /*****************************************************************************************************
    정점&픽셀 셰이더가 버퍼로 잘 컴파일되면 이를 이용해 셰이더 객체를 만들 수 있다.
    여기서 나온 포인터를 정점 셰이더와 픽셀 셰이더의 인터페이스로서 사용할 것.
    *****************************************************************************************************/
    // 버퍼로부터 정점 셰이더를 생성
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
    if (FAILED(result))
        return false;
    // 버퍼로부터 픽셀 셰이더를 생성
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);
    if (FAILED(result))
        return false;

    /*****************************************************************************************************
    셰이더에서 사용할 정점 데이터의 레이아웃 생성.
    현재 셰이더에서는 위치&색상 벡터를 사용하므로 레이아웃에 각각의 벡터의 크기를 포함하는 두 레이아웃을 만든다.
    POSITION을 먼저, COLOR를 두 번째로 처리한다.
    위치 벡터는 DXGI_FORMAT_R32G32B32_FLOAT    사용
    색상 벡터는 DXGI_FORMAT_R32G32B32A32_FLOAT 사용

    버퍼에 데이터가 어떻게 배열되는지 알려주는 AlignedByteOffset은 처음 12byte를 위치 벡터에 사용하고
    다음 16byte를 색상으로 사용할 것임을 알려줘야 하는데, 각 요소가 어디서 시작되는지를 나타낸다.
    직접 값을 입력하기보다 D3D11_APPEND_ALIGNED_ELEMENT를 지정하면 자동으로 알아내게 된다.
    *****************************************************************************************************/
    // 정점 입력 레이아웃 description 작성
    // Model 클래스의 셰이더에 있는 VertexType과 일치해야 한다.
    polygonLayout[0].SemanticName           = "POSITION";
    polygonLayout[0].SemanticIndex          = 0;
    polygonLayout[0].Format                 = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot              = 0;
    polygonLayout[0].AlignedByteOffset      = 0;
    polygonLayout[0].InputSlotClass         = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate   = 0;

    polygonLayout[1].SemanticName           = "COLOR";
    polygonLayout[1].SemanticIndex          = 0;
    polygonLayout[1].Format                 = DXGI_FORMAT_R32G32B32A32_FLOAT;
    polygonLayout[1].InputSlot              = 0;
    polygonLayout[1].AlignedByteOffset      = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass         = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[1].InstanceDataStepRate   = 0;

    // 레이아웃의 요소 갯수를 가져온다.
    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

    // 정점 입력 레이아웃을 생성
    result = device->CreateInputLayout(
        polygonLayout,
        numElements,
        vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(),
        &_layout);
    if (FAILED(result))
    {
        return false;
    }

    // 더 이상 사용되지 않는 정점 셰이더 퍼버와 픽셀 셰이더 버퍼를 해제.
    vertexShaderBuffer->Release();
    vertexShaderBuffer = 0;

    pixelShaderBuffer->Release();
    pixelShaderBuffer = 0;

    /*****************************************************************************************************
    셰이더를 사용하기 위한 마지막 단계, 상수버퍼(constant buffer)
    매 프레임마다 상수버퍼를 업데이트하기 때문에 버퍼의 사용은 동적이 될 필요가 있다.
    BindFlags를 이용해 상수버퍼로 이 버퍼를 사용한다고 설정한다.
    CPUAccessFlgas도 용도에 맞추어야 하기 때문에 D3D11_CPU_ACCESS_WRITE로 설정되야 한다.
    이 description이 채워지면 상수버퍼의 인텊이스를 만들고
    이와 SetShaderParameters 함수를 이용해 셰이더의 내부 변수들에 접근할 수 있도록 한다.
    *****************************************************************************************************/
    // 정점 셰이더에 있는 행렬 상수 버퍼의 description을 작성
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    // 상수 버퍼 포인터를 만들어 이 클래스에서 정점 셰이더 상수 버퍼에 접근할 수 있게 된다
    result = device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer);
    if (FAILED(result))
         return false;
 
    return true;
}
/*****************************************************************************************************
initShader()에서 생성했던 4개의 인터페이스를 정리한다.
4개의 인터페이스 : 상수버퍼, 레이아웃, 픽셀셰이더, 정점셰이더
*****************************************************************************************************/
void ColorShader::shutShader()
{
    // 상수 버퍼 해제
    if (_matrixBuffer)
    {
        _matrixBuffer->Release();
        _matrixBuffer = 0;
    }

    // 레이아웃 해제
    if (_layout)
    {
        _layout->Release();
        _layout = 0;
    }

    // 픽셀 셰이더 해제
    if (_pixelShader)
    {
        _pixelShader->Release();
        _pixelShader = 0;
    }

    // 정점 셰이더 해제
    if (_vertexShader)
    {
        _vertexShader->Release();
        _vertexShader = 0;
    }

    return;
}
/*****************************************************************************************************
정점&픽셀 셰이더의 컴파일 도중 오류발생시 에러메시지를 출력
*****************************************************************************************************/
void ColorShader::outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND window, WCHAR* shaderFileName)
{
    char*         compileErrors;
    unsigned long bufferSize, i;
    ofstream      fout;


    // 에러 메세지를 담고 있는 문자열 버퍼의 포인터를 가져온다
    compileErrors = (char*)(errorMessage->GetBufferPointer());

    // 메세지 길이를 가져온다.
    bufferSize = errorMessage->GetBufferSize();

    // 파일을 열고 안에 메세지를 기록
    fout.open("shader-error.txt");

    // 에러 메세지를 작성
    for (i = 0; i<bufferSize; i++)
    {
        fout << compileErrors[i];
    }

    // 파일을 닫는다.
    fout.close();

    // 에러 메세지를 반환
    errorMessage->Release();
    errorMessage = 0;

    // 컴파일 에러가 있음을 팝업 메세지로 알린다.
    MessageBox(window, L"Error compiling shader.  Check shader-error.txt for message.", shaderFileName, MB_OK);

    return;
}
/*****************************************************************************************************
셰이더의 전역변수를 쉽게 다루기 위해 사용
*****************************************************************************************************/
bool ColorShader::setShaderParameter(
    ID3D11DeviceContext* deviceContext,
    D3DXMATRIX           worldMatrix,
    D3DXMATRIX           viewMatrix,
    D3DXMATRIX           projectionMatrix)
{
    HRESULT                  result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixBufferType*        dataPtr;
    unsigned int             bufferNumber;


    // 행렬을 transpose하여 셰이더에서 사용할 수 있게 한다.
    D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
    D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
    D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

    // 상수 버퍼의 내용을 쓸 수 있도록 잠금
    result = deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
        return false;

    // 상수 버퍼의 데이터에 대한 포인터를 가져온다.
    dataPtr = (MatrixBufferType*)mappedResource.pData;

    // 상수 버퍼에 행렬을 복사
    dataPtr->worldMatrix      = worldMatrix;
    dataPtr->viewMatrix       = viewMatrix;
    dataPtr->projectionMatrix = projectionMatrix;

    // 상수 버퍼의 잠금해제
    deviceContext->Unmap(_matrixBuffer, 0);

    // 정점 셰이더에서의 상수 버퍼 위치설정
    bufferNumber = 0;

    // 정점 셰이더의 상수 버퍼를 바뀐 값으로 바꾼다.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);

    return true;
}
/*****************************************************************************************************
render()함수에서 setShaderParameter()보다 먼저 호출되어 셰이더의 인자들을 올바로 세팅하게 된다.

입력 레이아웃을 입력 어셈블러에 연결하여, GPU정점 버퍼의 자료구조를 알게 된다.
이후 정점버퍼를 그리기 위한 정점&픽셀 셰이더를 설정한다.
셰이더가 설정되면 D3D디바이스 컨텍스트에서 DirectX11의 DrawIndexed() 함수를 사용해
삼각형을 그려내게 된다.
이 함수가 호출된 이후에, 현재 튜토리얼 중인 초록색 삼각형이 그려지게 된다.
*****************************************************************************************************/
void ColorShader::renderShader(
    ID3D11DeviceContext* deviceContext,
    int                 indexCount)
{
    // 정점 입력 레이아웃을 설정
    deviceContext->IASetInputLayout(_layout);

    // 삼각형을 그릴 정점&픽셀 셰이더를 설정.
    deviceContext->VSSetShader(_vertexShader, NULL, 0);
    deviceContext->PSSetShader(_pixelShader, NULL, 0);

    // 렌더
    deviceContext->DrawIndexed(indexCount, 0, 0);

    return;
}
