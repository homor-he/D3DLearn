#include "GeometryCaseWithTech.h"
#include "BindableCommon.h"
//#include "RenderQueuePass.h"
#include "TechChannels.h"
#include "DynamicConstant.h"
#include "ConstantBufferEx.h"

#include "Sink.h"
#include "Source.h"
#include "Pass.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "TestPass.h"


namespace DrawTest
{
	//TestRenderGraph
	//void GeometryCaseWithTech::Init(Graphic & gfx)
	//{
	//	m_cube = make_shared<Cuboid>(gfx, 1.0f, GeometryType::Common);
	//	m_cube->SetPos(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	//	//m_cube->SetRotation(0.0f, 1.0f, 0.0f);
	//	//m_cube->SetRotSpeedAndStat(0.02f, RotaStat::Rotation);

	//	m_cube2 = make_shared<Cuboid>(gfx, 1.0f, GeometryType::Common);
	//	m_cube2->SetPos(DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f));
	//	{
	//		Technique common("Common", Chan::main);
	//		{
	//			Step only("test");
	//			shared_ptr<Bind::VertexShader> vtxShader = Bind::VertexShader::Resolve(gfx, "GeometryVS.cso");
	//			only.AddBindable(vtxShader);
	//			only.AddBindable(Bind::PixelShader::Resolve(gfx, "GeometryPS.cso"));
	//			only.AddBindable(Bind::InputLayout::Resolve(gfx, m_cube->m_vtxBuffer->GetLayout(), *vtxShader));
	//			only.AddBindable(Bind::Texture::Resolve(gfx, "images\\WoodCrate02.dds"));
	//			only.AddBindable(Bind::Sampler::Resolve(gfx));
	//			only.AddBindable(Bind::Rasterizer::Resolve(gfx, false));
	//			//only.AddBindable(Bind::Blender::Resolve(gfx, false));
	//			only.AddBindable(make_shared<Bind::TransformCbuf>(gfx));
	//			common.AddStep(move(only));
	//		}
	//		m_cube->AddTechnique(move(common));
	//	}

	//	{
	//		Technique outline("Outline", Chan::main, true);
	//		{
	//			Step mask("outlineMask");

	//			mask.AddBindable(Bind::InputLayout::Resolve(gfx, m_cube->m_vtxBuffer->GetLayout(), *Bind::VertexShader::Resolve(gfx, "SolidVS.cso")));
	//			mask.AddBindable(make_shared<Bind::TransformCbuf>(gfx));

	//			outline.AddStep(move(mask));
	//		}
	//		{
	//			Step draw("outlineDraw");
	//			Dcb::RawLayout colorLayout;
	//			colorLayout.Add<Dcb::Float3>("matColor");
	//			//Dcb::Buffer buf(move(pixelLayout));
	//			Dcb::Buffer colorBuf(move(colorLayout));
	//			colorBuf["matColor"] = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);

	//			draw.AddBindable(make_shared<Bind::CachingPixelConstantBufferEx>(gfx, colorBuf, 1u));
	//			draw.AddBindable(Bind::InputLayout::Resolve(gfx, m_cube->m_vtxBuffer->GetLayout(), *Bind::VertexShader::Resolve(gfx, "SolidVS.cso")));
	//			draw.AddBindable(make_shared<Bind::TransformCbuf>(gfx));

	//			outline.AddStep(move(draw));
	//		}
	//		m_cube->AddTechnique(move(outline));
	//	}

	//	{
	//		Technique common("Common", Chan::main);
	//		{
	//			Step only("test");
	//			shared_ptr<Bind::VertexShader> vtxShader = Bind::VertexShader::Resolve(gfx, "GeometryVS.cso");
	//			only.AddBindable(vtxShader);
	//			only.AddBindable(Bind::PixelShader::Resolve(gfx, "GeometryPS.cso"));
	//			only.AddBindable(Bind::InputLayout::Resolve(gfx, m_cube2->m_vtxBuffer->GetLayout(), *vtxShader));
	//			only.AddBindable(Bind::Texture::Resolve(gfx, "images\\WoodCrate02.dds"));
	//			only.AddBindable(Bind::Sampler::Resolve(gfx));
	//			only.AddBindable(Bind::Rasterizer::Resolve(gfx, false));
	//			//only.AddBindable(Bind::Blender::Resolve(gfx, false));
	//			only.AddBindable(make_shared<Bind::TransformCbuf>(gfx));
	//			common.AddStep(move(only));
	//		}
	//		m_cube2->AddTechnique(move(common));
	//	}
	//	
	//	//将相同名字的step和pass绑定
	//	m_cube->LinkTechnique(*gfx.GetRenderGraph());
	//	m_cube2->LinkTechnique(*gfx.GetRenderGraph());
	//	//给特定channel的technique下的所有step对应的pass添加渲染任务
	//	m_cube->Submit(Chan::main);
	//	m_cube2->Submit(Chan::main);
	//}


	void GeometryCaseWithTech::Init(Graphic & gfx)
	{
		m_cube = make_shared<Cuboid>(gfx, 2.0f, GeometryType::Common);
		m_cube->SetPos(DirectX::XMFLOAT3(0.0f, 5.0f, 0.0f));

		{
			Technique shade("shade", Chan::main);
			{
				Step only("lambertian");

				shared_ptr<Bind::VertexShader> vtxShader = Bind::VertexShader::Resolve(gfx, "ShadowTest_VS.cso");
				only.AddBindable(vtxShader);
				only.AddBindable(Bind::PixelShader::Resolve(gfx, "ShadowTest_PS.cso"));
				only.AddBindable(Bind::InputLayout::Resolve(gfx, m_cube->m_vtxBuffer->GetLayout(), *vtxShader));

				only.AddBindable(Bind::Texture::Resolve(gfx, "images\\WoodCrate02.dds"));
				only.AddBindable(Bind::Sampler::Resolve(gfx));
		
				Dcb::RawLayout lay;
				lay.Add<Dcb::Float3>("specularColor");
				lay.Add<Dcb::Float>("specularWeight");
				lay.Add<Dcb::Float>("specularGloss");
				auto buf = Dcb::Buffer(move(lay));
				buf["specularColor"] = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
				buf["specularWeight"] = 0.1f;
				buf["specularGloss"] = 20.0f;
				only.AddBindable(make_shared<Bind::CachingPixelConstantBufferEx>(gfx, buf, 1u));

				//cull back
				only.AddBindable(Bind::Rasterizer::Resolve(gfx, false));
				only.AddBindable(make_shared<Bind::TransformCbuf>(gfx));

				shade.AddStep(move(only));
			}
			m_cube->AddTechnique(move(shade));
		}

		{
			Technique map("ShadowMap", Chan::shadow, true);
			{
				Step draw("shadowMap");

				draw.AddBindable(Bind::InputLayout::Resolve(gfx, m_cube->m_vtxBuffer->GetLayout(), *Bind::VertexShader::Resolve(gfx, "Shadow_VS.cso")));
				draw.AddBindable(make_shared<Bind::TransformCbuf>(gfx));

				//draw.AddBindable(Bind::PixelShader::Resolve(gfx, "SolidPS.cso"));

				map.AddStep(draw);
			}
			m_cube->AddTechnique(move(map));
		}
		//将相同名字的step和pass绑定
		//m_cube->LinkTechnique(*gfx.GetRenderGraph());
		//m_cube->Submit(Chan::main);
		//m_cube->Submit(Chan::shadow);
	}

	//void GeometryCaseWithTech::Draw(Graphic & gfx, float dt)
	//{
	//	//遍历所有pass执行pass相关渲染
	//	gfx.GetRenderGraph()->Execute(gfx);
	//}

	void GeometryCaseWithTech::LinkTechnique(Rgph::RenderGraph & rg)
	{
		m_cube->LinkTechnique(rg);
	}

	void GeometryCaseWithTech::Submit(size_t channelFilter)
	{
		m_cube->Submit(channelFilter);
	}
}