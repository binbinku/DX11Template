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

    worldMatrix = XMMatrixRotationRollPitchYaw(1, 1, 1);

    //CreateBox(1, 1, 1);

    CreateSphere(1, 50, 50);

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

void CreateSphere(float radius, UINT levels, UINT slices)
{
    using namespace DirectX;

    vertices.clear();
    indices.clear();

    float phi = 0.0f, theta = 0.0f;
    float per_phi = XM_PI / levels;
    float per_theta = XM_2PI / slices;

    // 生成顶点
    for (UINT i = 0; i <= levels; ++i)
    {
        phi = per_phi * i;
        for (UINT j = 0; j <= slices; ++j)
        {
            theta = per_theta * j;

            float x = radius * sinf(phi) * cosf(theta);
            float y = radius * cosf(phi);
            float z = radius * sinf(phi) * sinf(theta);

            Vertex v;
            v.Position = XMFLOAT3(x, y, z);
            v.Normal = XMFLOAT3(x, y, z); // 法线即为归一化的位置向量
            XMStoreFloat2(&v.TexCoord, XMVector2Transform(XMVectorSet(theta / XM_2PI, phi / XM_PI, 0.0f, 0.0f), XMMatrixScaling(1.0f, 1.0f, 1.0f)));
            XMStoreFloat3(&v.Normal, XMVector3Normalize(XMLoadFloat3(&v.Position)));
            vertices.push_back(v);
        }
    }

    // 生成索引
    for (UINT i = 0; i < levels; ++i)
    {
        for (UINT j = 0; j < slices; ++j)
        {
            UINT top1 = i * (slices + 1) + j;
            UINT top2 = top1 + 1;
            UINT bottom1 = top1 + slices + 1;
            UINT bottom2 = bottom1 + 1;

            indices.push_back(top1);
            indices.push_back(bottom1);
            indices.push_back(bottom2);

            indices.push_back(bottom2);
            indices.push_back(top2);
            indices.push_back(top1);
        }
    }
}

void CreateBox(float width, float height, float depth)
{
    using namespace DirectX;

    vertices.clear();
    indices.clear();

    float w2 = width / 2, h2 = height / 2, d2 = depth / 2;

    // 顶点
    Vertex v[24] = {
        // 右面
        { XMFLOAT3(w2, -h2, -d2), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
        { XMFLOAT3(w2, h2, -d2), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
        { XMFLOAT3(w2, h2, d2), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
        { XMFLOAT3(w2, -h2, d2), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
        // 左面
        { XMFLOAT3(-w2, -h2, d2), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
        { XMFLOAT3(-w2, h2, d2), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
        { XMFLOAT3(-w2, h2, -d2), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
        { XMFLOAT3(-w2, -h2, -d2), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
        // 顶面
        { XMFLOAT3(-w2, h2, -d2), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
        { XMFLOAT3(-w2, h2, d2), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
        { XMFLOAT3(w2, h2, d2), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
        { XMFLOAT3(w2, h2, -d2), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
        // 底面
        { XMFLOAT3(w2, -h2, -d2), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
        { XMFLOAT3(w2, -h2, d2), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
        { XMFLOAT3(-w2, -h2, d2), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
        { XMFLOAT3(-w2, -h2, -d2), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
        // 背面
        { XMFLOAT3(w2, -h2, d2), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(w2, h2, d2), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-w2, h2, d2), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-w2, -h2, d2), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
        // 正面
        { XMFLOAT3(-w2, -h2, -d2), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
        { XMFLOAT3(-w2, h2, -d2), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
        { XMFLOAT3(w2, h2, -d2), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
        { XMFLOAT3(w2, -h2, -d2), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) }
    };

    vertices.insert(vertices.end(), v, v + 24);

    // 索引
    UINT idx[36] = {
        // 右面
        0, 1, 2, 2, 3, 0,
        // 左面
        4, 5, 6, 6, 7, 4,
        // 顶面
        8, 9, 10, 10, 11, 8,
        // 底面
        12, 13, 14, 14, 15, 12,
        // 背面
        16, 17, 18, 18, 19, 16,
        // 正面
        20, 21, 22, 22, 23, 20
    };

    indices.insert(indices.end(), idx, idx + 36);
}

float x;
void UpdateMesh()
{
    x += 0.01f;

    worldMatrix = XMMatrixRotationRollPitchYaw(x, 1, 1);
}