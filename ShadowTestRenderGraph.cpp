#include "ShadowTestRenderGraph.h"

namespace Rgph
{
	ShadowTestRenderGraph::ShadowTestRenderGraph(Graphic & gfx)
		: RenderGraph(gfx)
	{
		//{
		//	auto pass = std::make_unique<BufferClearPass>("clearRT");
		//	pass->SetSinkLinkage("buffer", "$.backbuffer");
		//	AppendPass(std::move(pass));
		//}
		//{
		//	auto pass = std::make_unique<BufferClearPass>("clearDS");
		//	//����һ��pass��source�󶨵���һ��pass��sink
		//	pass->SetSinkLinkage("buffer", "$.masterDepth");
		//	AppendPass(std::move(pass));
		//}

		
	}
}