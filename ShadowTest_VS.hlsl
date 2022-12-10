#include "Transform.hlsli"
#include "VSShadow.hlsli"

//��Ⱦ��ˮ��
//ģ������-������ɫ - ����ϸ�� - ������ɫ�� - �ü� - NDC�ռ� - ��Ļ�ռ� - ��դ�׶� - ֡����
//�ü� - NDC�ռ� - ��Ļ�ռ�һ���ɵײ����
//����������ɫ�������꣬��ʵ����Ļ�ռ�����

//�ü��ռ�->NDC�ռ�  ����͸�ӳ�����͸��ͶӰ�Ĳü��ռ�w��Ϊ1.0������ͶӰ�Ĳü��ռ�w=1.0,��pos.xyz = pos.xyz/pos.w�� pos.zΪ���
//NDC�ռ�->��Ļ�ռ� ��NDC�ռ�[-1.0,+1.0]������ת������Ļ�ռ�[0,1]������

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
	//��ȡ��Դ������µ������ͼ����ռ�λ��
	vso.shadowHomoPos = ToShadowHomoSpace(pos, world);
	vso.pos = mul(float4(pos, 1.0f), worldViewProj);
	return vso;
}