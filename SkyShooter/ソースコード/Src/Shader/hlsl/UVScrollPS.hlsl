
#include "../common/VertexToPixelHeader.hlsli"
#define PS_INPUT VertexToPixelLit
#include "../common/Pixel/PixelShader3DHeader.hlsli"

cbuffer cbParam : register(b4)
{
    float4 g_color;
    float g_time;
    float g_speedPar;
    float2 pading;
};

float4 main(PS_INPUT psIn) : SV_TARGET
{
    float2 uv = psIn.uv;
    
    uv.x += g_speedPar * g_time;
    
    float4 texCol = diffuseMapTexture.Sample(diffuseMapSampler, uv);
       
    
    clip(texCol.a);
    
    float3 norm = psIn.normal;
    
    
    return texCol * g_color;
}