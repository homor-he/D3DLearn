#include "ShaderOps.hlsli"
#include "LightVectorData.hlsli"

#include "Light.hlsli"
#include "_PShadow_Static.hlsli"

cbuffer ObjectCBuf : register(b1)
{
	float3 specularColor;
	float specularWeight;
	float specularGloss;
}

Texture2D tex : register(t0);

SamplerState sam : register(s0);

float4 main(float3 viewPos : Position, float3 viewNormal : Normal, float2 tx : Texcoord, float4 sPos : ShadowPosition) 
	: SV_TARGET
{
	float3 resAmbient = float3(0.0f,0.0f,0.0f);
	float3 resDiffuse = float3(0.0f,0.0f,0.0f);
	float3 resSpecular = float3(0.0f, 0.0f, 0.0f);

	// shadow map test
	const float shadowLevel = Shadow(sPos);
	if (shadowLevel != 0.0f)
	{
	// renormalize interpolated normal
	viewNormal = normalize(viewNormal);
	// fragment to light vector data
	const LightVectorData lv = CalculateLightVectorData(viewLightPos, viewPos);
	// attenuation
	const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	// diffuse
	resDiffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, viewNormal);
	// specular
	resSpecular = Specular(diffuseColor * diffuseIntensity * specularColor, specularWeight, viewNormal, lv.fragToL, viewPos, att, specularGloss);
	// scale by shadow level
	resDiffuse *= shadowLevel;
	resSpecular *= shadowLevel;
	}
	else
	{
		resDiffuse = resSpecular = 0.0f;
	}
	resAmbient += pointAmbient;
	// final color
	return float4(saturate((resDiffuse + resAmbient) * tex.Sample(sam, tx).rgb + resSpecular), 1.0f);
}