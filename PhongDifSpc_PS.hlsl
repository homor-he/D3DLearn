#include "ShaderOps.hlsli"
#include "LightVectorData.hlsli"
#include "Light.hlsli"
#include "_PShadow_Static.hlsli"

cbuffer ObjectCBuf : register(b1)
{
	bool useGlossAlpha;
	bool useSpecularMap;
	float3 specularColor;
	float specularWeight;
	float specularGloss;
};

Texture2D gTexMap : register(t0);
Texture2D gSpecMap : register(t1);

SamplerState sam : register(s0);

float4 main(float3 viewFragPos : Position,
	float3 viewFragNormal : Normal,
	float2 tex : Texcoord,float4 sPos : ShadowPosition) : SV_TARGET
{
	float3 resAmbient = float3(0.0f,0.0f,0.0f);
	float3 resDiffuse = float3(0.0f,0.0f,0.0f);
	float3 resSpecular = float3(0.0f, 0.0f, 0.0f);

	//�߹����
	float specLoaded = specularGloss;
	const float4 specSample = gSpecMap.Sample(sam, tex);
	float3 specReflectColor;
	if (useSpecularMap)
	{
		//ʹ�õĸ߹���ͼ�������������⣬Ӧ�ò��ó���255.0f
		specReflectColor = specSample.rgb/255.0f;
	}
	else
	{
		specReflectColor = specularColor;
	}

	if (useGlossAlpha)
	{
		//ʹ�õĸ߹���ͼ�������������⣬Ӧ�ò��ó���255.0f
		specLoaded = pow(2.0f, specSample.a/255.0f * 13.0f);
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
				diffuseColor * diffuseIntensity * specReflectColor, specularWeight, viewFragNormal,
				lv.fragToL, viewFragPos, pointAtt, specLoaded
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
			spotDiffuseColor * spotDiffuseIntensity * specReflectColor, specularWeight, viewFragNormal,
			lv.fragToL, viewFragPos, spotAtt, specLoaded
		);
	}

	if (useDirLight)
	{
		float3 dirToL = -normalize(dirDirection);
		resAmbient += dirAmbient;
		resDiffuse += Diffuse(dirDiffuseColor, dirDiffuseIntensity, 1.0f, dirToL, viewFragNormal);
		resSpecular += Specular(dirDiffuseColor*dirDiffuseIntensity*specReflectColor, specularWeight, viewFragNormal,
			-dirDirection, viewFragPos, 1.0f, specLoaded);
	}
	//saturate:��ֵ������0~1
	return float4(saturate((resDiffuse + resAmbient) * gTexMap.Sample(sam, tex).rgb + resSpecular), 1.0f);
}