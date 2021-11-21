#ifndef __BLENDER_H__
#define __BLENDER_H__

#pragma once

#include "Bindable.h"
#include <array>

using namespace std;

#define BLENDFACROR_DEFAULT -1.0f

namespace Bind
{
	class Blender : public Bindable
	{
	public:
		Blender(Graphic & gfx, bool blending, float factor = BLENDFACROR_DEFAULT);
		void Bind(Graphic & gfx) override;
		void SetFactor(float factor);
		float GetFactor();
	protected:
		bool m_blending;
		Microsoft::WRL::ComPtr<ID3D11BlendState> m_blenderStat;
		array<float, 4> m_factors;
	};
}


#endif // !__BLENDER_H__
