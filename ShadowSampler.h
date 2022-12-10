#ifndef __SHADOWSAMPLER_H__
#define __SHADOWSAMPLER_H__

#pragma once

#include "Bindable.h"

namespace Bind
{
	class ShadowSampler : public Bindable
	{
	public:
		ShadowSampler(Graphic & gfx);
		void Bind(Graphic & gfx) override;
		bool GetHwPcf();
		bool GetBilinear();
		void SetBilinear(bool bilin);
		void SetHwPcf(bool hwPcf);
	private:
		UINT GetCurrentSlot();
		static Microsoft::WRL::ComPtr<ID3D11SamplerState> MakeSampler(Graphic & gfx, bool bilin, bool hwPcf);
	protected:
		size_t m_curSampler;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplers[4];
	};
}

#endif // !__SHADOWSAMPLER_H__


