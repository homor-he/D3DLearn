#ifndef __TESTPASS_H__
#define __TESTPASS_H__

#pragma once

#include "RenderQueuePass.h"
#include "Stencil.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Blender.h"

class Graphic;

namespace Rgph
{
	class TestPass : public RenderQueuePass
	{
	public:
		TestPass(Graphic & gfx, string name) :
			RenderQueuePass(move(name))
		{
			using namespace Bind;
			/*RegisterSink(DirectBufferSink<RenderTarget>::Make("renderTarget", m_renderTarget));
			RegisterSink(DirectBufferSink<DepthStencil>::Make("depthStencil", m_depthStencil));
			RegisterSource(DirectBufferSource<RenderTarget>::Make("renderTarget", m_renderTarget));
			RegisterSource(DirectBufferSource<DepthStencil>::Make("depthStencil", m_depthStencil));*/
			//AddBind(Stencil::Resolve(gfx, Stencil::Mode::DepthReversed));

			RegisterSink(DirectBufferSink<Bind::RenderTarget>::Make("renderTarget", m_renderTarget));
			RegisterSink(DirectBufferSink<Bind::DepthStencil>::Make("depthStencil", m_depthStencil));
			RegisterSource(DirectBufferSource<Bind::RenderTarget>::Make("renderTarget", m_renderTarget));
			RegisterSource(DirectBufferSource<Bind::DepthStencil>::Make("depthStencil", m_depthStencil));
			//AddBind(VertexShader::Resolve(gfx, "SolidVS.cso"));
			//AddBind(PixelShader::Resolve(gfx, "SolidPS.cso"));
			//AddBind(Stencil::Resolve(gfx, Stencil::Mode::Mask));
			//AddBind(Blender::Resolve(gfx, false));
		}

		/*void Execute(Graphic & gfx) override
		{
			m_renderTarget->Clear(gfx);
			RenderQueuePass::Execute(gfx);
		}*/
	};
}


#endif // !__TESTPASS_H__
