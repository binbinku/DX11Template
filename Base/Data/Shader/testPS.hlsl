
struct PS_INPUT
{
    float3 Normal : NORMAL;
};

float4 main(PS_INPUT input) : SV_Target
{
    float4 color = float4(1,1,1,1);
    
    return color;
}