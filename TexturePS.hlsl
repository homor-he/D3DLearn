Texture2D gTexMap;

SamplerState sam;

float4 main(float2 tex : TexCoord) : SV_TARGET
{
	return gTexMap.Sample(sam,tex);
}