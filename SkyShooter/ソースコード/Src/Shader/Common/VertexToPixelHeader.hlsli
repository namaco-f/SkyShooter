
struct VertexToPixel
{
    //float4 svPos		: SV_POSITION;
    //float4 diffuse		: COLOR0;
    //float4 specular		: COLOR1;
    //float2 uv			: TEXCOORD;
    float4 svPos : SV_POSITION;
    float4 diffuse : COLOR0;    //拡散反射の色
    float4 spec : COLOR1;       //拡散反射の色
    float2 uv : TEXCOORD0;      //UV値
    float2 suv : TEXCOORD1;     //サブテクスチャのUV値
};


struct VertexToPixelLit
{
	float4 svPos		: SV_POSITION;	//座標( プロジェクション空間 )
	float2 uv			: TEXCOORD0;	//テクスチャ座標
	float3 vwPos		: TEXCOORD1;	//座標( ビュー座標 )
	float3 normal       : TEXCOORD2;	//法線( ビュー座標 )
	float4 diffuse      : COLOR0;		//ディフューズカラー
	float4 spec         : COLOR1;		//スペキュラカラー
	float3 lightDir     : TEXCOORD3;	//ライト方向(ローカル)
	float3 lightAtDir   : TEXCOORD4;	//ライト座標からの方向
};

struct VertexToPixelUVScale
{
    float4 svPos : SV_POSITION;     //座標( プロジェクション空間 )
    float2 uv : TEXCOORD0;          //テクスチャ座標
    float3 vwPos : TEXCOORD1;       //座標( ビュー座標 )
    float3 normal : TEXCOORD2;      //法線( ビュー座標 )
    float4 diffuse : COLOR0;        //ディフューズカラー
    float3 lightDir : TEXCOORD3;    //ライト方向(ローカル)
    float3 lightAtPos : TEXCOORD4;  //ライトから見た座標
    float3 worldPos : TEXCOORD5;    //頂点座標(ワールド座標)
};

struct VertexToPixelShadow
{
	float4 svPos		: SV_POSITION;	//座標( プロジェクション空間 )
	float2 uv			: TEXCOORD0;	//テクスチャ座標
	float4 vwPos		: TEXCOORD1;	//座標( ビュー座標 )
};

struct VertexToPixelTest
{
    float4 svPos : SV_POSITION;     //座標(プロジェクション空間)
    float2 uv : TEXCOORD0;			//テクスチャ座標
    float3 vwPos : TEXCOORD1;       //座標( ビュー座標 )
    float3 normal : TEXCOORD2;      //法線( ビュー座標 )
    float4 diffuse : COLOR0;        //ディフューズカラー
    float3 worldPos : TEXCOORD3;	//ワールド座標
    float4 sv : TEXCOORD4;
    
    
    float3 lightDir : TEXCOORD5;    //ライト方向(ローカル)
    //float3 lightAtPos : TEXCOORD4; //ライトから見た座標

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
//    float4 svPos : SV_POSITION; // 座標( プロジェクション空間 )
//    float3 pos : POSITION0; // 座標( ローカル空間 )
//    float4 SubPosition : POSITION1; // 補助座標
//    float3 Normal : NORMAL; // 法線
//    float3 Tangent : TANGENT; // 接線
//    float3 Binormal : BINORMAL; // 従法線
//    float4 diffuse : COLOR0; // ディフューズカラー
//    float4 Specular : COLOR1; // スペキュラカラー
//    float2 TexCoords0 : TEXCOORD0; // テクスチャ座標０
//    float2 TexCoords1 : TEXCOORD1; // テクスチャ座標１

//};