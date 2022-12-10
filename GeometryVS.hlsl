#include "Transform.hlsli"

struct vsOut
{
	float2 tex : TexCoord;
	float4 pos : SV_Position;
};

vsOut main( float3 posL : Position, float3 nlL : Normal, float3 tg : Tangent, float2 tex : TexCoord) 
{
	vsOut vsout;
	vsout.pos = mul(float4(posL, 1.0f), worldViewProj);
	vsout.tex = tex;
	return vsout;
}