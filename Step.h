#ifndef __STEP_H__
#define __STEP_H__

#pragma once

#include <vector>
#include <memory>
#include "Bindable.h"

namespace Rgph
{
	class RenderQueuePass;
	class RenderGraph;
}

class Step
{
public:
	Step(std::string targetPassName);
	void AddBindable(shared_ptr<Bind::Bindable> bind);
	void Submit(Drawable & gfx);
	void Bind(Graphic & gfx);
	void InitializeParentReferences(Drawable& parent);
	void Link(Rgph::RenderGraph & rg);
private:
	std::vector<std::shared_ptr<Bind::Bindable>> m_bindables;
	std::string m_targetPassName;
	Rgph::RenderQueuePass * m_pRenderQueuePass = nullptr;
};

#endif // !__STEP_H__