#include "Step.h"
#include "RenderQueuePass.h"
#include "RenderGraph.h"

Step::Step(std::string targetPassName):
	m_targetPassName(move(targetPassName))
{
}

void Step::AddBindable(shared_ptr<Bind::Bindable> bind)
{
	m_bindables.push_back(move(bind));
}

void Step::Submit(Drawable & gfx)
{
	m_pRenderQueuePass->Accept(Rgph::Job{ this,&gfx });
}

void Step::Bind(Graphic & gfx)
{
	for (auto & b : m_bindables)
	{
		b->Bind(gfx);
	}
}

void Step::InitializeParentReferences(Drawable & parent)
{
	for (auto & b: m_bindables)
	{
		b->InitializeParentRef(parent);
	}
}

void Step::Link(Rgph::RenderGraph & rg)
{
	m_pRenderQueuePass = &rg.GetRenderQueue(m_targetPassName);
}
