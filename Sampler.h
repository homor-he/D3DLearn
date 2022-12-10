#ifndef __SAMPLER_H__
#define __SAMPLER_H__

#pragma once

#include "Bindable.h"

namespace Bind
{
	class Sampler : public Bindable
	{
	public:
		enum class Type
		{
			Anisotropic,
			Bilinear,
			Point,
		};
	public:
		Sampler(Graphic & gfx, Type type, bool reflect, UINT slot);
		static shared_ptr<Sampler> Resolve(Graphic & gfx, Type type = Type::Anisotropic, bool reflect = false, UINT slot = 0u);
		static string GenerateUID(Type type, bool reflect, UINT slot);
		void Bind(Graphic & gfx) override;
	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerStat;
		Type m_type;
		bool m_reflect;
		UINT m_slot;
	};
}



#endif // !__SAMPLER_H__
