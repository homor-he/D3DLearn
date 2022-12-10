#ifndef __OUTLINEMASKGENERATIONPASS_H__
#define __OUTLINEMASKGENERATIONPASS_H__

#pragma once

#include "RenderQueuePass.h"
#include "VertexShader.h"
#include "NullPixelShader.h"
#include "Stencil.h"
#include "Rasterizer.h"

class Graphic;

namespace Rgph
{
	class OutlineMaskGenerationPass : public RenderQueuePass
	{
	public:
		OutlineMaskGenerationPass(Graphic & gfx, string name) :
			RenderQueuePass(move(name))
		{
			using namespace Bind;
			RegisterSink(DirectBufferSink<DepthStencil>::Make("depthStencil", m_depthStencil));
			RegisterSource(DirectBufferSource<DepthStencil>::Make("depthStencil", m_depthStencil));
			//关掉深度测试，将mesh重新绘制，在mesh同样的位置写入模版值ref，不写入颜色值（PS设为nullptr）
			AddBind(VertexShader::Resolve(gfx, "SolidVS.cso"));
			AddBind(NullPixelShader::Resolve(gfx));
			AddBind(Stencil::Resolve(gfx, Stencil::Mode::Write));
			AddBind(Rasterizer::Resolve(gfx, false));
		}
	};
}

#endif // !__OUTLINEMASKGENERATIONPASS_H__
