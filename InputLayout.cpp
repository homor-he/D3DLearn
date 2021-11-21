#include "InputLayout.h"
#include "GraphicsThrowMacros.h"
//#include <type_traits>

namespace Bind
{
	InputLayout::InputLayout(Graphic & gfx, VertexRela::VertexLayout layout, VertexShader & vertexShader) :
		m_layout(move(layout))

	{
		INFOMAN(gfx);
		vector<D3D11_INPUT_ELEMENT_DESC> sDesc = m_layout.GetLayoutDesc();
		ID3DBlob* pBlob = vertexShader.GetByteCode();
		GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(sDesc.data(),sDesc.size(),
			pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_inputLayout));
	}

	void InputLayout::Bind(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->IASetInputLayout(m_inputLayout.Get()));
	}
}


