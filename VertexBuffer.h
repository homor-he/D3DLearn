#ifndef __VERTEXBUFFER_H__
#define __VERTEXBUFFER_H__

#pragma once

#include "Bindable.h"
#include "Vertex.h"

namespace Bind
{
	class VertexBuffer : public  Bindable
	{
	public:
		VertexBuffer(Graphic & gfx, VertexRela::VertexBuffer & vBuf);
		void Bind(Graphic& gfx) override;
		VertexRela::VertexLayout & GetLayout();
	protected:
		UINT m_stride;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
		VertexRela::VertexLayout m_layout;
	};
}

#endif
