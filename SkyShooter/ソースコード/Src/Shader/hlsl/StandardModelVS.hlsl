
#include "../common/VertexToPixelHeader.hlsli"

#include "../common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_4FRAME

#define VS_OUTPUT VertexToPixelLit
#include "../common/Vertex/VertexShader3DHeader.hlsli"

#define L_W_MAT g_localWorldMatrix.lwMatrix

cbuffer cbBuffer : register(b7)
{
    float3 g_scl;
    float pad;
    float3 g_ligPos;
    float pad2;
    float3 g_ligDir;
    float pad3;
    
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
    
    float3 norm = normalize(
    mul(vsIn.norm, (float3x3) g_base.localWorldMatrix));
    
    //���[�J�����W�����[���h���W�ɕϊ�(����)
    lWorldPosition.w = 1.0f;
    lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);
    //lWorldPosition.xyz += norm * 5.0f;

    //���[���h���W���r���[���W�ɕϊ�
    lViewPosition.w = 1.0f;
    lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
    ret.vwPos.xyz = lViewPosition.xyz;
    
    //�r���[���W���ˉe���W�ɕϊ�
    ret.svPos = mul(lViewPosition, g_base.projectionMatrix);
        
    //uv���W
    ret.uv = vsIn.uv0.xy;
     
    //�@�������[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
    ret.normal = norm;
    
    //�f�B�t���[�Y
    ret.diffuse = vsIn.diffuse;
    
    //�X�y�L����
    ret.spec = float4(1.0f,1.0f,1.0f,1.0f);
    
    //���C�g����(���[�J��)
    ret.lightDir = normalize(g_ligDir);
    
    //���C�g���猩�����W
    ret.lightAtDir = normalize(lWorldPosition.xyz - g_ligPos);
    
    return ret;
}
