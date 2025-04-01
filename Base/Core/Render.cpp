#include "Render.h"


//DX11
ID3D11Device* mainDevice;
ID3D11DeviceContext* mainDeviceContext;


//DXGI
IDXGIFactory* g_pFactory;
IDXGISwapChain* g_pSwapChain;
ID3D11Texture2D* g_pBackBuffer;
ID3D11RenderTargetView* g_pRenderTargetView;




void SetupDX11(HWND hwnd)
{
    //[1.Setup DX11]============================================================================
    HRESULT hr = D3D11CreateDevice(nullptr,D3D_DRIVER_TYPE_HARDWARE,nullptr,0,nullptr,0,D3D11_SDK_VERSION,&mainDevice,nullptr,&mainDeviceContext);
    if (FAILED(hr)) MessageBox(0, "D3D11CreateDevice Failed.", 0, 0);


    //[2.Setup SwapChain]============================================================================
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 2; // 双缓冲
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32位颜色格式
    swapChainDesc.BufferDesc.Width = 1280; // 窗口宽度
    swapChainDesc.BufferDesc.Height = 800; // 窗口高度
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd; // 窗口句柄
    swapChainDesc.SampleDesc.Count = 1; // 不使用多采样
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE; // 窗口模式

    hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&g_pFactory);
    if (FAILED(hr)) MessageBox(0, "CreateDXGIFactory Failed.", 0, 0);


    hr = g_pFactory->CreateSwapChain(mainDevice, &swapChainDesc, &g_pSwapChain);
    if (FAILED(hr)) MessageBox(0, "CreateSwapChain Failed.", 0, 0);


    //[3.Setup TargetView]============================================================================
    g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&g_pBackBuffer);

    hr = mainDevice->CreateRenderTargetView(g_pBackBuffer, nullptr, &g_pRenderTargetView);
    if (FAILED(hr)) MessageBox(0, "CreateRenderTargetView Failed.", 0, 0);

    mainDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);


    //[4.Setup ViewPort]============================================================================
    D3D11_VIEWPORT vp;
    vp.Width = 1280.0f;
    vp.Height = 800.0f;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    
    mainDeviceContext->RSSetViewports(1, &vp);

}


void Start(HWND hwnd)
{
    SetupDX11(hwnd);

    SetupCamera();

    SetupMesh(mainDevice);

    SetUpShader(mainDevice);
}


void Update()
{
    float clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
    mainDeviceContext->ClearRenderTargetView(g_pRenderTargetView, clearColor);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    mainDeviceContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    mainDeviceContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    mainDeviceContext->IASetInputLayout(g_pInputLayout);

    mainDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    mainDeviceContext->VSSetShader(g_pVertexShader, nullptr, 0);

    mainDeviceContext->PSSetShader(g_pPixelShader, nullptr, 0);

    UpdateVSConstantBuffer(mainDeviceContext);

    mainDeviceContext->VSSetConstantBuffers(0, 1, &vsConstantBuffer);

    mainDeviceContext->DrawIndexed(indices.size(), 0, 0);

    g_pSwapChain->Present(1, 0);

}


