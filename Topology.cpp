#include "Topology.h"
#include "GraphicsThrowMacros.h"

namespace Bind
{
	Topology::Topology(Graphic & gfx, D3D11_PRIMITIVE_TOPOLOGY type):
		m_type(type)
	{
	}

	void Topology::Bind(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->IASetPrimitiveTopology(m_type));
	}
}