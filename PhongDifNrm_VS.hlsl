#include "Transform.hlsli"
#include "VSShadow.hlsli"


struct VSOut
{
	float3 viewFragPos : Position;
	float3 viewFragNormal : Normal;
	float3 tan : Tangent;
	float3 bitan : Bitangent;
	float2 tex : Texcoord;
	float4 shadowHomoPos : ShadowPosition;
	float4 pos : SV_Position;
};

VSOut main( float3 pos : Position,
	float3 nl : Normal,
	float2 tx : Texcoord,
	float3 tan : Tangent,
	float3 bitan : Bitangent)
{
	VSOut vsOut;
	vsOut.viewFragPos = (float3)mul(float4(pos, 1.0f), worldView);
	vsOut.viewFragNormal = mul(nl, (float3x3)worldView);
	vsOut.tan = mul(tan, (float3x3)worldView);
	vsOut.bitan = mul(bitan, (float3x3)worldView);
	vsOut.tex = tx;
	vsOut.shadowHomoPos = ToShadowHomoSpace(pos, world);
	vsOut.pos = mul(float4(pos,1.0f), worldViewProj);
	return vsOut;
}