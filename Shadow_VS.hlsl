#include "Transform.hlsli"
#include "VSShadow.hlsli"

struct VSOut
{
	float4  shadowViewPos : Position;
	float4  pos : SV_Position;
};

VSOut main( float3 pos : Position)
{
	VSOut vsout;
	vsout.shadowViewPos = ToShadowHomoSpace(pos, world);
	vsout.pos = mul(float4(pos, 1.0f), worldViewProj);
	return vsout;
}
