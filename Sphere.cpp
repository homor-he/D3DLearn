#include "Sphere.h"
#include "BindableCommon.h"
#include "GeometryGenerator.h"


Sphere::Sphere(Graphic & gfx, float radius, GeometryType type, string name)
{
	GeometryGenerator generator;
	VertexRela::VertexLayout vtLayout;
	string tag;
	if (name == "")
	{
		tag = "$Sphere" + to_string(radius);
	}
	else
	{
		tag = name + to_string(radius);
	}

	switch (type)
	{
	case Common:
	{
		GeometryGenerator::MeshData meshData;
		generator.CreateSphere(radius, 40u, 80u, meshData);

		vtLayout.Append(VertexRela::VertexLayout::ElementType::Position3D);
		vtLayout.Append(VertexRela::VertexLayout::ElementType::Normal);
		vtLayout.Append(VertexRela::VertexLayout::ElementType::Tangent);
		vtLayout.Append(VertexRela::VertexLayout::ElementType::Texture2D);
		m_vtxBuffer = make_shared<VertexRela::VertexBuffer>(vtLayout);
		for (int i = 0; i < meshData.Vertices.size(); ++i)
		{
			m_vtxBuffer->EmplaceBack(meshData.Vertices[i].Position, meshData.Vertices[i].Normal,
				meshData.Vertices[i].TangentU, meshData.Vertices[i].TexC);
		}
		m_pVtBuffer = Bind::VertexBuffer::Resolve(gfx, tag, *m_vtxBuffer);
		m_pIdBuffer = Bind::IndexBuffer::Resolve(gfx, tag, meshData.Indices);
		break;
	}
	case Solid:
	{
		GeometryGenerator::SolidMeshData meshData;
		generator.CreateSolidSphere(radius, meshData);

		vtLayout.Append(VertexRela::VertexLayout::ElementType::Position3D);
		m_vtxBuffer = make_shared<VertexRela::VertexBuffer>(vtLayout);
		for (int i = 0; i < meshData.Vertices.size(); ++i)
		{
			m_vtxBuffer->EmplaceBack(meshData.Vertices[i]);
		}
		m_pVtBuffer = Bind::VertexBuffer::Resolve(gfx, tag, *m_vtxBuffer);
		m_pIdBuffer = Bind::IndexBuffer::Resolve(gfx, tag, meshData.Indices);
		break;
	}
	default:
		break;
	}
	m_pTopology = Bind::Topology::Resolve(gfx);
}

void Sphere::SetPos(DirectX::XMFLOAT3 pos)
{
	m_pos = pos;
	m_cbPos = m_pos;
}

DirectX::XMFLOAT3 Sphere::GetCBPos()
{
	return m_cbPos;
}

DirectX::XMMATRIX Sphere::GetTransform()
{
	m_rotSpeed += m_rotSpeedPerFrame;
	DirectX::XMMATRIX rotation = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX transform = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX res = DirectX::XMMatrixIdentity();
	if (m_rotSpeed == 0.0f)
		return DirectX::XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll)*
		DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	else if (m_rotStat == RotaStat::Rotation)
	{
		rotation = DirectX::XMMatrixRotationRollPitchYaw(m_pitch * m_rotSpeed, m_yaw * m_rotSpeed, m_roll *m_rotSpeed);
		transform = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
		res = DirectX::XMMatrixMultiply(rotation, transform);
		DirectX::XMStoreFloat3(&m_cbPos, res.r[3]);
		return res;
	}
	else
	{
		rotation = DirectX::XMMatrixRotationRollPitchYaw(m_pitch * m_rotSpeed, m_yaw * m_rotSpeed, m_roll *m_rotSpeed);
		transform = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
		res = DirectX::XMMatrixMultiply(transform, rotation);
		DirectX::XMStoreFloat3(&m_cbPos, res.r[3]);
		return res;
	}
}

void Sphere::SetRotation(float pitch, float yaw, float roll)
{
	m_pitch = pitch;
	m_yaw = yaw;
	m_roll = roll;
}

void Sphere::SetRotSpeedAndStat(float rotspeed, RotaStat rotStat)
{
	m_rotSpeedPerFrame = rotspeed;
	m_rotStat = rotStat;
}
