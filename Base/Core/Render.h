#pragma once
#include "../pch.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

using namespace DirectX;

//DX11
extern ID3D11Device* mainDevice;
extern ID3D11DeviceContext* mainDeviceContext;


//DXGI
extern IDXGIFactory* g_pFactory;
extern IDXGISwapChain* g_pSwapChain;
extern ID3D11Texture2D* g_pBackBuffer;
extern ID3D11RenderTargetView* g_pRenderTargetView;


void SetupDX11(HWND hwnd);

void Start(HWND hwnd);

void Update();
