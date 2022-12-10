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
			//�ص���Ȳ��ԣ���mesh���»��ƣ���meshͬ����λ��д��ģ��ֵref����д����ɫֵ��PS��Ϊnullptr��
			AddBind(VertexShader::Resolve(gfx, "SolidVS.cso"));
			AddBind(NullPixelShader::Resolve(gfx));
			AddBind(Stencil::Resolve(gfx, Stencil::Mode::Write));
			AddBind(Rasterizer::Resolve(gfx, false));
		}
	};
}

#endif // !__OUTLINEMASKGENERATIONPASS_H__
