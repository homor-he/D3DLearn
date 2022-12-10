#ifndef __TOPOLOGY_H__
#define __TOPOLOGY_H__

#pragma once

#include "Bindable.h"

namespace Bind
{
	class Topology : public Bindable
	{
	public:
		Topology(Graphic & gfx, D3D11_PRIMITIVE_TOPOLOGY type);
		static shared_ptr<Topology> Resolve(Graphic & gfx, D3D11_PRIMITIVE_TOPOLOGY type = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		static string GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type);
		void Bind(Graphic & gfx) override;
	protected:
		D3D11_PRIMITIVE_TOPOLOGY m_type;
	};
}

#endif // !__TOPOLOGY_H__
