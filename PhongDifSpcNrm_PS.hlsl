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
	bool useNormalMap;
	float normalMapWeight;
}

Texture2D gTexMap : register(t0);
Texture2D gSpecMap : register(t1);
Texture2D gNormalMap : register(t2);

SamplerState sam : register(s0);
 
float4 main(float3 viewFragPos : Position, 
	float3 viewFragNormal : Normal, 
	float3 tan : Tangent, 
	float3 bitan : Bitangent, 
	float2 tx : Texcoord,float4 sPos : ShadowPosition) : SV_TARGET
{
	float3 resAmbient = float3(0.0f,0.0f,0.0f);
	float3 resDiffuse = float3(0.0f,0.0f,0.0f);
	float3 resSpecular = float3(0.0f, 0.0f, 0.0f);

	const float4 diffuseMap = gTexMap.Sample(sam, tx);

#ifdef MASK_BOI
	//clip��������:������������е��κ�Ԫ��С��0,������ǰ����
	clip(diffuseMap.a < 0.1f ? -1 : 1);
	float3 fragPosToCamera = float3(0.0f, 0.0f, 0.0f) - viewFragPos;
	//�������ĵط���ת���߱���棿
	if (dot(viewFragNormal, fragPosToCamera) <= 0.0f)
	{
		viewFragNormal = -viewFragNormal;
	}
#endif
	const float shadowLevel = Shadow(sPos);
	viewFragNormal = normalize(viewFragNormal);

	if (useNormalMap)
	{
		const float3 mapNormal = MapNormal(normalize(tan), normalize(bitan), viewFragNormal, tx, gNormalMap, sam);
		viewFragNormal = lerp(viewFragNormal, mapNormal, normalMapWeight);
	}

	const float4 specularSample = gSpecMap.Sample(sam, tx);
	float3 specularRelfectionColor = specularColor;
	float specularPower = specularGloss;
	if (useSpecularMap)
	{
		//ʹ�õĸ߹���ͼ�������������⣬Ӧ�ò��ó���255.0f
		specularRelfectionColor = specularSample.rgb/255.0f;
	}
	
	if (useGlossAlpha)
	{
		//ʹ�õĸ߹���ͼ�������������⣬Ӧ�ò��ó���255.0f
		specularPower = pow(2.0f, specularSample.a/255.0f * 13.0f);
	}

	
	if (usePointLight)
    {
		if (shadowLevel != 0.0f)
		{
			const LightVectorData lv = CalculateLightVectorData(viewLightPos, viewFragPos);
			const float pointAtt = Attenuate(attConst, attLin, attQuad, lv.distToL);
			resDiffuse += shadowLevel*Diffuse(diffuseColor, diffuseIntensity, pointAtt, lv.dirToL, viewFragNormal);
			resSpecular += shadowLevel*Specular(
				diffuseColor * diffuseIntensity * specularRelfectionColor, specularWeight, viewFragNormal,
				lv.fragToL, viewFragPos, pointAtt, specularPower
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
			spotDiffuseColor * spotDiffuseIntensity * specularRelfectionColor, specularWeight, viewFragNormal,
			lv.fragToL, viewFragPos, spotAtt, specularPower
		);
	}

	if (useDirLight)
	{
		float3 dirToL = -normalize(dirDirection);
		resAmbient += dirAmbient;
		resDiffuse += Diffuse(dirDiffuseColor, dirDiffuseIntensity, 1.0f, dirToL, viewFragNormal);
		resSpecular += Specular(dirDiffuseColor*dirDiffuseIntensity*specularRelfectionColor, specularWeight, viewFragNormal,
			-dirDirection, viewFragPos, 1.0f, specularPower);
	}
	return float4(saturate((resDiffuse + resAmbient)* gTexMap.Sample(sam, tx).rgb + resSpecular), 1.0f);
}