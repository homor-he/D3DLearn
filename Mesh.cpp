#include "Mesh.h"

Mesh::Mesh(Graphic & gfx, Material & mat, aiMesh & mesh, float scale):
	Drawable(gfx,mat,mesh,scale)
{
	DirectX::XMStoreFloat4x4(&m_transform, DirectX::XMMatrixIdentity());
}

DirectX::XMMATRIX Mesh::GetTransform()
{
	//return DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	return DirectX::XMLoadFloat4x4(&m_transform);
}

void Mesh::Submit(size_t channel, DirectX::FXMMATRIX transform)
{
	DirectX::XMStoreFloat4x4(&m_transform, transform);
	Drawable::Submit(channel);
}
