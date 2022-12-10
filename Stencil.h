#ifndef __STENCIL_H__
#define __STENCIL_H__

#pragma once

#include "Bindable.h"

namespace Bind
{
	class Stencil : public Bindable
	{
	public:
		enum class Mode
		{
			Off,
			Write,
			Mask,
			DepthOff,
			DepthReversed,
			DepthFirst,
		};

		Stencil(Graphic & gfx, Mode mode);
		void Bind(Graphic & gfx) override;
		static shared_ptr<Stencil> Resolve(Graphic & gfx, Mode mode);
		static string GenerateUID(Mode mode);
	private:
		Mode m_mode;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pStencil;
	};
}

#endif // !__STENCIL_H__
