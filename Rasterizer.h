#ifndef __RASTERIZER_H__
#define __RASTERIZER_H__

#pragma once

#include "Bindable.h"

namespace Bind
{
	class Rasterizer : public Bindable
	{
	public:
		Rasterizer(Graphic & gfx, bool twoSized);
		void Bind(Graphic & gfx) override;
	protected:
		bool m_twoSided;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerStat;
	};
}

#endif // !__RASTERIZER_H__
