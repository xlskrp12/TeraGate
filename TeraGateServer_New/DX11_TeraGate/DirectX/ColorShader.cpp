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
    // ������ ����� ���̴� ���� �Է�
    setShaderParameter(deviceContext, mtxWorld, mtxView, mtxProjection);
    // ���̴��� �غ�� ���۸� ����
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
    //���������̴� ����
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
            // �����޽����� ���ٸ� ������ ã�� ���� ��.
            WCHAR wstr[100];
            wsprintf(wstr, L"D3DX11ComfileFromFile()\n������ ������ ã�� ���߽��ϴ�.\n%s\n", vsFileName);
            MessageBox(NULL, wstr, L"Error", NULL);
        }
    }
    //�� ID3D11Device::CreateVertexShader(
    // [in]            const void*          pHsaderByteCode, // �����ϵ� ���̴��� ����Ű�� ������
    // [in]            SIZE_T               VytecodeLength,  // �����ϵ� ���ؽ� ���̴� ũ��
    // [in, optional ] ID3D11ClassLinkage*  pClassLinkage,   // ���� �������̽�(?)�� ���� ������
    // [out, optional] ID3D11VertexShader** ppVertexShader   // ID3D11VertexShader �������̽��� ����ų ������.
    // );
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11Device::CreateVertexShader()", L"Error", NULL);
    //���ȼ����̴� ����
    result = D3DX11CompileFromFile(psFileName, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
    if (FAILED(result))
    {
        if (errorMessage)
        {
            outputShaderErrorMessage(errorMessage, hwnd, psFileName);
        }
        else
        {
            // �����޽����� ���ٸ� ������ ã�� ���� ��.
            WCHAR wstr[100];
            wsprintf(wstr, L"D3DX11ComfileFromFile()\n������ ������ ã�� ���߽��ϴ�.\n%s\n", psFileName);
            MessageBox(NULL, wstr, L"Error", NULL);
        }
    }
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11Device::CreatePixelShader()", L"Error", NULL);
    //����̴��� ����� ��ǲ ���̾ƿ� ����
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
    inputDesc[1].AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT; // ���ۿ� �����Ͱ� ��� �迭�Ǵ����� �˷�����ϴµ�, �ڵ����� �˾Ƴ���� ��
    inputDesc[1].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
    inputDesc[1].InstanceDataStepRate = 0;
    //����ǲ ��ũ�������� ��ǲ ���̾ƿ� ����
    // ���̾ƿ� ���� �� ����&�ȼ� ���̴� ���۴� ������� �����Ƿ� ����
    inputNumber = sizeof(inputDesc) / sizeof(inputDesc[0]);
    result = device->CreateInputLayout(inputDesc, inputNumber, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_inputLayout);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11Device::CreateInputLayout()", L"Error", NULL);
    vertexShaderBuffer->Release();
    vertexShaderBuffer = NULL;
    pixelShaderBuffer->Release();
    pixelShaderBuffer  = NULL;
    // ���̴��� ����ϱ� ���� �������(constant buffer)
    // �������̴��� �ִ� ��� ������� ��ũ���� �ۼ�
    cBufferDesc.Usage                = D3D11_USAGE_DYNAMIC;
    cBufferDesc.ByteWidth            = sizeof(CBuffer);
    cBufferDesc.BindFlags            = D3D11_BIND_CONSTANT_BUFFER;// ������۸� �� ���۷� �Ѵٰ� ����.
    cBufferDesc.CPUAccessFlags       = D3D11_CPU_ACCESS_WRITE;    // ������� �뵵�� �´� ��
    cBufferDesc.MiscFlags            = 0;
    cBufferDesc.StructureByteStride  = 0;
    // ������� ������(_cBuffer)�� ����� �� Ŭ�������� ���� ���̴� ������ۿ� ������ �� �ְ�
    result = device->CreateBuffer(&cBufferDesc, NULL, &_cBuffer);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11Device::CreateBuffer()", L"Error", NULL);
}
void ColorShader::releaseShader()
{
    // �������, ���̾ƿ�, �ȼ�&���ؽ� ���̴� �� ���� ����
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
    wsprintf(wstr, L"%s\n���̴� ������ �������ϴµ� ������ ������ϴ�.\n", shaderFile);
    MessageBox(hwnd, wstr, L"Error", NULL);
}
void ColorShader::setShaderParameter(ID3D11DeviceContext* deviceContext, D3DXMATRIX mtxWorld, D3DXMATRIX mtxView, D3DXMATRIX mtxProjection)
{
    HRESULT                  result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    CBuffer*                 cBuffer;
    unsigned int             bufferNumber;

    // transpose (��ġ���)
    // ����� transpose�� ���̴����� ����� �� �ְ� �����.
    D3DXMatrixTranspose(&mtxWorld, &mtxWorld);
    D3DXMatrixTranspose(&mtxView, &mtxView);
    D3DXMatrixTranspose(&mtxProjection, &mtxProjection);

    // ��������� ������ ����� �� �ֵ��� ���
    result = deviceContext->Map(_cBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if(FAILED(result))
        MessageBox(NULL, L"ID3D11DeviceContext::Map()", L"Error", NULL);

    // ��������� �����Ϳ� ���� �����͸� �����´�.
    cBuffer                = (CBuffer*)mappedResource.pData;
    cBuffer->mtxWorld      = mtxWorld;
    cBuffer->mtxView       = mtxView;
    cBuffer->mtxProjection = mtxProjection;

    // ��������� ����� Ǭ��.
    deviceContext->Unmap(_cBuffer, 0);

    // ���� ���̴������� ��������� ��ġ�� �����Ѵ�.
    bufferNumber = 0;

    // �������̴��� ������۸� �ٲ� ������ �ٲ۴�.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_cBuffer);
}
void ColorShader::renderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
    deviceContext->IASetInputLayout(_inputLayout);     // ���� �Է� ���̾ƿ� ����
    deviceContext->VSSetShader(_vertexShader, NULL, 0);// ����&�ȼ� ���̴� ����
    deviceContext->PSSetShader(_pixelShader, NULL, 0); // ����&�ȼ� ���̴� ����
    deviceContext->DrawIndexed(indexCount, 0, 0);      // �׸���
}