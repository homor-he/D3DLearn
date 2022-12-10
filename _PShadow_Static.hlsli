TextureCube smap : register(t3);
//Texture2D smap : register(t3);
SamplerComparisonState ssamHw : register(s1);
SamplerState samHw : register(s2);

static const float zf = 100.0f;
static const float zn = 0.5f;
static const float c1 = zf / (zf - zn);
static const float c0 = -zn * zf / (zf - zn);


//平行光阴影：适合正交投影
//聚光灯等：适合透视投影
//此处采用了正交投影
float CalculateShadowDepth(const in float4 shadowPos)
{
	const float3 m = shadowPos.xyz;

	float3 val = float3(abs(m.x), abs(m.y), abs(m.z));

	const float major = max(val.x, max(val.y, val.z));

	return (c1*major + c0) / major;
	//return 0.0f;
}

//传进来的是屏幕空间坐标，
//float Shadow(const in float4 shadowPos)
//{
//	// 正交投影的裁剪空间不用除以w值，即为NDC空间，因为w=1
//	// 用在深度贴图纹理裁剪空间shadowPos位置的x、y、z在深度贴图gShadowMap中采样，
//	// 并把采样深度结果s(p)与光源裁剪空间原始深度值d(p)做对比， s(p)<d(p) 即存在阴影(光照不到)，s(p) >= d(p) 不存在阴影(可以把深度纹理贴图理解为一个曲面,采样的z值越小越近)
//	//return smap.SampleCmpLevelZero(ssam, shadowPos.xyz, CalculateShadowDepth(shadowPos));
//
//	const float3 sPos = shadowPos.xyz / shadowPos.w;
//	
//	//对深度图进行采样时，深度值位于r值
//	return smap.SampleCmpLevelZero(ssamHw, sPos.xy, sPos.z);
//}

//传进来的是光源视图空间坐标
float Shadow(const in float4 shadowPos)
{
	//float3 val3 = { 0.0f,0.0f,0.0f };
	//float val = 0.0f;
	//float3 samVal = smap.Sample(samHw, shadowPos.xyz);
	//val3 += samVal;
	//float realLen = length(shadowPos.xyz);
	//val += realLen;

	//0.0005f防止阴影失真，原因是深度图采样使用了像素点中心的深度值，导致中心两边的深度差
	if (smap.Sample(samHw, shadowPos.xyz).r > length(shadowPos.xyz) / 100.0f - 0.0005f)
		return 1.0f;
	else
		return 0.0f;
}
