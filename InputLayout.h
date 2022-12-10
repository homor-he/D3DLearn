#ifndef __INPUTLAYOUT_H__
#define __INPUTLAYOUT_H__

#pragma once

#include "Bindable.h"
#include "Vertex.h"
#include "VertexShader.h"

namespace Bind
{
	class InputLayout : public Bindable
	{
	public:
		InputLayout(Graphic & gfx, VertexRela::VertexLayout layout, VertexShader & vertexShader);
		void Bind(Graphic& gfx) override;
		static shared_ptr<InputLayout> Resolve(Graphic & gfx, VertexRela::VertexLayout & layout, 
			VertexShader & vertexShader);
		static string GenerateUID(VertexRela::VertexLayout layout, VertexShader & vertexShader);
	protected:
		VertexRela::VertexLayout m_layout;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	};
}

#endif // !__INPUTLAYOUT_H__
