#include "VSShadow.hlsli"
#include "Transform.hlsli"

struct VSOut
{
	float3 viewFragPos : Position;
	float3 viewFragNormal : Normal;
	float4 shadowHomoPos : ShadowPosition;
	float4 pos : SV_Position;
};

VSOut main( float3 pos : Position, float3 nl:Normal/*, float3 tg:Tangent, float2 tex: TexCoord*/)
{
	VSOut vsOut;
	vsOut.viewFragPos = (float3)mul(float4(pos, 1.0f), worldView);
	vsOut.viewFragNormal = mul(nl, (float3x3)worldView);
	vsOut.shadowHomoPos = ToShadowHomoSpace(pos, world);
	vsOut.pos = mul(float4(pos, 1.0f), worldViewProj);
	return vsOut;
}