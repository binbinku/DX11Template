#include "Shader.h"


//Shader
ID3D11VertexShader* g_pVertexShader;
ID3D11PixelShader* g_pPixelShader;
ID3D11InputLayout* g_pInputLayout;


//ConstantBuffer
ID3D11Buffer* vsConstantBuffer;
VSConstantBufferData* vsCBData;

void LoadShader(ID3D11Device* mainDevice)
{
    ID3DBlob* pErrorBlob = nullptr;

    //VS
    ID3DBlob* pVertexShaderBlob = nullptr;
    HRESULT hr = D3DCompileFromFile(L"Data/Shader/testVS.hlsl", nullptr, nullptr, "main", "vs_5_0", 0, 0, &pVertexShaderBlob, &pErrorBlob);
    if (FAILED(hr))
    {
        if (pErrorBlob)
        {
            MessageBoxA(0, (char*)pErrorBlob->GetBufferPointer(), 0, 0);
            pErrorBlob->Release();
        }
        return;
    }

    hr = mainDevice->CreateVertexShader(pVertexShaderBlob->GetBufferPointer(), pVertexShaderBlob->GetBufferSize(), nullptr, &g_pVertexShader);
    if (FAILED(hr)) { MessageBoxA(0, "CreateVertexShader Failed!", 0, 0); }



    //PS
    ID3DBlob* pPixelShaderBlob = nullptr;
    hr = D3DCompileFromFile(L"Data/Shader/testPS.hlsl", nullptr, nullptr, "main", "ps_5_0", 0, 0, &pPixelShaderBlob, &pErrorBlob);
    if (FAILED(hr))
    {
        if (pErrorBlob)
        {
            MessageBoxA(0, (char*)pErrorBlob->GetBufferPointer(), 0, 0);
            pErrorBlob->Release();
        }
        return;
    }

    hr = mainDevice->CreatePixelShader(pPixelShaderBlob->GetBufferPointer(), pPixelShaderBlob->GetBufferSize(), nullptr, &g_pPixelShader);
    if (FAILED(hr)){MessageBoxA(0,"CreatePixelShader Failed!",0,0);}



    //InputLayout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    hr = mainDevice->CreateInputLayout(layout, 3, pVertexShaderBlob->GetBufferPointer(), pVertexShaderBlob->GetBufferSize(), &g_pInputLayout);
    if (FAILED(hr)) { MessageBoxA(0, "CreateInputLayout Failed!", 0, 0); }

}

void SetUpShader(ID3D11Device* mainDevice)
{
    LoadShader(mainDevice);

    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.Usage = D3D11_USAGE_DEFAULT;
    cbDesc.ByteWidth = sizeof(VSConstantBufferData);
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    vsCBData = new VSConstantBufferData();
    
    vsCBData->worldMatrix = XMMatrixTranspose(worldMatrix);
    vsCBData->viewMatrix = XMMatrixTranspose(viewMatrix);
    vsCBData->projMatrix = XMMatrixTranspose(projMatrix);

    D3D11_SUBRESOURCE_DATA cbData = {};
    cbData.pSysMem = vsCBData;

    HRESULT hr = mainDevice->CreateBuffer(&cbDesc, &cbData, &vsConstantBuffer);
    if (FAILED(hr)) { MessageBoxA(0, "CreateBuffer Failed!", 0, 0); }

}

void UpdateVSConstantBuffer(ID3D11DeviceContext* mainDeviceContext)
{
    vsCBData->worldMatrix = XMMatrixTranspose(worldMatrix);
    vsCBData->viewMatrix = XMMatrixTranspose(viewMatrix);
    vsCBData->projMatrix = XMMatrixTranspose(projMatrix);

    mainDeviceContext->UpdateSubresource(vsConstantBuffer, 0, 0, vsCBData, 0, 0);
}
