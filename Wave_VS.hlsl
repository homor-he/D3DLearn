#include "Transform.hlsli"


struct vsOut
{
	float4 color : Color;
	float4 pos : SV_Position;
};


vsOut main( float3 pos : Position, float4 color : Color )
{
	vsOut vsout;
	vsout.pos = mul(float4(pos, 1.0f), worldViewProj);
	vsout.color = color;
	return vsout;
}