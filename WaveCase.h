#ifndef __WAVECASE_H__
#define __WAVECASE_H__

#pragma once

#include "CaseBase.h"
#include "Waves.h"

namespace DrawTest
{
	using namespace Bind;
	class WaveCase : public CaseBase
	{
		struct WaveVertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT4 color;
		};
	public:
		void Init(Graphic & gfx) override;
		virtual void Draw(Graphic & gfx, float dt) {}
		void LinkTechnique(Rgph::RenderGraph & rg);
		void Submit(size_t channelFilter);
	public:
		void UpdateWave(Graphic & gfx, float totalTime, float dt);
	public:
		shared_ptr<Waves> m_wave;
		UINT m_waveRow;
		UINT m_waveCol;
	};

}

#endif // !__WAVECASE_H__
