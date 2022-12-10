#include "SkyboxCase.h"

namespace DrawTest
{
	void SkyboxCase::Init(Graphic & gfx)
	{
		if (!m_userSphere)
		{
			m_pCube = make_shared<Cuboid>(gfx, 1000.0f, GeometryType::Solid);
			m_pVtxShader = VertexShader::Resolve(gfx, "Skybox_VS.cso");
			m_pInputLayout = InputLayout::Resolve(gfx, m_pCube->m_vtxBuffer->GetLayout(), *m_pVtxShader);
		}
		else
		{
			m_pSphere = make_shared<Sphere>(gfx, 100.0f, GeometryType::Solid);
			m_pVtxShader = VertexShader::Resolve(gfx, "Skybox_VS.cso");
			m_pInputLayout = InputLayout::Resolve(gfx, m_pSphere->m_vtxBuffer->GetLayout(), *m_pVtxShader);
		}
		m_pPxShader = PixelShader::Resolve(gfx, "Skybox_PS.cso");
		m_pTransformCbuf = make_shared<SkyboxTransformCbuf>(gfx);
		m_pCubeTexture = make_shared<CubeTexture>(gfx, "images\\spacebox");
		m_pSampler =  Sampler::Resolve(gfx, Sampler::Type::Bilinear);
		//������պ�ʼ�մ�������Զ���������е����嶼��������պ��ཻ���������Ⱦʱ����Ҫ��ֹ��Ȳ����Լ��������д��
		m_pStencilDepthFirst = Stencil::Resolve(gfx, Stencil::Mode::DepthFirst);
		m_pStencilOff = Stencil::Resolve(gfx, Stencil::Mode::Off);
		m_pRasterizer = Rasterizer::Resolve(gfx, true);
	}


	void SkyboxCase::Draw(Graphic & gfx, float dt)
	{
		m_pVtxShader->Bind(gfx);
		m_pPxShader->Bind(gfx);
		m_pInputLayout->Bind(gfx);
		m_pTransformCbuf->Bind(gfx);
		m_pCubeTexture->Bind(gfx);
		m_pSampler->Bind(gfx);
		//��Ⱦ��պ�ʱ����Ҫ��ֹ��Ȳ����Լ��������д��
		m_pStencilDepthFirst->Bind(gfx);
		m_pRasterizer->Bind(gfx);
		if (!m_userSphere)
		{
			m_pCube->Bind(gfx);
			gfx.DrawIndex(m_pCube->GetIndexCount());
		}
		else
		{
			m_pSphere->Bind(gfx);
			gfx.DrawIndex(m_pSphere->GetIndexCount());
		}
		//��Ⱦ��պн�����Ϊ����Ⱦ�������壬Ҫ�رս�ֹ��Ȳ����Լ��������д�������
		m_pStencilOff->Bind(gfx);
	}
}