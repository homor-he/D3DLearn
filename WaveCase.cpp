#include "WaveCase.h"
#include "TechChannels.h"
#include "MathHelper.h"
#include "GraphicResource.h"

void DrawTest::WaveCase::Init(Graphic & gfx)
{
	m_waveRow = 200;
	m_waveCol = 200;
	m_wave = make_shared<Waves>(gfx, m_waveRow, m_waveCol, 0.8f, 0.03f, 3.25f, 0.4f);
	m_wave->SetPos(DirectX::XMFLOAT3(-100.0f, 0.0f, 0.0f));

	{
		Technique shade("shade", Chan::main);
		{
			//和pass绑定到一起
			Step only("lambertian");
			auto vtxShader = Bind::VertexShader::Resolve(gfx, "Wave_VS.cso");
			only.AddBindable(vtxShader);
			only.AddBindable(Bind::PixelShader::Resolve(gfx, "Wave_PS.cso"));
			only.AddBindable(Bind::InputLayout::Resolve(gfx, m_wave->m_vtxBuffer->GetLayout(), *vtxShader));

			//only.AddBindable(Bind::Texture::Resolve(gfx, "images\\brickwall.jpg"));
			//only.AddBindable(Bind::Sampler::Resolve(gfx));

			only.AddBindable(make_shared<Bind::TransformCbuf>(gfx));
			only.AddBindable(Bind::Rasterizer::Resolve(gfx, false,D3D11_FILL_MODE::D3D11_FILL_SOLID));
			shade.AddStep(move(only));
		}
		m_wave->AddTechnique(move(shade));
	}


	{
		Technique map("ShadowMap", Chan::shadow, true);
		{
			Step draw("shadowMap");

			draw.AddBindable(Bind::InputLayout::Resolve(gfx, m_wave->m_vtxBuffer->GetLayout(), *Bind::VertexShader::Resolve(gfx, "SolidVS.cso")));
			draw.AddBindable(make_shared<Bind::TransformCbuf>(gfx));

			map.AddStep(draw);
		}
		m_wave->AddTechnique(move(map));
	}
}

void DrawTest::WaveCase::LinkTechnique(Rgph::RenderGraph & rg)
{
	m_wave->LinkTechnique(rg);
}

void DrawTest::WaveCase::Submit(size_t channelFilter)
{
	m_wave->Submit(channelFilter);
}

void DrawTest::WaveCase::UpdateWave(Graphic & gfx, float totalTime, float dt)
{
	static float t_base = 0.0f;
	if ((totalTime - t_base) >= 0.25f)
	{
		t_base += 0.25f;

		DWORD i = 5 + rand() % (m_waveRow - 10);
		DWORD j = 5 + rand() % (m_waveCol - 10);

		float r = MathHelper::RandF(1.0f, 2.0f);

		//给与wave 5个顶点扰动
		m_wave->Disturb(i, j, r);
	}
	//随dt更新所有当前顶点y轴坐标
	m_wave->Update(dt);

	//更新vertexbuffer中的坐标
	D3D11_MAPPED_SUBRESOURCE msr = {0};
	GraphicResource::GetContext(gfx)->Map(m_wave->GetD3DVtxBuffer().Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
	WaveVertex * v = reinterpret_cast<WaveVertex*>(msr.pData);
	for (UINT i = 0; i < m_wave->GetVertextCnt(); ++i)
	{
		v[i].pos = m_wave->GetCurrVertexIndexPos(i);
		v[i].color = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	}
	GraphicResource::GetContext(gfx)->Unmap(m_wave->GetD3DVtxBuffer().Get(), 0u);
}
