
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
 
    //float3 �� float4
    lLocalPosition.xyz = vsIn.pos;
    lLocalPosition.w = 1.0f;
    
    
    //���[�J�����W�����[���h���W�ɕϊ�(����)
    lWorldPosition.w = 1.0f;
    lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);

    //���[���h���W���r���[���W�ɕϊ�
    lViewPosition.w = 1.0f;
    lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
    ret.vwPos.xyz = lViewPosition.xyz;
    
    //�r���[���W���ˉe���W�ɕϊ�
    ret.svPos = mul(lViewPosition, g_base.projectionMatrix);
        
    ////uv���W
    //ret.uv = vsIn.uv0.xy;
    
    ret.uv = vsIn.uv0.xy * g_uv_scale;
     
    //�@�������[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
    ret.normal = normalize(
    mul(vsIn.norm, (float3x3) g_base.localWorldMatrix));
    
    // �f�B�t���[�Y
    ret.diffuse = vsIn.diffuse;
    
    //�X�y�L����
    ret.spec = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    //���C�g����(���[�J��)
    ret.lightDir = normalize(float3(0.0f,0.0f,0.0f));
    
    //���C�g���猩�����W
    ret.lightAtDir = normalize(float3(0.0f, 0.0f, 0.0f));
    
    return ret;
}
