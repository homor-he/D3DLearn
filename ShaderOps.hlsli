
//һ������������ɫ���ɷ���(emissive)����������(ambient)��������(diffuse)�;��淴��(specular)�ȹ������õ��ܺ�
//���������ռ��㹫ʽ
//I = ������(Iambient) + �������(Idiffuse) + ����߹�(Ispecular)

//����׷�ٹ�ǿ˥����ʽ 1/(a+b*dist+c*dist^2),����a�ǳ���˥������,b������˥������,c�Ƕ�����˥������
float Attenuate(uniform float attConst, uniform float attLin, uniform float attQuad, const in float distFragToL)
{
	return 1.0f / (attConst + attLin * distFragToL + attQuad *(distFragToL*distFragToL));
}

//���㻷����
//Iambient = Aintensity * Acolor ; (Aintensity��ʾ������ǿ�ȣ�Acolor��ʾ��������ɫ)

//���㷽����������ǿ
//Idiffuse = Dintensity*Dcolor*N.L ; (Dintensity��ʾ������ǿ�ȣ�Dcolor��ʾ���������ɫ��NΪ�õ�ķ�������LΪ��Դ����)
float3 Diffuse(uniform float3 diffuseColor,
	uniform float diffuseIntensity,
	const in float att,
	const in float3 viewDirFragToL,
	const in float3 viewNormal)
{
	return diffuseColor * diffuseIntensity * att * max(0.0f, dot(viewDirFragToL, viewNormal));
}

//���㾵��߹�
//Ispecular = Sintensity*Scolor*(R.V)n ; (Sintensity��ʾ�������ǿ�ȣ�Scolor��ʾ�������ɫ��RΪ��ķ���������VΪ�۲�������, n�Ǹ߹ⷴ���)
float3 Specular(const in float3 specularColor,
	uniform float specularIntensity,
	const in float3 viewNormal,
	const in float3 viewFragToL,
	const in float3 viewPos,
	const in float att,
	const in float specularPower)
{
	//���������
	//OB = AO - 2��AO��N����N  ->  OB = 2(OA��N)��N - OA
	const float3 r = normalize(2.0f *(dot(viewFragToL, viewNormal) * viewNormal) - viewFragToL);

	//�ڹ۲��߿ռ�
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
	//������ͼ�������߿ռ��(ͼ��ռ�)����Ҫ���߿ռ�ת���������ڵľֲ��ռ�
	//ת������TBN ����ʵ�����߿ռ䵽ģ�Ϳռ��ת��,����tan/bitan/normal�����ǹ۲�ռ����������ʵ�ֵ������߿ռ䵽�۲�ռ��ת��
	const float3x3 tanToTarget = float3x3(tan, bitan, normal);
	//�Է�����ͼ���н�ѹ����(������255�����ڽ�һ��0-255��Χ�ڵ���ת��������������[0 ,1])
	const float3 normalSample = nmap.Sample(sam, tx).xyz;
	//ͨ��ƫ�ƣ����Ž�ÿ��������[0 , 1]ת����[-1 , 1]��ɲ���
	const float3 tanNormal = 2.0f * normalSample - 1.0f;
	//��������ͼ����ת������任���ֲ��ռ�
	return normalize(mul(tanNormal, tanToTarget));
}