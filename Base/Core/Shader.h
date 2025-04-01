#pragma once
#include "../pch.h"
#include "Mesh.h"
#include "Camera.h"


using namespace DirectX;

struct VSConstantBufferData
{
    XMMATRIX worldMatrix;
    XMMATRIX viewMatrix;
    XMMATRIX projMatrix;
};

//Shader
extern ID3D11VertexShader* g_pVertexShader;
extern ID3D11PixelShader* g_pPixelShader;
extern ID3D11InputLayout* g_pInputLayout;


//ConstantBuffer
extern ID3D11Buffer* vsConstantBuffer;
extern VSConstantBufferData* vsCBData;


void LoadShader(ID3D11Device* mainDevice);

void SetUpShader(ID3D11Device* mainDevice);

void UpdateVSConstantBuffer(ID3D11DeviceContext* mainDeviceContext);
