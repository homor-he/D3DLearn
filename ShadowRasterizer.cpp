#include "ShadowRasterizer.h"
#include "GraphicsThrowMacros.h"

namespace Bind
{
	ShadowRasterizer::ShadowRasterizer(Graphic & gfx, int depthBias, float slopeBias, float clamp)
	{
		ChangeDepthBiasParameters(gfx, depthBias, slopeBias, clamp);
	}

	void ShadowRasterizer::Bind(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->RSSetState(m_pRasterizer.Get()));
	}

	void ShadowRasterizer::ChangeDepthBiasParameters(Graphic & gfx, int depthBias, float slopeBias, float clamp)
	{
		this->m_depthBias = depthBias;
		this->m_slopeBias = slopeBias;
		this->m_clamp = clamp;

		D3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());
		rasterDesc.DepthBias = depthBias;
		rasterDesc.SlopeScaledDepthBias = slopeBias;
		rasterDesc.DepthBiasClamp = clamp;

		INFOMAN(gfx);
		GFX_THROW_INFO(GetDevice(gfx)->CreateRasterizerState(&rasterDesc, &m_pRasterizer));
	}
}