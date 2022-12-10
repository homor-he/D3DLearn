#include "ShaderOps.hlsli"

#include "Light.hlsli"

cbuffer ObjectCBuf : register(b1)
{
	float3 specularColor;
	float specularWeight;
	float specularGloss;
}

Texture2D tex : register(t0);

SamplerState sam : register(s0);


float4 main(float3 worldPos : Position, float3 worldNormal : Normal, float2 tx : Texcoord) : SV_TARGET
{
	float3 ambient = float3(0.0f,0.0f,0.0f);
	float3 diffuse = float3(0.0f,0.0f,0.0f);
	float3 specular = float3(0.0f, 0.0f, 0.0f);


	worldNormal = normalize(worldNormal);

	float3 frag2Light = worldLightPos - worldPos;
	float dist = length(frag2Light);
	frag2Light /= dist;

	float3 frag2Eye = mainCameraPos - worldPos;
	float distFrag2Eye = length(frag2Eye);
	frag2Eye /= distFrag2Eye;

	const float att = Attenuate(attConst, attLin, attQuad, dist);
	diffuse = Diffuse(diffuseColor, diffuseIntensity, att, frag2Light, worldNormal);
	//specular = Specular(diffuseColor * diffuseIntensity * specularColor, specularWeight, viewNormal, lv.fragToL, viewPos, att, specularGloss);

	float3 v = reflect(-frag2Light, worldNormal);
	specular = pow(max(dot(v, frag2Eye), 0.0f), specularGloss)*diffuseColor * diffuseIntensity * specularColor*specularWeight*att;

	ambient += pointAmbient;
	return float4(saturate((diffuse + ambient) * tex.Sample(sam, tx).rgb + specular), 1.0f);
}