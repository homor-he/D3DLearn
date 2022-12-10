struct vsOut
{
	float3 color : Color;
	float4 pos : SV_Position;
};

cbuffer cb
{
	matrix transformation;
};

vsOut main(float2 pos : Position, float3 color: Color)
{
	vsOut sOut;
	sOut.pos = mul(float4(pos, 0.0f, 1.0f), transformation);
	sOut.color = color;
	return sOut;
}