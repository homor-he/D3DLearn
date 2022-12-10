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
		VertexBuffer(Graphic & gfx, string tag, VertexRela::VertexBuffer & vBuf, bool cpuAccess = false);
		void Bind(Graphic& gfx) override;
		VertexRela::VertexLayout & GetLayout();
		static shared_ptr<VertexBuffer> Resolve(Graphic & gfx, string &tag, VertexRela::VertexBuffer & vBuf, bool cpuAccess = false);
		template<typename ... Ignore>
		static string GenerateUID(string & tag, Ignore && ... p)
		{
			return GenerateUID_(tag);
		}
		Microsoft::WRL::ComPtr<ID3D11Buffer> GetD3DVtxBuffer();
	private:
		static string GenerateUID_(string & tag);
	protected:
		string m_tag;
		UINT m_stride;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
		VertexRela::VertexLayout m_layout;
	};
}

#endif
