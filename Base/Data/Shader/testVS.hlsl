cbuffer TransformBuffer : register(b0)
{
    float4x4 worldMatrix;
    float4x4 viewMatrix;
    float4x4 projMatrix;
};

struct VS_INPUT
{
    float3 Pos : POSITION;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float4 worldPos = mul(float4(input.Pos, 1.0), worldMatrix);
    float4 worldViewPos = mul(worldPos, viewMatrix);
    float4 worldViewProjPos = mul(worldViewPos, projMatrix);
    
    output.Pos = worldViewProjPos;
    
    output.Normal = input.Normal;
    
    return output;
}