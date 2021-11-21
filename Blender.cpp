#include "Blender.h"
#include "GraphicsThrowMacros.h"

namespace Bind
{
	Blender::Blender(Graphic & gfx, bool blending, float factor):
		m_blending(blending)
	{
		INFOMAN(gfx);
		if (factor >= 0.0f)
		{
			m_factors.fill(factor);
		}

		D3D11_BLEND_DESC blendDesc = CD3D11_BLEND_DESC{ CD3D11_DEFAULT{} };
		if (blending)
		{
			blendDesc.RenderTarget[0].BlendEnable = true;
			if (factor >= 0.0f)
			{
				blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_BLEND_FACTOR;
				blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_BLEND_FACTOR;
			}
			else
			{
				blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
				blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			}
		}
		GFX_THROW_INFO(GetDevice(gfx)->CreateBlendState(&blendDesc, &m_blenderStat));
	}

	void Blender::Bind(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		const float * blendFactor = m_factors.size() > 0 ? m_factors.data() : nullptr;
		GFX_THROW_INFO_ONLY(GetContext(gfx)->OMSetBlendState(m_blenderStat.Get(), blendFactor, 0xffffffff));
	}

	void Blender::SetFactor(float factor)
	{
		if (factor > 0.0f)
			m_factors.fill(factor);
	}

	float Blender::GetFactor()
	{
		return m_factors.front();
	}
}


