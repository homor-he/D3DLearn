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
			//��renderTarget�Ͳ۰󶨣����ڰ�����pass��source
			//renderTarget/depthStencil��������: ����pass��source -> ��pass��sink = ͬ��ע��ı�pass��source ->����pass��sink   
			RegisterSink(DirectBufferSink<RenderTarget>::Make("renderTarget", m_renderTarget));
			RegisterSink(DirectBufferSink<DepthStencil>::Make("depthStencil", m_depthStencil));
			//��renderTarget��Դ�󶨣����ڸ�����pass��sink��
			RegisterSource(DirectBufferSource<RenderTarget>::Make("renderTarget", m_renderTarget));
			RegisterSource(DirectBufferSource<DepthStencil>::Make("depthStencil", m_depthStencil));

			//��VSShadow.hlsli��shadowPos��ֵ,���ڽ���������ת����Դ���������(clip)�ռ�
			AddBind(m_pShadowCBuf);
			//��һ���ۣ�������pass�����source��
			AddBindSink<Bind::Bindable>("shadowMap");
			//��_PShadow_Static.hlsli��ssam��ֵ��_PShadow_Dynamic.hlsli��ssamHw��������Ȳ���
			AddBind(make_shared<Bind::ShadowSampler>(gfx));
			//��_PShadow_Dynamic.hlsli��ssamSw��ֵ��PShadow_Static.hlsli�ò���
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
