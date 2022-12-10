#ifndef __RASTERIZER_H__
#define __RASTERIZER_H__

#pragma once

#include "Bindable.h"

namespace Bind
{
	class Rasterizer : public Bindable
	{
	public:
		Rasterizer(Graphic & gfx, bool twoSized, D3D11_FILL_MODE mode = D3D11_FILL_SOLID);
		void Bind(Graphic & gfx) override;
		static shared_ptr<Rasterizer> Resolve(Graphic & gfx, bool twoSized, D3D11_FILL_MODE mode = D3D11_FILL_SOLID);
		static string GenerateUID(bool twoSized, D3D11_FILL_MODE mode = D3D11_FILL_SOLID);
	protected:
		bool m_twoSided;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerStat;
	};
}

#endif // !__RASTERIZER_H__
