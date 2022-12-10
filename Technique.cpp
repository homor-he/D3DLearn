#include "Technique.h"

Technique::Technique(size_t channels):
	m_channels(channels)
{
}

Technique::Technique(std::string name, size_t channels, bool startActive):
	m_name(name), m_channels(channels), m_active(startActive)
{
}

void Technique::Submit(Drawable & drawable, size_t channelFilter)
{
	if (m_active && (m_channels & channelFilter) != 0)
	{
		for (auto & step : m_steps)
		{
			//1个technique对应N个step,1个step对应1个job，
			//对一个Drawable进行渲染可能需要多个step
			//job可以理解为对一个Drawable进行其中一步的渲染并Drawindexed
			step.Submit(drawable);
		}
	}
}

void Technique::AddStep(Step step)
{
	m_steps.push_back(move(step));
}

void Technique::InitializeParentReference(Drawable & drawable)
{
	for (auto & b : m_steps)
	{
		b.InitializeParentReferences(drawable);
	}
}

void Technique::Link(Rgph::RenderGraph & rg)
{
	for (auto & step : m_steps)
	{
		step.Link(rg);
	}
}
