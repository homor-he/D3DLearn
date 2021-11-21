#include "Rasterizer.h"
#include "GraphicsThrowMacros.h"

namespace Bind
{
	Rasterizer::Rasterizer(Graphic & gfx, bool twoSized):
		m_twoSided(twoSized)
	{
		INFOMAN(gfx);
		D3D11_RASTERIZER_DESC rasterizerDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
		rasterizerDesc.CullMode = twoSized ? D3D11_CULL_NONE : D3D11_CULL_BACK;
		GFX_THROW_INFO(GetDevice(gfx)->CreateRasterizerState(&rasterizerDesc, &m_rasterizerStat));
	}

	void Rasterizer::Bind(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->RSSetState(m_rasterizerStat.Get()));
	}
}