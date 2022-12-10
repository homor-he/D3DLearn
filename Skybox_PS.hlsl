TextureCube tex : register(t0);
SamplerState sam : register(s0);

float4 main(float3 worldPos : Position) : SV_TARGET
{
	//此处的worldPos为向量，天空盒背景显示不受相机移动影响
	return tex.Sample(sam, worldPos);
}