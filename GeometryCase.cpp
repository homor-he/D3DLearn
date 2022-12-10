#include "GeometryCase.h"

void DrawTest::GeometryCase::Init(Graphic & gfx)
{
	GeometryGenerator generator;
	generator.CreateBox(1.0f, 1.0f, 1.0f, m_boxMeshData);

	SetVertexBuffer(gfx, m_boxMeshData.Vertices);
	SetIndexBuffer(gfx, m_boxMeshData.Indices);
	SetVertexShaderAndInputLayout(gfx);
	SetPixelShader(gfx);

	string imagePath = "images\\WoodCrate02.dds"/*"images\\Muro_head_dm.png"*/;
	Bind::Texture tex(gfx, imagePath);
	tex.Bind(gfx);

	Bind::Sampler sample(gfx, Bind::Sampler::Type::Anisotropic, false, 0);
	sample.Bind(gfx);

	Bind::Topology topology(gfx, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	topology.Bind(gfx);
}

void DrawTest::GeometryCase::SetVertexBuffer(Graphic & gfx, vector<GeometryGenerator::Vertex>& vecArr)
{
	m_vtLayout.Append(VertexRela::VertexLayout::ElementType::Position3D);
	m_vtLayout.Append(VertexRela::VertexLayout::ElementType::Normal);
	m_vtLayout.Append(VertexRela::VertexLayout::ElementType::Tangent);
	m_vtLayout.Append(VertexRela::VertexLayout::ElementType::Texture2D);
	VertexRela::VertexBuffer vecBuff(m_vtLayout);
	for (int i = 0; i < vecArr.size(); ++i)
	{
		vecBuff.EmplaceBack(vecArr[i].Position, vecArr[i].Normal,
			vecArr[i].TangentU, vecArr[i].TexC);
	}
	Bind::VertexBuffer veBindBuffer(gfx,vecBuff);
	veBindBuffer.Bind(gfx);
}

void DrawTest::GeometryCase::SetIndexBuffer(Graphic & gfx, vector<unsigned short>& vecArr)
{
	Bind::IndexBuffer idBuffer(gfx,vecArr);
	idBuffer.Bind(gfx);
}

void DrawTest::GeometryCase::SetVertexShaderAndInputLayout(Graphic & gfx, string fileName)
{
	Bind::VertexShader vtShader(gfx, fileName);
	vtShader.Bind(gfx);

	Bind::InputLayout inputLayout(gfx, m_vtLayout,vtShader);
	inputLayout.Bind(gfx);
}

void DrawTest::GeometryCase::SetPixelShader(Graphic & gfx, string fileName)
{
	Bind::PixelShader pxShader(gfx, fileName);
	pxShader.Bind(gfx);
}

size_t DrawTest::GeometryCase::GetIndexArrSize()
{
	return m_boxMeshData.Indices.size();
}
