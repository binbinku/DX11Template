#pragma once
#include "../pch.h"

using namespace DirectX;

struct Vertex
{
    XMFLOAT3 Position;
    XMFLOAT2 TexCoord;
    XMFLOAT3 Normal;
};

extern std::vector<Vertex> vertices;
extern std::vector<UINT> indices;

extern ID3D11Buffer* g_pVertexBuffer;
extern ID3D11Buffer* g_pIndexBuffer;

extern DirectX::XMMATRIX worldMatrix;

void CreateSphereGeometry(float radius, int slices, int stacks);

void SetupMesh(ID3D11Device* mainDevice);

