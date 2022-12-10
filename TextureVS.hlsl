struct vsOut
{
	float2 tex : TexCoord;
	float4 pos : SV_Position;
};

cbuffer cb
{
	matrix transform;
};

vsOut main( float3 pos : Position, float2 tex : TexCoord)
{
	vsOut vsout;
	vsout.pos = mul(float4(pos, 1.0f), transform);
	vsout.tex = tex;
	return vsout;
}