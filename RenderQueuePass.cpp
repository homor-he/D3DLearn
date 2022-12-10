#include "RenderQueuePass.h"

namespace Rgph
{
	RenderQueuePass::RenderQueuePass(string name, vector<shared_ptr<Bind::Bindable>> binds):
		BindingPass(move(name),move(binds))
	{
	}

	void RenderQueuePass::Execute(Graphic & gfx)
	{
		BindAll(gfx);
		for (auto & j : m_jobs)
		{
			j.Execute(gfx);
		}
	}

	void RenderQueuePass::Reset()
	{
		m_jobs.clear();
	}

	void RenderQueuePass::Accept(Job job)
	{
		m_jobs.push_back(job);
	}
}