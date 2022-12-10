struct LightVectorData
{
	float3 fragToL;
	float3 dirToL;
	float distToL;
};

LightVectorData CalculateLightVectorData(const in float3 lightPos, const in float3 fragPos)
{
	LightVectorData lv;
	lv.fragToL = lightPos - fragPos;
	lv.distToL = length(lv.fragToL);
	lv.dirToL = lv.fragToL / lv.distToL;
	return lv;
}