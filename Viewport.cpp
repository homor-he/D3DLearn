#include "Viewport.h"
#include "GraphicsThrowMacros.h"

namespace Bind
{
	Viewport::Viewport(Graphic & gfx) :
		Viewport(gfx, (float)gfx.GetWidth(), (float)gfx.GetHeight())
	{
	}

	Viewport::Viewport(Graphic & gfx, float width, float height)
	{
		//设置视口参数
		m_viewport.Width = width;
		m_viewport.Height = height;
		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
	}

	void Viewport::Bind(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->RSSetViewports(1, &m_viewport));
	}
}



