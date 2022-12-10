#ifndef __FIRSTCASE__H__
#define __FIRSTCASE__H__

#pragma once

#include "Vertex.h"
#include "BindableCommon.h"

namespace DrawTest
{
	// µœ÷2dªÊ÷∆
	class FirstTest
	{
		struct Vertex
		{
			DirectX::XMFLOAT2 pos;
			DirectX::XMFLOAT3 color;
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

		void SetVertexShaderAndInputLayout(Graphic & gfx, string fileName = "VertexShader.cso");
		void SetPixelShader(Graphic & gfx, string fileName = "PixelShader.cso");
		size_t GetIndexArrSize();
	private:
		vector<Vertex> m_vertexArr;
		vector<unsigned short> m_indexArr;
		VertexRela::VertexLayout m_vtLayout;
	};
	
}



#endif // !__FIRSTCASE__H__
