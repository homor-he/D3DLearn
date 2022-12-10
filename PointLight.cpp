#include "PointLight.h"
#include "Camera.h"

PointLight::PointLight(Graphic & gfx, DirectX::XMFLOAT3 pos, float radius):
	m_solidSphere(gfx,radius, GeometryType::Solid), m_cBuf(gfx)
{
	m_home = {
		pos,
		{ 0.5f,0.5f,0.5f },
		{ 0.5f,0.5f,0.5f },
		pos,
		{0.0f,0.0f,0.0f},
		6.0f,
		1.0f,
		0.025f,
		0.0030f,
		true,
	};
	m_cbData = m_home;
	m_solidSphere.SetPos(pos);
	//�����������ʼ�����趨pith��yawֵ(����)������ĳ�ʼ�����(pitch=0.0f��yaw=0.0f)����+z����(��Ļ����)�����˳���+y����ѭ���ֶ��򣨴�Ĵָ����Ϊ+z,ʳָ����Ϊ+x����ָ����Ϊ+y��
	//��תʱ����ÿ�������ת�������൱��-����ʳָΪ����4ָ��������Ϊ������
	m_pCamera = make_shared<Camera>(gfx, "Light", ((float)gfx.GetWidth())/gfx.GetHeight(), m_cbData.viewPos, DirectX::XM_PI*0.5f, 0.0f);
}

void PointLight::Bind(Graphic & gfx, DirectX::XMMATRIX view, DirectX::XMFLOAT3 mainCamWorldPos)
{
	PointLightCBuf copy = m_cbData;
	//DirectX::XMVECTOR pos =  DirectX::XMLoadFloat3(&m_cbData.pos);
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&m_solidSphere.GetCBPos());
	DirectX::XMStoreFloat3(&copy.worldPos, pos);
	DirectX::XMStoreFloat3(&copy.viewPos, DirectX::XMVector3Transform(pos, view));
	copy.mainCameraPos = mainCamWorldPos;
	m_cBuf.Update(gfx, copy);
	m_cBuf.Bind(gfx);
	//�����������������
	m_pCamera->SetWorldPos(m_solidSphere.GetCBPos());
}

void PointLight::Submit(size_t channel)
{
	//m_solidSphere.SetPos(m_cbData.pos);
	//m_solidSphere.Submit(channel);
}

shared_ptr<Camera> PointLight::ShareCamera()
{
	return m_pCamera;
}
