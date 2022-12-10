#include "Rasterizer.h"
#include "GraphicsThrowMacros.h"
#include "BindsDic.h"

namespace Bind
{
	Rasterizer::Rasterizer(Graphic & gfx, bool twoSized, D3D11_FILL_MODE mode):
		m_twoSided(twoSized)
	{
		INFOMAN(gfx);
		D3D11_RASTERIZER_DESC rasterizerDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
		rasterizerDesc.CullMode = twoSized ? D3D11_CULL_NONE : D3D11_CULL_BACK;
		rasterizerDesc.FillMode = mode;
		GFX_THROW_INFO(GetDevice(gfx)->CreateRasterizerState(&rasterizerDesc, &m_rasterizerStat));
	}

	void Rasterizer::Bind(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->RSSetState(m_rasterizerStat.Get()));
	}

	shared_ptr<Rasterizer> Rasterizer::Resolve(Graphic & gfx, bool twoSized, D3D11_FILL_MODE mode)
	{
		return BindsDic::Resolve<Rasterizer>(gfx, twoSized, mode);
	}

	string Rasterizer::GenerateUID(bool twoSized, D3D11_FILL_MODE mode)
	{
		return typeid(Rasterizer).name() + "#"s + (twoSized ? "2s" : "1s");
	}
}