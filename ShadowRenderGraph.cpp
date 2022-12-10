#include "ShadowRenderGraph.h"
#include "ShadowMappingPass.h"
#include "LambertianPass.h"
#include "BufferClearPass.h"
#include "SkyboxPass.h"

namespace Rgph
{
	ShadowRenderGraph::ShadowRenderGraph(Graphic & gfx) : RenderGraph(gfx)
	{
		{
			auto pass = make_unique<ShadowMappingPass>(gfx, "shadowMap");
			AppendPass(move(pass));
		}

		{
			auto pass = make_unique<LambertianPass>(gfx, "lambertian");
			//将shadowMapingPass光源相机深度图传入lambertianPass
			pass->SetSinkLinkage("shadowMap", "shadowMap.map");
			pass->SetSinkLinkage("renderTarget", "$.backbuffer");
			pass->SetSinkLinkage("depthStencil", "$.masterDepth");
			AppendPass(move(pass));
		}

		{
			auto pass = make_unique<SkyboxPass>(gfx, "skybox");
			pass->SetSinkLinkage("renderTarget", "lambertian.renderTarget");
			pass->SetSinkLinkage("depthStencil", "lambertian.depthStencil");
			AppendPass(move(pass));
		}

		SetSinkTarget("backbuffer", "skybox.renderTarget");
		Finalize();
	}

	void ShadowRenderGraph::BindMainCamera(Camera & cam)
	{
		dynamic_cast<LambertianPass&>(FindPassByName("lambertian")).BindMainCamera(cam);
	}

	void ShadowRenderGraph::BindShadowCamera(Camera & cam)
	{
		dynamic_cast<ShadowMappingPass&>(FindPassByName("shadowMap")).BindShadowCamera(cam);
		dynamic_cast<LambertianPass&>(FindPassByName("lambertian")).BindShadowCamera(cam);
	}

	void ShadowRenderGraph::DumpShadowMap(Graphic & gfx,const string & path)
	{
		dynamic_cast<ShadowMappingPass&>(FindPassByName("shadowMap")).DumpShadowMap(gfx, path);
		//m_masterDepth->ToSurface(gfx).Save(path);
	}
}

