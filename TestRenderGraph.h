#ifndef __TESTRENDERGRAPH_H__
#define __TESTRENDERGRAPH_H__

#pragma once

#include "RenderGraph.h"

class Graphic;

namespace Rgph
{
	class TestRenderGraph : public RenderGraph
	{
	public:
		TestRenderGraph(Graphic & gfx);
	};
}


#endif // !__TESTRENDERGRAPH_H__
