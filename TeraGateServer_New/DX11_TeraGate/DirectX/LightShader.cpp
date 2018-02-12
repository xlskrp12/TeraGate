#include "LightShader.h"
LightShader::LightShader()
{
    _vs           = NULL;
    _ps           = NULL;
    _inputLayout  = NULL;
    _samplerState = NULL;
    _vsCBuffer    = NULL;
    _psCBuffer    = NULL;
}
LightShader::LightShader(const LightShader& lightShader)
{
}
LightShader::~LightShader()
{
}
void LightShader::init(ID3D11Device* device, HWND hwnd)
{
    initShader(device, hwnd, L"Shader/LightVS.hlsl", L"Shader/LightPS.hlsl");
}
void LightShader::release()
{
    releaseShader();
}
void LightShader::render(
    ID3D11DeviceContext *deviceContext, int indexCount,
    D3DXMATRIX mtxWorld, D3DXMATRIX mtxView, D3DXMATRIX mtxProjection,
    ID3D11ShaderResourceView *texture, D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColor)
{
    setShaderParameter(deviceContext, mtxWorld, mtxView, mtxProjection, texture, lightDirection, diffuseColor);
    renderShader(deviceContext, indexCount);
}
void LightShader::initShader(
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
    //���������̴� ����
    // ���̴� ���Ϸε�
    result = D3DX11CompileFromFile(vsFileName, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vs, &errorMessage, NULL);
    if (FAILED(result))
    {
        if (errorMessage)
        {
            outputShaderErrorMessage(errorMessage, hwnd, vsFileName);
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
    // ���̴� ����
    result = device->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), NULL, &_vs);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11Device::CreateVertexShader()", L"Error", NULL);
    //���ȼ����̴� ����
    // ���̴� ���Ϸε�
    result = D3DX11CompileFromFile(psFileName, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &ps, &errorMessage, NULL);
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
    // ���̴� ����
    result = device->CreatePixelShader(ps->GetBufferPointer(), ps->GetBufferSize(), NULL, &_ps);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11Device::CreatePixelShader()", L"Error", NULL);

    //������ �Է·��̾ƿ� ����
    inputLayoutDesc[0].SemanticName         = "POSITION";//SemanticName�����δ� POSITION, COLOR, TEXCOORD, NORMAL ���� �� �� �ִ�.
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
    inputLayoutDesc[1].AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT; // ���ۿ� �����Ͱ� ��� �迭�Ǵ����� �˷�����ϴµ�, �ڵ����� �˾Ƴ���� ��
    inputLayoutDesc[1].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
    inputLayoutDesc[1].InstanceDataStepRate = 0;
    inputLayoutDesc[2].SemanticName         = "NORMAL";
    inputLayoutDesc[2].SemanticIndex        = 0;
    inputLayoutDesc[2].Format               = DXGI_FORMAT_R32G32B32_FLOAT;// DXGI_FORMAT_R32G32B32_FLOAT, DXGI_FORMAT_R32G32_FLOAT
    inputLayoutDesc[2].InputSlot            = 0;
    inputLayoutDesc[2].AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT; // ���ۿ� �����Ͱ� ��� �迭�Ǵ����� �˷�����ϴµ�, �ڵ����� �˾Ƴ���� ��
    inputLayoutDesc[2].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
    inputLayoutDesc[2].InstanceDataStepRate = 0;
    inputNumber = sizeof(inputLayoutDesc) / sizeof(inputLayoutDesc[0]);
    result = device->CreateInputLayout(inputLayoutDesc, inputNumber, vs->GetBufferPointer(), vs->GetBufferSize(), &_inputLayout);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11Device::CreateInputLayout()", L"Error", NULL);

    //������&�ȼ� ���̴�, ���� �Է·��̾ƿ� ���� �� ������� �ʴ� �ڿ� ����
    vs->Release();
    vs = NULL;
    ps->Release();
    ps = NULL;

    //���������̴� ������� ����
    vsCBufferDesc.Usage                = D3D11_USAGE_DYNAMIC;
    vsCBufferDesc.ByteWidth            = sizeof(VSCBuffer);
    vsCBufferDesc.BindFlags            = D3D11_BIND_CONSTANT_BUFFER;// ������۸� �� ���۷� �Ѵٰ� ����.
    vsCBufferDesc.CPUAccessFlags       = D3D11_CPU_ACCESS_WRITE;    // ������� �뵵�� �´� ��
    vsCBufferDesc.MiscFlags            = 0;
    vsCBufferDesc.StructureByteStride  = 0;
    result = device->CreateBuffer(&vsCBufferDesc, NULL, &_vsCBuffer);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11Device::CreateBuffer()", L"Error", NULL);
    
    //���ȼ����̴� ������� ����
    psCBufferDesc.Usage               = D3D11_USAGE_DYNAMIC;
    psCBufferDesc.ByteWidth           = sizeof(PSCBuffer);
    psCBufferDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
    psCBufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
    psCBufferDesc.MiscFlags           = 0;
    psCBufferDesc.StructureByteStride = 0;
    result = device->CreateBuffer(&psCBufferDesc, NULL, &_psCBuffer);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11Device::CreateBuffer()", L"Error", NULL);

    //����÷� ������Ʈ ����
    samplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;// ���÷��� ���Ȯ��+�ӷ��� ���ø��� ��������(linear interpolation)��� // ó������ �� �������� ����Ƽ�� ����.
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
void LightShader::releaseShader()
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
void LightShader::outputShaderErrorMessage(ID3D10Blob *errorMessage, HWND hwnd, WCHAR *shaderFileName)
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
    wsprintf(wstr, L"%s\n���̴� ������ �������ϴµ� ������ ������ϴ�.\n", shaderFileName);
    MessageBox(hwnd, wstr, L"Error", NULL);
}
// �ؽ��� ���ҽ��� �����͸� �޾� ���̴��� ���
// �ؽ��Ĵ� �ݵ�� ������ ���� �������־�� ���� ����.
void LightShader::setShaderParameter(
    ID3D11DeviceContext      *deviceContext,
    D3DXMATRIX               mtxWorld,
    D3DXMATRIX               mtxView,
    D3DXMATRIX               mtxProjection,
    ID3D11ShaderResourceView *texture,
    D3DXVECTOR3              lightDirection,
    D3DXVECTOR4              diffuseColor)
{
    HRESULT                  result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    unsigned int             bufferNumber;
    VSCBuffer                *vsCBuffer;
    PSCBuffer                *psCBuffer;

    //�尢 ��ĵ��� ��ġ(transpose)�� ���̴����� ����� �� �ֵ��� �غ�
    D3DXMatrixTranspose(&mtxWorld, &mtxWorld);
    D3DXMatrixTranspose(&mtxView, &mtxView);
    D3DXMatrixTranspose(&mtxProjection, &mtxProjection);

    //���������̴� ������� set
    // ������۸� map�� ������� ��ȯ
    result = deviceContext->Map(_vsCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11DeviceContext::Map()", L"Error", NULL);    
    // ������۷� �����͸� ī��
    vsCBuffer = (VSCBuffer*)mappedResource.pData;// ������� �����Ϳ� ���� �����͸� �����´�.
    vsCBuffer->mtxWorld      = mtxWorld;
    vsCBuffer->mtxView       = mtxView;
    vsCBuffer->mtxProjection = mtxProjection;    
    // ������۸� unmap�� �б���� ��ȯ
    deviceContext->Unmap(_vsCBuffer, 0);
    // �������̴� ������� set
    bufferNumber = 0;// ���̴��� �ִ� ������� �������� 0����
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_vsCBuffer);

    //���ȼ����̴��� �ؽ��� ���ҽ� set
    deviceContext->PSSetShaderResources(0, 1, &texture);

    //���ȼ����̴� ������� set
    // ������۸� map�� ������� ��ȯ
    result = deviceContext->Map(_psCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
        MessageBox(NULL, L"ID3D11DeviceContext::Map()", L"Error", NULL);
    // ������۷� �����͸� ī��
    psCBuffer = (PSCBuffer*)mappedResource.pData;// ������� �����Ϳ� ���� �����͸� �����´�.
    psCBuffer->diffuseColor   = diffuseColor;
    psCBuffer->lightDirection = lightDirection;
    psCBuffer->padding        = 0.f;
    // ������۸� unmap�� �б���� ��ȯ
    deviceContext->Unmap(_psCBuffer, 0);
    bufferNumber = 0;// ���̴��� �ִ� ������� �������� 0����
    deviceContext->PSSetConstantBuffers(bufferNumber, 1, &_psCBuffer);
}
void LightShader::renderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
    deviceContext->IASetInputLayout(_inputLayout);     // �Է� ���̾ƿ�         set
    deviceContext->VSSetShader(_vs, NULL, 0);          // �������̴�            set
    deviceContext->PSSetShader(_ps, NULL, 0);          // �ȼ����̴�            set
    deviceContext->PSSetSamplers(0, 1, &_samplerState);// �ȼ����̴� ���÷� ���� set 
    deviceContext->DrawIndexed(indexCount, 0, 0);      // �׸���
}