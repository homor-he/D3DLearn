#include "Projection.h"

Projection::Projection(Graphic & gfx, float width, float height, float nearZ, float farZ):
	m_width(width),m_height(height),m_nearZ(nearZ),m_farZ(farZ)
{
}

DirectX::XMMATRIX Projection::GetProjMatrix()
{
	return DirectX::XMMatrixPerspectiveLH(m_width, m_height, m_nearZ, m_farZ);
}
