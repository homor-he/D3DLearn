#include "Transform.hlsli"
#include "VSShadow.hlsli"

//渲染流水线
//模型数据-顶点着色 - 曲面细分 - 几何着色器 - 裁剪 - NDC空间 - 屏幕空间 - 光栅阶段 - 帧缓存
//裁剪 - NDC空间 - 屏幕空间一般由底层完成
//输入像素着色器的坐标，其实是屏幕空间坐标

//裁剪空间->NDC空间  进行透视除法，透视投影的裁剪空间w不为1.0，正交投影的裁剪空间w=1.0,即pos.xyz = pos.xyz/pos.w， pos.z为深度
//NDC空间->屏幕空间 把NDC空间[-1.0,+1.0]的坐标转换到屏幕空间[0,1]的坐标

struct VSOut
{
	float3 viewPos : Position;
	float3 viewNormal : Normal;
	float2 tx : Texcoord;
	float4 shadowHomoPos : ShadowPosition;
	float4 pos : SV_Position;
};

VSOut main( float3 pos : Position, float3 n : Normal, float2 tx : Texcoord)
{
	VSOut vso;
	vso.viewPos = (float3)mul(float4(pos, 1.0f), worldView);
	vso.viewNormal = mul(n, (float3x3)worldView);
	vso.tx = tx;
	//获取光源摄像机下的深度贴图纹理空间位置
	vso.shadowHomoPos = ToShadowHomoSpace(pos, world);
	vso.pos = mul(float4(pos, 1.0f), worldViewProj);
	return vso;
}