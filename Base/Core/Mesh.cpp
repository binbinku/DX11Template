#include "Mesh.h"

std::vector<Vertex> vertices;
std::vector<UINT> indices;

ID3D11Buffer* g_pVertexBuffer;
ID3D11Buffer* g_pIndexBuffer;

DirectX::XMMATRIX worldMatrix;




void CreateSphereGeometry(float radius, int slices, int stacks)
{
    const float PI = 3.14159265358979323846f;

    for (int i = 0; i <= stacks; ++i)
    {
        float phi = PI * i / stacks;
        for (int j = 0; j <= slices; ++j)
        {
            float theta = 2.0f * PI * j / slices;

            Vertex v;
            v.Position.x = radius * sin(phi) * cos(theta);
            v.Position.y = radius * sin(phi) * sin(theta);
            v.Position.z = radius * cos(phi);

            DirectX::XMVECTOR normalVector = DirectX::XMVector3Normalize(DirectX::XMVectorSet(v.Position.x, v.Position.y, v.Position.z, 0.0f));
            v.Normal.x = normalVector.m128_f32[0];
            v.Normal.y = normalVector.m128_f32[1];
            v.Normal.z = normalVector.m128_f32[2];

            v.TexCoord.x = (float)j / slices;
            v.TexCoord.y = (float)i / stacks;

            vertices.push_back(v);
        }
    }

    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < slices; ++j)
        {
            UINT first = i * (slices + 1) + j;
            UINT second = first + (slices + 1);

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
}

void SetupMesh(ID3D11Device* mainDevice)
{
    worldMatrix = XMMatrixTranslation(0,0,0);

    CreateSphereGeometry(1, 50, 50);

    //Vertex Buffer
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertices.size();
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexBufferData = {};
    vertexBufferData.pSysMem = vertices.data();

    HRESULT hr = mainDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &g_pVertexBuffer);
    if (FAILED(hr)) { MessageBoxA(0, "Create Vertex Buffer Failed!", 0, 0); }


    //Index Buffer
    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(UINT) * indices.size();
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexBufferData = {};
    indexBufferData.pSysMem = indices.data();

    hr = mainDevice->CreateBuffer(&indexBufferDesc, &indexBufferData, &g_pIndexBuffer);
    if (FAILED(hr)) { MessageBoxA(0, "Create Index Buffer Failed!", 0, 0); }
}