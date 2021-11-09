struct vsOut
{
	float3 color : Color;
	float4 pos : SV_Position;
};

vsOut main(float2 pos : Position, float3 color: Color)
{
	vsOut sOut;
	sOut.pos = float4(pos, 0.0f, 1.0f);
	sOut.color = color;
	return sOut;
}