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
	//�������귨��ת�������귨��һ��Ҫ����(world-1)T �����������ת�ã� ���Ƕ�����������AAT=E����������ͬ����ת�þ���world-1 = worldT 
	// ����ֻ���ȱ����š���ת�ı任��������������
	// normal* worldԭ-1 = normal * worldT, ͬ��normal * worldViewԭ-1 = normal * worldViewԭT
	vsOut.viewFragNormal = mul(nl, (float3x3)worldView);
	vsOut.tex = tx;
	vsOut.shadowHomoPos = ToShadowHomoSpace(pos, world);
	vsOut.pos = mul(float4(pos, 1.0f), worldViewProj);
	return vsOut;
}