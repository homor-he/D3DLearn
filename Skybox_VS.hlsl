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
	//���������պ���ԭ�����ͬһλ��
	//��������ͼ�����÷����������в�������������������������Ķ���λ�á���˿��Խ�������Ķ���������Ϊ��������ʹ��
	vsout.worldPos = pos;
	vsout.pos = mul(float4(pos,0.0f), viewProj);
	// ����z = w���Ա� z/w = 1 (i.e., �����������Զƽ��)
	vsout.pos.z = vsout.pos.w;
	return vsout;
}