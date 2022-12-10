#include "BindingPass.h"

namespace Rgph
{
	BindingPass::BindingPass(string name, vector<shared_ptr<Bind::Bindable>> binds):
		Pass(move(name)),m_binds(move(binds))
	{
	}

	void BindingPass::AddBind(shared_ptr<Bind::Bindable> bind)
	{
		m_binds.push_back(move(bind));
	}

	void BindingPass::BindAll(Graphic & gfx)
	{
		//一个pass对应一个rendertaget和depthstencil
		//通常pass里的shader会用同一个（依据具体情况而定）
		BindBufferResource(gfx);
		for (auto & bind : m_binds)
		{
			bind->Bind(gfx);
		}
	}

	void BindingPass::BindBufferResource(Graphic & gfx)
	{
		if (m_renderTarget)
			m_renderTarget->BindAsBuffer(gfx, m_depthStencil.get());
		else
			m_depthStencil->BindAsBuffer(gfx);
	}
}