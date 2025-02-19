#include "../common/VertexToPixelHeader.hlsli"
#define PS_INPUT VertexToPixelLit
#include "../common/Pixel/PixelShader3DHeader.hlsli"

cbuffer cbParam : register(b4)
{
    float4 g_color;
    float4 g_colorEnd;
    float g_time;
    float g_timeMax;
    float2 padding;
};

float4 main(PS_INPUT psIn) : SV_TARGET
{
    float2 uv = psIn.uv;    
    float4 texCol = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    
    clip(texCol.a);
    
    float3 finCol = lerp(g_color.rgb, g_colorEnd.rgb, g_time / g_timeMax);
    
    return float4(finCol.rgb, texCol.a);
}