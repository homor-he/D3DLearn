#include "VSShadow.hlsli"
#include "Transform.hlsli"

struct VSOut
{
	float3 viewFragPos : Position;
	float3 viewFragNormal : Normal;
	float2 tex : Texcoord;
	float4 shadowHomoPos : ShadowPosition;
	float4 pos : SV_Position;
};

VSOut main( float3 pos : Position, float3 nl : Normal, float2 tx : Texcoord)
{
	VSOut vsOut;
	vsOut.viewFragPos = (float3)mul(float4(pos, 1.0f), worldView);
	//本地坐标法线转世界坐标法线一般要乘以(world-1)T 世界矩阵的逆的转置， 但是对于正交矩阵AAT=E，其逆矩阵等同于其转置矩阵world-1 = worldT 
	// 由于只含等比缩放、旋转的变换矩阵是正交矩阵
	// normal* world原-1 = normal * worldT, 同理normal * worldView原-1 = normal * worldView原T
	vsOut.viewFragNormal = mul(nl, (float3x3)worldView);
	vsOut.tex = tx;
	vsOut.shadowHomoPos = ToShadowHomoSpace(pos, world);
	vsOut.pos = mul(float4(pos, 1.0f), worldViewProj);
	return vsOut;
}