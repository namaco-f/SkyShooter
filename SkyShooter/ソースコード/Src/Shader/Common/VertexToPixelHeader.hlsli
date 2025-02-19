
struct VertexToPixel
{
    //float4 svPos		: SV_POSITION;
    //float4 diffuse		: COLOR0;
    //float4 specular		: COLOR1;
    //float2 uv			: TEXCOORD;
    float4 svPos : SV_POSITION;
    float4 diffuse : COLOR0;    //�g�U���˂̐F
    float4 spec : COLOR1;       //�g�U���˂̐F
    float2 uv : TEXCOORD0;      //UV�l
    float2 suv : TEXCOORD1;     //�T�u�e�N�X�`����UV�l
};


struct VertexToPixelLit
{
	float4 svPos		: SV_POSITION;	//���W( �v���W�F�N�V������� )
	float2 uv			: TEXCOORD0;	//�e�N�X�`�����W
	float3 vwPos		: TEXCOORD1;	//���W( �r���[���W )
	float3 normal       : TEXCOORD2;	//�@��( �r���[���W )
	float4 diffuse      : COLOR0;		//�f�B�t���[�Y�J���[
	float4 spec         : COLOR1;		//�X�y�L�����J���[
	float3 lightDir     : TEXCOORD3;	//���C�g����(���[�J��)
	float3 lightAtDir   : TEXCOORD4;	//���C�g���W����̕���
};

struct VertexToPixelUVScale
{
    float4 svPos : SV_POSITION;     //���W( �v���W�F�N�V������� )
    float2 uv : TEXCOORD0;          //�e�N�X�`�����W
    float3 vwPos : TEXCOORD1;       //���W( �r���[���W )
    float3 normal : TEXCOORD2;      //�@��( �r���[���W )
    float4 diffuse : COLOR0;        //�f�B�t���[�Y�J���[
    float3 lightDir : TEXCOORD3;    //���C�g����(���[�J��)
    float3 lightAtPos : TEXCOORD4;  //���C�g���猩�����W
    float3 worldPos : TEXCOORD5;    //���_���W(���[���h���W)
};

struct VertexToPixelShadow
{
	float4 svPos		: SV_POSITION;	//���W( �v���W�F�N�V������� )
	float2 uv			: TEXCOORD0;	//�e�N�X�`�����W
	float4 vwPos		: TEXCOORD1;	//���W( �r���[���W )
};

struct VertexToPixelTest
{
    float4 svPos : SV_POSITION;     //���W(�v���W�F�N�V�������)
    float2 uv : TEXCOORD0;			//�e�N�X�`�����W
    float3 vwPos : TEXCOORD1;       //���W( �r���[���W )
    float3 normal : TEXCOORD2;      //�@��( �r���[���W )
    float4 diffuse : COLOR0;        //�f�B�t���[�Y�J���[
    float3 worldPos : TEXCOORD3;	//���[���h���W
    float4 sv : TEXCOORD4;
    
    
    float3 lightDir : TEXCOORD5;    //���C�g����(���[�J��)
    //float3 lightAtPos : TEXCOORD4; //���C�g���猩�����W

};

struct VertexDepth
{
    float4 svPos : SV_Position;
    float2 uv : TEXCOORD0;
    float4 ScreenPos : POSITION1;
    float3 normal : NORMAL;
    float4 diffuse : COLOR0;
    float4 spec : COLOR1;
    float4 WorldPos : POSITION0;
};

//struct VertexPolygon3D
//{
//    float4 svPos : SV_POSITION; // ���W( �v���W�F�N�V������� )
//    float3 pos : POSITION0; // ���W( ���[�J����� )
//    float4 SubPosition : POSITION1; // �⏕���W
//    float3 Normal : NORMAL; // �@��
//    float3 Tangent : TANGENT; // �ڐ�
//    float3 Binormal : BINORMAL; // �]�@��
//    float4 diffuse : COLOR0; // �f�B�t���[�Y�J���[
//    float4 Specular : COLOR1; // �X�y�L�����J���[
//    float2 TexCoords0 : TEXCOORD0; // �e�N�X�`�����W�O
//    float2 TexCoords1 : TEXCOORD1; // �e�N�X�`�����W�P

//};