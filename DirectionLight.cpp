#include "DirectionLight.h"

DirectLight::DirectLight(Graphic & gfx, DirectX::XMFLOAT3 pos, float radius)
	: m_solidSphere(gfx,radius,GeometryType::Solid), m_cBuf(gfx,3)
{
	m_home = {
		{ 0.5f,0.5f,0.5f },
		{ 0.5f,0.5f,0.5f },
		1.0f,
		{ 0.0f,-5.0f,0.0f },
		true,
	};
	m_cbData = m_home;
	m_solidSphere.SetPos(pos);
}

void DirectLight::Bind(Graphic & gfx, DirectX::XMMATRIX view)
{
	//DirectLightCBuf copy = m_cbData;
	//DirectX::XMVECTOR pos =  DirectX::XMLoadFloat3(&m_cbData.pos);
	//DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&m_solidSphere.GetCBPos());
	//DirectX::XMStoreFloat3(&copy.pos, DirectX::XMVector3Transform(pos, view));
	m_cBuf.Update(gfx, m_cbData);
	m_cBuf.Bind(gfx);
}
