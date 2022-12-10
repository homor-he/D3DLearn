#ifndef __SKYBOXPASS_H__
#define __SKYBOXPASS_H__

#pragma once

#include "RenderQueuePass.h"
#include "Cuboid.h"
#include "Sphere.h"
#include "CubeTexture.h"
#include "SkyboxTransformCbuf.h"

class Graphic;

namespace Rgph
{
	class SkyboxPass : public BindingPass
	{
	public:
		SkyboxPass(Graphic & gfx, string name) : BindingPass(move(name))
		{
			//���ڻ�����״��drawble������step,���Բ��ü̳�renderQueuePass,ֱ�Ӽ̳�bindingPass,����дExecute����
			using namespace Bind;
			RegisterSink(DirectBufferSink<RenderTarget>::Make("renderTarget", m_renderTarget));
			RegisterSink(DirectBufferSink<DepthStencil>::Make("depthStencil", m_depthStencil));
			RegisterSource(DirectBufferSource<RenderTarget>::Make("renderTarget", m_renderTarget));
			RegisterSource(DirectBufferSource<DepthStencil>::Make("depthStencil", m_depthStencil));

			AddBind(PixelShader::Resolve(gfx, "Skybox_PS.cso"));
			AddBind(make_shared<CubeTexture>(gfx, "images\\spacebox"));
			AddBind(Sampler::Resolve(gfx, Sampler::Type::Bilinear));
			AddBind(make_shared<SkyboxTransformCbuf>(gfx));
			//������պ�ʼ�մ�������Զ��(����������ɫ��z/w=1)����Զ���Ϊ1,������ȱȽϺ�������ΪLESS_EQUAL������պ���˳��ͨ����Ȳ���
			AddBind(Stencil::Resolve(gfx, Stencil::Mode::DepthFirst));
			AddBind(Rasterizer::Resolve(gfx, true));

			{
				auto vtxShader = VertexShader::Resolve(gfx, "Skybox_VS.cso");
				//������
				{
					m_pCube = make_shared<Cuboid>(gfx, 1000.0f, GeometryType::Solid, "$cube_map");
					AddBind(InputLayout::Resolve(gfx, m_pCube->m_vtxBuffer->GetLayout(), *vtxShader));
				}
				//����
				{
					m_pSphere = make_shared<Sphere>(gfx, 100.0f, GeometryType::Solid, "$sphere_map");
				}
				AddBind(move(vtxShader));
			}
			m_stencilOff = Stencil::Resolve(gfx, Stencil::Mode::Off);
		}

		void Execute(Graphic & gfx) override
		{
			DWORD indexCnt = 0;
			if (m_useCube)
			{
				m_pCube->Bind(gfx);
				indexCnt = m_pCube->GetIndexCount();
			}
			else
			{
				m_pSphere->Bind(gfx);
				indexCnt = m_pSphere->GetIndexCount();
			}
			//shader/inputlayout/texture�����õ�deviceContext
			BindAll(gfx);
			gfx.DrawIndex(indexCnt);

			//m_pVtxShader->Bind(gfx);
			//m_pPxShader->Bind(gfx);
			//m_pInputLayout->Bind(gfx);
			//m_pTransformCbuf->Bind(gfx);
			//m_pCubeTexture->Bind(gfx);
			//m_pSampler->Bind(gfx);
			////��Ⱦ��պ�ʱ����Ҫ��ֹ��Ȳ����Լ��������д��
			//m_pStencilDepthFirst->Bind(gfx);
			//m_pRasterizer->Bind(gfx);
			//if (!m_userSphere)
			//{
			//	m_pCube->Bind(gfx);
			//	gfx.DrawIndex(m_pCube->GetIndexCount());
			//}
			//else
			//{
			//	m_pSphere->Bind(gfx);
			//	gfx.DrawIndex(m_pSphere->GetIndexCount());
			//}
			////��Ⱦ��պн�����Ϊ����Ⱦ�������壬Ҫ�رս�ֹ��Ȳ����Լ��������д�������
			//m_pStencilOff->Bind(gfx);
			m_stencilOff->Bind(gfx);
		}
	private:
		shared_ptr<Cuboid> m_pCube;
		shared_ptr<Sphere> m_pSphere;
		bool m_useCube = true;
		shared_ptr<Bind::Stencil> m_stencilOff;
	};
}


#endif // !__SKYBOXPASS_H__
