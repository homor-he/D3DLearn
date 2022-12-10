TextureCube smap : register(t3);
//Texture2D smap : register(t3);
SamplerComparisonState ssamHw : register(s1);
SamplerState samHw : register(s2);

static const float zf = 100.0f;
static const float zn = 0.5f;
static const float c1 = zf / (zf - zn);
static const float c0 = -zn * zf / (zf - zn);


//ƽ�й���Ӱ���ʺ�����ͶӰ
//�۹�Ƶȣ��ʺ�͸��ͶӰ
//�˴�����������ͶӰ
float CalculateShadowDepth(const in float4 shadowPos)
{
	const float3 m = shadowPos.xyz;

	float3 val = float3(abs(m.x), abs(m.y), abs(m.z));

	const float major = max(val.x, max(val.y, val.z));

	return (c1*major + c0) / major;
	//return 0.0f;
}

//������������Ļ�ռ����꣬
//float Shadow(const in float4 shadowPos)
//{
//	// ����ͶӰ�Ĳü��ռ䲻�ó���wֵ����ΪNDC�ռ䣬��Ϊw=1
//	// ���������ͼ����ü��ռ�shadowPosλ�õ�x��y��z�������ͼgShadowMap�в�����
//	// ���Ѳ�����Ƚ��s(p)���Դ�ü��ռ�ԭʼ���ֵd(p)���Աȣ� s(p)<d(p) ��������Ӱ(���ղ���)��s(p) >= d(p) ��������Ӱ(���԰����������ͼ���Ϊһ������,������zֵԽСԽ��)
//	//return smap.SampleCmpLevelZero(ssam, shadowPos.xyz, CalculateShadowDepth(shadowPos));
//
//	const float3 sPos = shadowPos.xyz / shadowPos.w;
//	
//	//�����ͼ���в���ʱ�����ֵλ��rֵ
//	return smap.SampleCmpLevelZero(ssamHw, sPos.xy, sPos.z);
//}

//���������ǹ�Դ��ͼ�ռ�����
float Shadow(const in float4 shadowPos)
{
	//float3 val3 = { 0.0f,0.0f,0.0f };
	//float val = 0.0f;
	//float3 samVal = smap.Sample(samHw, shadowPos.xyz);
	//val3 += samVal;
	//float realLen = length(shadowPos.xyz);
	//val += realLen;

	//0.0005f��ֹ��Ӱʧ�棬ԭ�������ͼ����ʹ�������ص����ĵ����ֵ�������������ߵ���Ȳ�
	if (smap.Sample(samHw, shadowPos.xyz).r > length(shadowPos.xyz) / 100.0f - 0.0005f)
		return 1.0f;
	else
		return 0.0f;
}
