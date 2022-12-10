#ifndef __LAMBERTIANPASS_H__
#define __LAMBERTIANPASS_H__

#pragma once

#include "RenderQueuePass.h"
#include "ShadowCameraCBuf.h"
#include "ShadowSampler.h"
#include "Sampler.h"
#include "Camera.h"

class Graphic;

namespace Rgph
{
	class LambertianPass : public RenderQueuePass
	{
	public:
		LambertianPass(Graphic & gfx, string name) : RenderQueuePass(move(name)),
			m_pShadowCBuf(make_shared<Bind::ShadowCameraCBuf>(gfx))
		{
			using namespace Bind;
			//把renderTarget和槽绑定，用于绑定其他pass的source
			//renderTarget/depthStencil传递流程: 其他pass的source -> 本pass的sink = 同名注册的本pass的source ->其他pass的sink   
			RegisterSink(DirectBufferSink<RenderTarget>::Make("renderTarget", m_renderTarget));
			RegisterSink(DirectBufferSink<DepthStencil>::Make("depthStencil", m_depthStencil));
			//把renderTarget和源绑定，用于给其他pass的sink绑定
			RegisterSource(DirectBufferSource<RenderTarget>::Make("renderTarget", m_renderTarget));
			RegisterSource(DirectBufferSource<DepthStencil>::Make("depthStencil", m_depthStencil));

			//给VSShadow.hlsli的shadowPos赋值,用于将物体坐标转到光源摄像机剪裁(clip)空间
			AddBind(m_pShadowCBuf);
			//留一个槽，和其他pass传入的source绑定
			AddBindSink<Bind::Bindable>("shadowMap");
			//给_PShadow_Static.hlsli的ssam赋值或_PShadow_Dynamic.hlsli的ssamHw，用于深度采样
			AddBind(make_shared<Bind::ShadowSampler>(gfx));
			//给_PShadow_Dynamic.hlsli的ssamSw赋值，PShadow_Static.hlsli用不上
			AddBind(make_shared<Bind::Sampler>(gfx, Bind::Sampler::Type::Anisotropic, false, 2));
			AddBind(Stencil::Resolve(gfx, Bind::Stencil::Mode::Off));
		}

		void BindMainCamera(Camera & cam)
		{
			m_pMainCamera = &cam;
		}

		void BindShadowCamera(Camera & cam)
		{
			m_pShadowCBuf->SetCamera(&cam);
		}

		void Execute(Graphic &gfx) override
		{
			m_pShadowCBuf->Update(gfx);
			m_pMainCamera->BindToGraphic(gfx);
			RenderQueuePass::Execute(gfx);
		}
	private:
		shared_ptr<Bind::ShadowCameraCBuf> m_pShadowCBuf;
		Camera * m_pMainCamera = nullptr;
	};
}

#endif // !__LAMBERTIANPASS_H__
