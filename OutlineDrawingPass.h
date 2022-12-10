#ifndef __OUTLINEDRAWINGPASS_H__
#define __OUTLINEDRAWINGPASS_H__

#pragma once

#include "RenderQueuePass.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Stencil.h"
#include "Rasterizer.h"

namespace Rgph
{
	class OutlineDrawingPass : public RenderQueuePass
	{
	public:
		OutlineDrawingPass(Graphic & gfx, string name) : RenderQueuePass(move(name))
		{
			using namespace Bind;
			RegisterSink(DirectBufferSink<RenderTarget>::Make("renderTarget", m_renderTarget));
			RegisterSink(DirectBufferSink<DepthStencil>::Make("depthStencil", m_depthStencil));
			RegisterSource(DirectBufferSource<RenderTarget>::Make("renderTarget", m_renderTarget));
			RegisterSource(DirectBufferSource<DepthStencil>::Make("depthStencil", m_depthStencil));
			AddBind(VertexShader::Resolve(gfx, "SolidVS.cso"));
			AddBind(PixelShader::Resolve(gfx, "SolidPS.cso"));
			//两个物体重叠时，会绘制后渲染的一个物体，而不会交替显示
			AddBind(Stencil::Resolve(gfx, Stencil::Mode::Mask));
			AddBind(Rasterizer::Resolve(gfx, false));
		}
	};
}


#endif // !__OUTLINEDRAWINGPASS_H__
