#include "SpotLight.h"

SpotLight::SpotLight(Graphic & gfx, DirectX::XMFLOAT3 pos, float radius):
	m_solidSphere(gfx, radius, GeometryType::Solid), m_cBuf(gfx,2)
{
	m_home = {
		pos,
		{ 0.5f,0.5f,0.5f },
		{ 0.5f,0.5f,0.5f },
		2.0f,
		1.0f,
		0.025f,
		0.0030f,
		{0.0f,-5.0f,0.0f}, 
		10.0f,			
		true,
	};
	m_cbData = m_home;
	m_solidSphere.SetPos(pos);
}

void SpotLight::Bind(Graphic & gfx, DirectX::XMMATRIX view)
{
	SpotLightCBuf copy = m_cbData;
	//DirectX::XMVECTOR pos =  DirectX::XMLoadFloat3(&m_cbData.pos);
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&m_solidSphere.GetCBPos());
	DirectX::XMStoreFloat3(&copy.pos, DirectX::XMVector3Transform(pos, view));
	m_cBuf.Update(gfx, copy);
	m_cBuf.Bind(gfx);
}
