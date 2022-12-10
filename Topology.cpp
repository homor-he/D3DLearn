#include "Topology.h"
#include "GraphicsThrowMacros.h"
#include "BindsDic.h"

namespace Bind
{
	Topology::Topology(Graphic & gfx, D3D11_PRIMITIVE_TOPOLOGY type):
		m_type(type)
	{
	}

	shared_ptr<Topology> Topology::Resolve(Graphic & gfx, D3D11_PRIMITIVE_TOPOLOGY type)
	{
		return BindsDic::Resolve<Topology>(gfx, type);
	}

	string Topology::GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type)
	{
		return typeid(Topology).name() + "#"s + std::to_string(type);
	}

	void Topology::Bind(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->IASetPrimitiveTopology(m_type));
	}
}