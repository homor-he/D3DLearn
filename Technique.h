#ifndef __TECHNIQUE_H__
#define __TECHNIQUE_H__

#pragma once

#include <vector>
#include "Step.h"

namespace Rgph
{
	class RenderGraph;
}

class Technique
{
public:
	Technique(size_t channels);
	Technique(std::string name, size_t channels, bool startActive = true);
	void Submit(Drawable & drawable, size_t channelFilter);
	void AddStep(Step step);
	void InitializeParentReference(Drawable & drawable);
	void Link(Rgph::RenderGraph &);
private:
	bool m_active = true;
	std::vector<Step> m_steps;
	std::string m_name;
	size_t m_channels;
};

#endif // !__TECHNIQUE_H__

