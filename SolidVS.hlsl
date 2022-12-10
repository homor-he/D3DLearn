#include "Transform.hlsli"

float4 main( float3 pos : Position ) : SV_Position
{
	//行向量*列矩阵
	return mul(float4(pos,1.0f), worldViewProj);
}
