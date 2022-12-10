cbuffer ShadowTransformCBuf : register(b1)
{
	//��Դλ���������viewProjת�������ڽ���͸�ӳ�����ΪNDC�ռ����꣬��ת����Ļ�ռ�����    
	//matrix shadowPos;
	
	//�˴���Ϊ��Դλ�������viewת����������ת�޹�
	matrix shadowPos;
};

float4 ToShadowHomoSpace(const in float3 pos, uniform matrix modelTransform)
{
	const float4 world = mul(float4(pos, 1.0f), modelTransform);
	//��ȡ��Դ�������Ļ�ռ�����
	return mul(world, shadowPos);
}