cbuffer PointLightCBuf : register(b0)
{
	float3 viewLightPos;
	float3 pointAmbient;
	float3 diffuseColor;
	float3 worldLightPos;
	float3 mainCameraPos;
	float diffuseIntensity;
	float attConst;
	float attLin;
	float attQuad;
	bool usePointLight;
};

cbuffer SpotLightCBuf : register(b2)
{
	float3 spotViewLightPos;
	float3 spotAmbient;
	float3 spotDiffuseColor;
	float spotDiffuseIntensity;
	float spotAttConst;
	float spotAttLin;
	float spotAttQuad;
	float3 spotDirection;
	float spotFactor;	//¾Û¹âµÆÏµÊý
	bool useSpotLight;
}

cbuffer DirectLightCBuf : register(b3)
{
	float3 dirAmbient;
	float3 dirDiffuseColor;
	float dirDiffuseIntensity;
	float3 dirDirection;
	bool useDirLight;
}