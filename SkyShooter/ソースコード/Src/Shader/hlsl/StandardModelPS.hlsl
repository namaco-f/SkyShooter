
#include "../common/VertexToPixelHeader.hlsli"
#define PS_INPUT VertexToPixelLit
#include "../common/Pixel/PixelShader3DHeader.hlsli"

cbuffer cbParam : register(b4)
{
    float4 g_color;
};

float4 main(PS_INPUT psIn) : SV_TARGET
{
    float4 texCol = diffuseMapTexture.Sample(diffuseMapSampler,psIn.uv);
    
    float d = dot(psIn.lightAtDir, psIn.normal);
    
    texCol.a = texCol.a * g_color.a;
    clip(texCol.a);
    
    if (d < 0.0f)
    {
        d = 0.0f;
    }
    
    float4 finCol;
    float par = lerp(1.0f, 0.0f, d);
    texCol.rgb *= par;
    finCol.a = texCol.a;
    
    return texCol;
}