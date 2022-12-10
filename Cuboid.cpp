#include "Cuboid.h"
#include "GeometryGenerator.h"

Cuboid::Cuboid(Graphic & gfx, float size, GeometryType type, string name) :
	Cuboid(gfx, size,size,size,type,name)
{}

Cuboid::Cuboid(Graphic & gfx, float width, float height, float depth, GeometryType type, string name)
{
	GeometryGenerator generator;
	GeometryGenerator::MeshData meshData;
	generator.CreateBox(width, height, depth, meshData);

	string tag;
	if (name == "")
		tag = "$Cuboid" + to_string(width) + "w" + to_string(height) + "h" + to_string(depth) + "d";
	else
		tag = name + to_string(width) + "w" + to_string(height) + "h" + to_string(depth) + "d";

	VertexRela::VertexLayout vtLayout;
	if (type == GeometryType::Common)
	{
		vtLayout.Append(VertexRela::VertexLayout::ElementType::Position3D);
		vtLayout.Append(VertexRela::VertexLayout::ElementType::Normal);
		vtLayout.Append(VertexRela::VertexLayout::ElementType::Texture2D);
		vtLayout.Append(VertexRela::VertexLayout::ElementType::Tangent);
		m_vtxBuffer = make_shared<VertexRela::VertexBuffer>(vtLayout);
		for (int i = 0; i < meshData.Vertices.size(); ++i)
		{
			m_vtxBuffer->EmplaceBack(meshData.Vertices[i].Position, meshData.Vertices[i].Normal, 
				meshData.Vertices[i].TexC, meshData.Vertices[i].TangentU);
		}
	}
	else if (type == GeometryType::Solid)
	{
		vtLayout.Append(VertexRela::VertexLayout::ElementType::Position3D);
		m_vtxBuffer = make_shared<VertexRela::VertexBuffer>(vtLayout);
		for (int i = 0; i < meshData.Vertices.size(); ++i)
		{
			m_vtxBuffer->EmplaceBack(meshData.Vertices[i].Position);
		}
	}
	m_pVtBuffer = Bind::VertexBuffer::Resolve(gfx, tag, *m_vtxBuffer);
	m_pIdBuffer = Bind::IndexBuffer::Resolve(gfx, tag, meshData.Indices);
	m_pTopology = Bind::Topology::Resolve(gfx);
}

void Cuboid::SetPos(DirectX::XMFLOAT3 pos)
{
	m_pos = pos;
}

void Cuboid::SetRotation(float pitch, float yaw, float roll)
{
	m_pitch = pitch;
	m_yaw = yaw;
	m_roll = roll;
}

void Cuboid::SetRotSpeedAndStat(float rotspeed, RotaStat rotStat)
{
	m_rotSpeedPerFrame = rotspeed;
	m_rotStat = rotStat;
}

DirectX::XMMATRIX Cuboid::GetTransform()
{
	m_rotSpeed += m_rotSpeedPerFrame;
	if (m_rotSpeed == 0.0f)
		return DirectX::XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll)*
		DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	else if (m_rotStat == RotaStat::Rotation)
		return DirectX::XMMatrixRotationRollPitchYaw(m_pitch * m_rotSpeed, m_yaw * m_rotSpeed, m_roll *m_rotSpeed) *
		DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
		/*return DirectX::XMMatrixRotationRollPitchYaw(m_pitch*m_rotSpeed, m_yaw*m_rotSpeed, m_roll*m_rotSpeed)*
		DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);*/
	else 
		return DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z) *
		DirectX::XMMatrixRotationRollPitchYaw(m_pitch*m_rotSpeed, m_yaw*m_rotSpeed, m_roll*m_rotSpeed);
}
