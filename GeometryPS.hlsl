Texture2D gTexMap;

SamplerState sp;

float4 main(float2 tex : TexCoord) : SV_Target
{
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
	return gTexMap.Sample(sp, tex);
}