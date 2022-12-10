#ifndef __SHADOWTESTRENDERGRAPH_H__
#define __SHADOWTESTRENDERGRAPH_H__

#pragma once

#include "RenderGraph.h"

class Graphic;

namespace Rgph
{
	class ShadowTestRenderGraph : public RenderGraph
	{
	public:
		ShadowTestRenderGraph(Graphic & gfx);
	};
}

#endif // !__SHADOWTESTRENDERGRAPH_H__