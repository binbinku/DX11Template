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
    float4 color : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float4 worldPos = mul(float4(input.Pos, 1.0), worldMatrix);
    float4 worldViewPos = mul(worldPos, viewMatrix);
    float4 worldViewProjPos = mul(worldViewPos, projMatrix);
    
    output.Pos = worldViewProjPos;
    
    float4 worldNormal = mul(float4(input.Normal, 0), worldMatrix);
    
    float3 lightDir = float3(0.5,-0.5,-0.5);
    
    float diff = dot(worldNormal.xyz, -lightDir);
    
    output.color = float4(1, 1, 1, 1) * diff;
    
    return output;
}