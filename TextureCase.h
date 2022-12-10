#ifndef __TEXTURECASE_H__
#define __TEXTURECASE_H__

#pragma once

#include "Vertex.h"
#include "BindableCommon.h"

namespace DrawTest
{
	//≤‚ ‘∆Ω√ÊŒ∆¿Ì
	class TextureCase
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT2 tex;
		};

	public:
		void Init(Graphic & gfx);
		void SetVertexBuffer(Graphic & gfx, vector<Vertex> & vecArr);
		void SetIndexBuffer(Graphic & gfx, vector<unsigned short> & vecArr);

		template<typename T>
		void SetConstantBuffer(Graphic & gfx, T & val)
		{
			Bind::VertexConstantBuffer<T> vtConstantBuffer(gfx, val);
			vtConstantBuffer.Bind(gfx);
		}

		void SetVertexShaderAndInputLayout(Graphic & gfx, string fileName = "TextureVS.cso");
		void SetPixelShader(Graphic & gfx, string fileName = "TexturePS.cso");
		size_t GetIndexArrSize();

	private:
		vector<Vertex> m_vertexArr;
		vector<unsigned short> m_indexArr;
		VertexRela::VertexLayout m_vtLayout;
	};
}

#endif // !__TEXTURECASE_H__
