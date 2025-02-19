
#include "../common/VertexToPixelHeader.hlsli"

#include "../common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_4FRAME

#define VS_OUTPUT VertexToPixelLit
#include "../common/Vertex/VertexShader3DHeader.hlsli"

#define L_W_MAT g_localWorldMatrix.lwMatrix

cbuffer cbBuffer : register(b7)
{
    float2 g_uv_scale;
    float2 pading;
    //float speed;
    //float3 g_modelWorldPos;
};

VS_OUTPUT main(VS_INPUT vsIn)
{
    VS_OUTPUT ret;
   
    float4 lLocalPosition;
    float4 lWorldPosition;
    float4 lViewPosition;
 
    //float3 → float4
    lLocalPosition.xyz = vsIn.pos;
    lLocalPosition.w = 1.0f;
    
    
    //ローカル座標をワールド座標に変換(剛体)
    lWorldPosition.w = 1.0f;
    lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);

    //ワールド座標をビュー座標に変換
    lViewPosition.w = 1.0f;
    lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
    ret.vwPos.xyz = lViewPosition.xyz;
    
    //ビュー座標を射影座標に変換
    ret.svPos = mul(lViewPosition, g_base.projectionMatrix);
        
    ////uv座標
    //ret.uv = vsIn.uv0.xy;
    
    ret.uv = vsIn.uv0.xy * g_uv_scale;
     
    //法線をローカル空間からワールド空間へ変換
    ret.normal = normalize(
    mul(vsIn.norm, (float3x3) g_base.localWorldMatrix));
    
    // ディフューズ
    ret.diffuse = vsIn.diffuse;
    
    //スペキュラ
    ret.spec = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    //ライト方向(ローカル)
    ret.lightDir = normalize(float3(0.0f,0.0f,0.0f));
    
    //ライトから見た座標
    ret.lightAtDir = normalize(float3(0.0f, 0.0f, 0.0f));
    
    return ret;
}
