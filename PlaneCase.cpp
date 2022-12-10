#include "PlaneCase.h"
#include "TechChannels.h"
#include "DynamicConstant.h"
#include "ConstantBufferEx.h"

namespace DrawTest
{
	/*void PlaneCase::Init(Graphic & gfx)
	{
		m_objCBuf = {
			{ 1.0f,1.0f,1.0f },
			0.05f,0.1f
		};

		float height = 0.2f;
		m_plane = make_shared<Cuboid>(gfx, 20.0f, height, 20.0f, GeometryType::Common);
		m_plane->SetPos(DirectX::XMFLOAT3(0.0f, -height / 2, 0.0f));
		m_transformBuf = make_shared<Bind::TransformCbuf>(gfx);
		m_transformBuf->InitializeParentRef(*m_plane);

		m_vtxShader =  Bind::VertexShader::Resolve(gfx, "PhongDif_VS.cso");
		m_pxShader =  Bind::PixelShader::Resolve(gfx, "PhongDif_PS.cso");
		m_inputLayout = Bind::InputLayout::Resolve(gfx, m_plane->m_vtxBuffer->GetLayout(), *m_vtxShader);

		m_tex = Bind::Texture::Resolve(gfx, "images\\brickwall.jpg");
		m_sampler = Bind::Sampler::Resolve(gfx, Bind::Sampler::Type::Anisotropic);
		m_pixelBuffer = Bind::PixelConstantBuffer<ObjectCBuf>::Resolve(gfx, m_objCBuf, 1);
	}

	void PlaneCase::Draw(Graphic & gfx, float dt)
	{
		m_plane->Bind(gfx);
		m_transformBuf->Bind(gfx);
		m_vtxShader->Bind(gfx);
		m_pxShader->Bind(gfx);
		m_inputLayout->Bind(gfx);
		m_tex->Bind(gfx);
		m_sampler->Bind(gfx);
		m_pixelBuffer->Bind(gfx);
		gfx.DrawIndex(m_plane->GetIndexCount());
	}*/

	void PlaneCase::Init(Graphic & gfx)
	{
		float height = 0.2f;
		m_plane = make_shared<Cuboid>(gfx, 80.0f, height, 80.0f, GeometryType::Common);
		m_plane->SetPos(DirectX::XMFLOAT3(0.0f, -height / 2, 0.0f));

		{
			Technique shade("shade", Chan::main);
			{
				Step only("lambertian");
				auto vtxShader = Bind::VertexShader::Resolve(gfx, "ShadowTest_VS.cso");
				only.AddBindable(vtxShader);
				only.AddBindable(Bind::PixelShader::Resolve(gfx, "ShadowTest_PS.cso"));
				only.AddBindable(Bind::InputLayout::Resolve(gfx, m_plane->m_vtxBuffer->GetLayout(), *vtxShader));

				only.AddBindable(Bind::Texture::Resolve(gfx, "images\\brickwall.jpg"));
				only.AddBindable(Bind::Sampler::Resolve(gfx));

				only.AddBindable(make_shared<Bind::TransformCbuf>(gfx));

				Dcb::RawLayout lay;
				lay.Add<Dcb::Float3>("specularColor");
				lay.Add<Dcb::Float>("specularWeight");
				lay.Add<Dcb::Float>("specularGloss");
				Dcb::Buffer buf(move(lay));
				buf["specularColor"] = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
				buf["specularWeight"] = 0.05f;
				buf["specularGloss"] = 0.1f;
				only.AddBindable(make_shared<Bind::CachingPixelConstantBufferEx>(gfx, buf, 1u));

				only.AddBindable(Bind::Rasterizer::Resolve(gfx, false));
				shade.AddStep(move(only));
			}
			m_plane->AddTechnique(move(shade));
		}


		{
			Technique map("ShadowMap", Chan::shadow, true);
			{
				Step draw("shadowMap");

				draw.AddBindable(Bind::InputLayout::Resolve(gfx, m_plane->m_vtxBuffer->GetLayout(), *Bind::VertexShader::Resolve(gfx, "SolidVS.cso")));
				draw.AddBindable(make_shared<Bind::TransformCbuf>(gfx));

				map.AddStep(draw);
			}
			m_plane->AddTechnique(move(map));
		}
	}

	void PlaneCase::LinkTechnique(Rgph::RenderGraph & rg)
	{
		m_plane->LinkTechnique(rg);
	}

	void PlaneCase::Submit(size_t channelFilter)
	{
		m_plane->Submit(channelFilter);
	}
}