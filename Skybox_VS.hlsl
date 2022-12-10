cbuffer TransformCBuf : register(b0)
{
	matrix viewProj;
};

struct VSOut
{
	float3 worldPos : Position;
	float4 pos : SV_Position;
};

VSOut main( float3 pos : Position )
{
	VSOut vsout;
	//摄像机和天空盒子原点放在同一位置
	//立方体贴图可以用方向向量进行采样，方向向量类似于立方体的顶点位置。因此可以将立方体的顶点坐标作为纹理坐标使用
	vsout.worldPos = pos;
	vsout.pos = mul(float4(pos,0.0f), viewProj);
	// 设置z = w，以便 z/w = 1 (i.e., 天空球总是在远平面)
	vsout.pos.z = vsout.pos.w;
	return vsout;
}