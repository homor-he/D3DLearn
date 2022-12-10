#include "Transform.hlsli"

struct VSOut
{
	float3 worldPos : Position;
	float3 worldNormal : Normal;
	float2 tx : Texcoord;
	//float4 shadowHomoPos : ShadowPosition;
	float4 pos : SV_Position;
};

VSOut main( float3 pos : Position, float3 n : Normal, float2 tx : Texcoord) 
{
	VSOut vso;
	vso.worldPos = mul(float4(pos, 1.0f), world);
	vso.worldNormal = mul(n, (float3x3)worldInvTranspose);
	vso.tx = tx;
	//vso.shadowHomoPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
	vso.pos = mul(float4(pos, 1.0f), worldViewProj);
	return vso;
}