#include "ShaderOps.hlsli"
#include "LightVectorData.hlsli"
#include "Light.hlsli"
#include "_PShadow_Static.hlsli"

cbuffer ObjectCBuf : register(b1)
{
	float3 specularColor;
	float specularWeight;
	float specularGloss;
	bool userNormalMap;
	float normalMapWeight;
}

Texture2D gTexMap : register(t0);
Texture2D gNormalMap : register(t2);

SamplerState sam : register(s0);

float4 main(float3 viewFragPos : Position,
	float3 viewFragNormal : Normal,
	float3 tan : Tangent,
	float3 bitan : Bitangent,
	float2 tex : Texcoord,float4 sPos : ShadowPosition) : SV_TARGET
{
	float3 resAmbient = float3(0.0f,0.0f,0.0f);
	float3 resDiffuse = float3(0.0f,0.0f,0.0f);
	float3 resSpecular = float3(0.0f, 0.0f, 0.0f);

	if (userNormalMap)
	{
		//��ù۲�ռ��µ�������ͼ��������
		const float3 mapNormal = MapNormal(normalize(tan), normalize(bitan), viewFragNormal, tex, gNormalMap, sam);
		viewFragNormal = lerp(viewFragNormal, mapNormal, normalMapWeight);
	}

	const float shadowLevel = Shadow(sPos);
	viewFragNormal = normalize(viewFragNormal);

	if (usePointLight)
	{
		if (shadowLevel != 0.0f)
		{
			const LightVectorData lv = CalculateLightVectorData(viewLightPos, viewFragPos);
			const float pointAtt = Attenuate(attConst, attLin, attQuad, lv.distToL);
			resDiffuse += shadowLevel*Diffuse(diffuseColor, diffuseIntensity, pointAtt, lv.dirToL, viewFragNormal);
			resSpecular += shadowLevel*Specular(
				diffuseColor * diffuseIntensity * specularColor, specularWeight, viewFragNormal,
				lv.fragToL, viewFragPos, pointAtt, specularGloss
			);
		}
		resAmbient += pointAmbient;
	}

	if (useSpotLight)
	{
		const LightVectorData lv = CalculateLightVectorData(spotViewLightPos, viewFragPos);
		//���ƾ���߹�Բ׶�巴��ϵ���Ĺ�ʽ���۹�Ʒ��������������Ĺ��������ļн�Խ�󣬹���ǿ��ԽС
		float spot = pow(max(0.0f, dot(normalize(spotDirection), -lv.dirToL)), spotFactor);

		const float spotAtt = spot * Attenuate(spotAttConst, spotAttLin, spotAttQuad, lv.distToL);
		resAmbient += spotAmbient * spot;
		resDiffuse += Diffuse(spotDiffuseColor, spotDiffuseIntensity, spotAtt, lv.dirToL, viewFragNormal);
		resSpecular += Specular(
			spotDiffuseColor * spotDiffuseIntensity * specularColor, specularWeight, viewFragNormal,
			lv.fragToL, viewFragPos, spotAtt, specularGloss
		);
	}

	if (useDirLight)
	{
		float3 dirToL = -normalize(dirDirection);
		resAmbient += dirAmbient;
		resDiffuse += Diffuse(dirDiffuseColor, dirDiffuseIntensity, 1.0f, dirToL, viewFragNormal);
		resSpecular += Specular(dirDiffuseColor*dirDiffuseIntensity*specularColor, specularWeight, viewFragNormal,
			-dirDirection, viewFragPos, 1.0f, specularGloss);
	}
	//saturate:��ֵ������0~1
	return float4(saturate((resDiffuse + resAmbient) * gTexMap.Sample(sam, tex).rgb + resSpecular), 1.0f);
}