#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

#pragma once

#include "Bindable.h"

namespace Bind
{
	class Viewport : public Bindable
	{
	public:
		Viewport(Graphic & gfx);
		Viewport(Graphic & gfx, float width, float height);
		void Bind(Graphic & gfx) override;
	private:
		D3D11_VIEWPORT m_viewport;
	};
}

#endif // !__VIEWPORT_H__
