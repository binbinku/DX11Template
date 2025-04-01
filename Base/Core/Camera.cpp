#include "Camera.h"


DirectX::XMMATRIX viewMatrix;
DirectX::XMMATRIX projMatrix;


void SetupCamera()
{
    viewMatrix = DirectX::XMMatrixLookAtLH(
        DirectX::XMVectorSet(0.0f, 0.0f, -30.0f, 0.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
        DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    );

    projMatrix = DirectX::XMMatrixPerspectiveFovLH(
        DirectX::XM_PI / 4.0f,
        1280.0f / 800.0f,
        0.01f,
        1000.0f
    );
}
