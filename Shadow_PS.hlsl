float4 main(float4 shadowViewPos : Position) : SV_TARGET
{
	//ԽԶֵԽ�󣬱��ֳ�����ɫҲԽ�����Ѿ������ģ�����
	return length(shadowViewPos.xyz) / 100.0f;
}