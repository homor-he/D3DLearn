float4 main(float4 shadowViewPos : Position) : SV_TARGET
{
	//越远值越大，表现出的颜色也越亮，已经过深度模板测试
	return length(shadowViewPos.xyz) / 100.0f;
}