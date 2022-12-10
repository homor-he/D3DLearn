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
			//1��technique��ӦN��step,1��step��Ӧ1��job��
			//��һ��Drawable������Ⱦ������Ҫ���step
			//job�������Ϊ��һ��Drawable��������һ������Ⱦ��Drawindexed
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
