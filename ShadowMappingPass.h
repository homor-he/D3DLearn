#ifndef __SHADOWMAPPINGPASS_H__
#define __SHADOWMAPPINGPASS_H__

#pragma once

#include "RenderQueuePass.h"
#include "RenderTarget.h"
#include "CubeTexture.h"
#include "VertexShader.h"
#include "NullPixelShader.h"
#include "PixelShader.h"
#include "Stencil.h"
#include "Blender.h"
#include "Viewport.h"
#include "ShadowRasterizer.h"
#include "Camera.h"

class Graphic;

namespace Rgph
{
	class ShadowMappingPass : public RenderQueuePass
	{
	public:
		ShadowMappingPass(Graphic & gfx, string name) : RenderQueuePass(move(name))
		{
			using namespace Bind;
			//获取深度图，设置_PShadow_Static中的smap,用于采样
			m_pRenderCube = make_shared<CubeTargetTexture>(gfx, gfx.GetWidth(), gfx.GetHeight(), 3, DXGI_FORMAT_R32_FLOAT);
			//m_depthStencil = make_shared<ShaderInputDepthStencil>(gfx, gfx.GetWidth(), gfx.GetHeight(), 3, DepthStencil::Usage::ShadowDepth);
			m_depthStencil = make_shared<OutputOnlyDepthStencil>(gfx);
			AddBind(VertexShader::Resolve(gfx, "Shadow_VS.cso"));
			//禁用像素着色器(使用depthstencil时,深度采用硬件默认的)
			//AddBind(NullPixelShader::Resolve(gfx));
			//使用像素着色器(使用rendertarget时,深度采用计算过的)
			AddBind(PixelShader::Resolve(gfx, "Shadow_PS.cso"));
			AddBind(Stencil::Resolve(gfx, Stencil::Mode::Off));
			AddBind(Blender::Resolve(gfx, false));
			AddBind(make_shared<Viewport>(gfx, (float)gfx.GetWidth(), (float)gfx.GetHeight()));
			AddBind(make_shared<Bind::ShadowRasterizer>(gfx, 50, 2.0f, 0.1f));
			//用于传给下个pass的sink
			RegisterSource(DirectBindableSource<Bind::CubeTargetTexture>::Make("map", m_pRenderCube));
			//RegisterSource(DirectBindableSource<Bind::DepthStencil>::Make("map", m_depthStencil));

			//设置光源摄像机的投影矩阵
			DirectX::XMStoreFloat4x4(&m_projection, DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI / 2, 1.0f, 0.5f, 100.0f));

			//三维空间6个方向，shadowCamera的up和direct向量
			//+x
			DirectX::XMStoreFloat3(&m_vecCameraDirection[0], DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));
			DirectX::XMStoreFloat3(&m_vecCameraUp[0], DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
			//-x
			DirectX::XMStoreFloat3(&m_vecCameraDirection[1], DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f));
			DirectX::XMStoreFloat3(&m_vecCameraUp[1], DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
			//+y
			DirectX::XMStoreFloat3(&m_vecCameraDirection[2], DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
			DirectX::XMStoreFloat3(&m_vecCameraUp[2], DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f));
			//-y
			DirectX::XMStoreFloat3(&m_vecCameraDirection[3], DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f));
			DirectX::XMStoreFloat3(&m_vecCameraUp[3], DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
			//+z
			DirectX::XMStoreFloat3(&m_vecCameraDirection[4], DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
			DirectX::XMStoreFloat3(&m_vecCameraUp[4], DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
			//-z
			DirectX::XMStoreFloat3(&m_vecCameraDirection[5], DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f));
			DirectX::XMStoreFloat3(&m_vecCameraUp[5], DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

			//设置depthstencil，防止父类bindingPass BindAll()方法报错
			//SetDepthBuffer(m_pDepthCube->GetDepthBuffer(0));
			//m_pDepthTexBuffer = make_shared<Bind::OutputOnlyDepthStencil>(gfx, gfx.GetWidth(), gfx.GetHeight(), gfx.Get4xMsaaStat(), gfx.GetMsaaQuality());
			//SetDepthBuffer(m_pDepthTexBuffer);
			//SetDepthBuffer(m_pDepthStencil->m_pDepthTexBuffer);
		}

		void Execute(Graphic & gfx)
		{
			using namespace DirectX;
			XMVECTOR cameraPos = XMLoadFloat4(&m_pShadowCamera->GetWorldPos());

			gfx.SetProjection(XMLoadFloat4x4(&m_projection));
			//3维空间有6个方向，在6个方向生成深度图？
			for (size_t i = 0; i < 6; ++i)
			{
				shared_ptr<Bind::OutputOnlyRenderTarget> rt = m_pRenderCube->GetRenderTarget(i);
				rt->Clear(gfx);
				m_depthStencil->Clear(gfx);
				//move只是使智能指针引用计数-1
				SetRenderTarget(move(rt));
				XMVECTOR lookAt = cameraPos + XMLoadFloat3(&m_vecCameraDirection[i]);
				//获取观察矩阵
				gfx.SetCamera(XMMatrixLookAtLH(cameraPos, lookAt, XMLoadFloat3(&m_vecCameraUp[i])));
				RenderQueuePass::Execute(gfx);
			}

			//m_depthStencil->Clear(gfx);
			////设置阴影相机的viewMatrix
			//m_pShadowCamera->BindToGraphic(gfx);
			////重设阴影相机投影矩阵
			//gfx.SetProjection(XMLoadFloat4x4(&m_projection));
			//RenderQueuePass::Execute(gfx);
		}

		void BindShadowCamera(Camera & cam)
		{
			m_pShadowCamera = &cam;
		}

		void DumpShadowMap(Graphic & gfx, const string & path)
		{
			//m_depthStencil->ToSurface(gfx).Save(path);
			for (int i = 0; i < 6; ++i)
			{
				shared_ptr<Bind::OutputOnlyRenderTarget> d = m_pRenderCube->GetRenderTarget(i);
				d->ToSurface(gfx).Save(path + to_string(i) + ".png");
			}
		}
	private:
		void SetDepthBuffer(shared_ptr<Bind::DepthStencil> ds)
		{
			const_cast<ShadowMappingPass*>(this)->m_depthStencil = move(ds);
		}

		void SetRenderTarget(shared_ptr<Bind::RenderTarget> rt)
		{
			const_cast<ShadowMappingPass*>(this)->m_renderTarget = move(rt);
		}

	private:
		UINT m_size = 1000;
		shared_ptr<Bind::CubeTargetTexture> m_pRenderCube;
		//shared_ptr<Bind::OutputOnlyDepthStencil> m_pDepthTexBuffer;
		//用于绑定hlsl中的深度图
		//shared_ptr<Bind::ShaderInputDepthStencil> m_pDepthStencil;
		DirectX::XMFLOAT4X4 m_projection;
		vector<DirectX::XMFLOAT3> m_vecCameraDirection{ 6 };
		vector<DirectX::XMFLOAT3> m_vecCameraUp{ 6 };
		Camera * m_pShadowCamera = nullptr;
	};
}


#endif // !__SHADOWMAPPINGPASS_H__
