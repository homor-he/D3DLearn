
//一个物体表面的颜色是由放射(emissive)、环境反射(ambient)、漫反射(diffuse)和镜面反射(specular)等光照作用的总和
//物体表面光照计算公式
//I = 环境光(Iambient) + 漫反射光(Idiffuse) + 镜面高光(Ispecular)

//光线追踪光强衰减公式 1/(a+b*dist+c*dist^2),其中a是常数衰减因子,b是线性衰减因子,c是二次性衰减因子
float Attenuate(uniform float attConst, uniform float attLin, uniform float attQuad, const in float distFragToL)
{
	return 1.0f / (attConst + attLin * distFragToL + attQuad *(distFragToL*distFragToL));
}

//计算环境光
//Iambient = Aintensity * Acolor ; (Aintensity表示环境光强度，Acolor表示环境光颜色)

//计算方向光漫反射光强
//Idiffuse = Dintensity*Dcolor*N.L ; (Dintensity表示漫反射强度，Dcolor表示漫反射光颜色，N为该点的法向量，L为光源向量)
float3 Diffuse(uniform float3 diffuseColor,
	uniform float diffuseIntensity,
	const in float att,
	const in float3 viewDirFragToL,
	const in float3 viewNormal)
{
	return diffuseColor * diffuseIntensity * att * max(0.0f, dot(viewDirFragToL, viewNormal));
}

//计算镜面高光
//Ispecular = Sintensity*Scolor*(R.V)n ; (Sintensity表示镜面光照强度，Scolor表示镜面光颜色，R为光的反射向量，V为观察者向量, n是高光反光度)
float3 Specular(const in float3 specularColor,
	uniform float specularIntensity,
	const in float3 viewNormal,
	const in float3 viewFragToL,
	const in float3 viewPos,
	const in float att,
	const in float specularPower)
{
	//反射光向量
	//OB = AO - 2（AO·N）·N  ->  OB = 2(OA·N)·N - OA
	const float3 r = normalize(2.0f *(dot(viewFragToL, viewNormal) * viewNormal) - viewFragToL);

	//在观察者空间
	const float3 viewFragToCam = normalize(-viewPos);

	return specularColor*specularIntensity*att*pow(max(0.0f, dot(r, viewFragToCam)), specularPower);

}

float3 MapNormal(const in float3 tan, 
	const in float3 bitan,
	const in float3 normal, 
	const in float2 tx, 
	uniform Texture2D nmap,
	uniform SamplerState sam)
{
	//法线贴图是在切线空间的(图像空间)，需要切线空间转到物体所在的局部空间
	//转换矩阵TBN 可以实现切线空间到模型空间的转换,由于tan/bitan/normal都已是观察空间向量，因此实现的是切线空间到观察空间的转换
	const float3x3 tanToTarget = float3x3(tan, bitan, normal);
	//对法线贴图进行解压操作(即除以255，用于将一个0-255范围内的数转换到浮点数区间[0 ,1])
	const float3 normalSample = nmap.Sample(sam, tx).xyz;
	//通过偏移，缩放将每个分量从[0 , 1]转换到[-1 , 1]完成操作
	const float3 tanNormal = 2.0f * normalSample - 1.0f;
	//将法线贴图乘以转换矩阵变换到局部空间
	return normalize(mul(tanNormal, tanToTarget));
}