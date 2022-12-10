#include "TestRenderGraph.h"
#include "TestPass.h"
#include "LambertianPass.h"
#include "OutlineMaskGenerationPass.h"
#include "OutlineDrawingPass.h"
#include "BufferClearPass.h"
#include "SkyboxPass.h"

namespace Rgph
{
	TestRenderGraph::TestRenderGraph(Graphic & gfx) :
		RenderGraph(gfx)
	{
		//{
		//	auto pass = std::make_unique<BufferClearPass>("clearRT");
		//	pass->SetSinkLinkage("buffer", "$.backbuffer");
		//	AppendPass(std::move(pass));
		//}
		//{
		//	auto pass = std::make_unique<BufferClearPass>("clearDS");
		//	//把上一个pass的source绑定到这一个pass的sink
		//	pass->SetSinkLinkage("buffer", "$.masterDepth");
		//	AppendPass(std::move(pass));
		//}

		unique_ptr<Rgph::TestPass> lp = make_unique<Rgph::TestPass>(gfx, "test");
		lp->SetSinkLinkage("renderTarget", "$.backbuffer");
		lp->SetSinkLinkage("depthStencil", "$.masterDepth");
		AppendPass(move(lp));

		/*unique_ptr<Rgph::SkyboxPass> sp = make_unique<Rgph::SkyboxPass>(gfx, "skybox");
		sp->SetSinkLinkage("renderTarget", "lambertian.renderTarget");
		sp->SetSinkLinkage("depthStencil", "lambertian.depthStencil");
		AppendPass(move(sp));*/

		unique_ptr<Rgph::OutlineMaskGenerationPass> ogp = make_unique<Rgph::OutlineMaskGenerationPass>(gfx, "outlineMask");
		//ogp->SetSinkLinkage("renderTarget", "lambertian.renderTarget");
		ogp->SetSinkLinkage("depthStencil", "test.depthStencil");
		AppendPass(move(ogp));

		unique_ptr<Rgph::OutlineDrawingPass> odp = make_unique<Rgph::OutlineDrawingPass>(gfx, "outlineDraw");
		odp->SetSinkLinkage("renderTarget", "test.renderTarget");
		odp->SetSinkLinkage("depthStencil", "outlineMask.depthStencil");
		AppendPass(move(odp));

		SetSinkTarget("backbuffer", "outlineDraw.renderTarget");
		Finalize();
	}
}