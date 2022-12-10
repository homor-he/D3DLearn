#include "Drawable.h"
#include "BindableCommon.h"
#include "Material.h"

Drawable::Drawable(Graphic & gfx, Material & mat, aiMesh & mesh, float scale)
{
	m_pVtBuffer = mat.MakeVertexBindable(gfx, mesh, scale);
	m_pIdBuffer = mat.MakeIndexBindable(gfx, mesh);
	m_pTopology = Bind::Topology::Resolve(gfx);

	for (auto & tech : mat.GetTechniques())
	{
		AddTechnique(move(tech));
	}
}

Drawable::~Drawable()
{
}

void Drawable::AddTechnique(Technique tech)
{
	tech.InitializeParentReference(*this);
	m_techniques.push_back(move(tech));
}

void Drawable::Bind(Graphic & gfx)
{
	m_pVtBuffer->Bind(gfx);
	m_pIdBuffer->Bind(gfx);
	m_pTopology->Bind(gfx);
}

UINT Drawable::GetIndexCount() 
{
	return m_pIdBuffer->GetCount();
}

void Drawable::Submit(size_t channelFilter)
{
	for (auto& tech : m_techniques)
	{
		tech.Submit(*this, channelFilter);
	}
}

void Drawable::LinkTechnique(Rgph::RenderGraph & rg)
{
	for (auto & tech : m_techniques)
	{
		tech.Link(rg);
	}
}
