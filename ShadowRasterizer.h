#ifndef __SHADOWRASTERIZER_H__
#define __SHADOWRASTERIZER_H__

#pragma once

#include "Bindable.h"

namespace Bind
{
	class ShadowRasterizer : public Bindable
	{
	public:
		ShadowRasterizer(Graphic & gfx, int depthBias, float slopeBias, float clamp);
		void Bind(Graphic & gfx) override;
		void ChangeDepthBiasParameters(Graphic & gfx, int depthBias, float slopeBias, float clamp);
	protected:
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pRasterizer;
		int m_depthBias;
		float m_slopeBias;
		float m_clamp;
	};
}



#endif // !__SHADOWRASTERIZER_H__
