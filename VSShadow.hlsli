cbuffer ShadowTransformCBuf : register(b1)
{
	//光源位置摄像机的viewProj转换矩阵，在进行透视除法变为NDC空间坐标，再转到屏幕空间坐标    
	//matrix shadowPos;
	
	//此处改为光源位置摄像机view转换矩阵，与旋转无关
	matrix shadowPos;
};

float4 ToShadowHomoSpace(const in float3 pos, uniform matrix modelTransform)
{
	const float4 world = mul(float4(pos, 1.0f), modelTransform);
	//获取光源摄像机屏幕空间坐标
	return mul(world, shadowPos);
}