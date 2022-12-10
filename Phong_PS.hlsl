#include "ShaderOps.hlsli"
#include "LightVectorData.hlsli"
#include "Light.hlsli"
#include "_PShadow_Static.hlsli"

cbuffer ObjectCBuf : register(b1)
{
	float3 materialColor;
	float3 specularColor;
	float specularWeight;
	float specularGloss;
};

float4 main(float3 viewFragPos : Position, float3 viewFragNormal : Normal, float4 sPos : ShadowPosition) : SV_Target
{
	float3 resAmbient = float3(0.0f, 0.0f, 0.0f);
	float3 resDiffuse = float3(0.0f, 0.0f, 0.0f);
	float3 resSpecular = float3(0.0f, 0.0f, 0.0f);

	const float shadowLevel = Shadow(sPos);
	viewFragNormal = normalize(viewFragNormal);

	if (usePointLight)
	{
		if (shadowLevel != 0.0f)
		{
			const LightVectorData lv = CalculateLightVectorData(viewLightPos, viewFragPos);
			const float pointAtt = Attenuate(attConst, attLin, attQuad, lv.distToL);
			resDiffuse += shadowLevel * Diffuse(diffuseColor, diffuseIntensity, pointAtt, lv.dirToL, viewFragNormal);
			resSpecular += shadowLevel * Specular(
				diffuseColor * diffuseIntensity * specularColor, specularWeight, viewFragNormal,
				lv.fragToL, viewFragPos, pointAtt, specularGloss
			);
		}
		//阴影等级不影响环境光
		resAmbient += pointAmbient;
	}
	
	if (useSpotLight)
	{
		const LightVectorData lv = CalculateLightVectorData(spotViewLightPos, viewFragPos);
		//控制镜面高光圆锥体反射系数的公式，聚光灯方向向量和照射点的光线向量的夹角越大，光照强度越小
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
	return float4(saturate((resDiffuse + resAmbient) * materialColor + resSpecular), 1.0f);
}