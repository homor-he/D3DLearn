#ifndef __GEOMETRYCASE_H__
#define __GEOMETRYCASE_H__

#pragma once

#include "Vertex.h"
#include "BindableCommon.h"
#include "GeometryGenerator.h"

namespace DrawTest
{
	//实现绘制一个3d物体及其纹理
	class GeometryCase
	{
	public:
		struct VertexConstantVar
		{
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX worldInvTranspose;
			DirectX::XMMATRIX worldViewProj;
			DirectX::XMMATRIX texTransform;
		};


	public:
		void Init(Graphic & gfx);
		void SetVertexBuffer(Graphic & gfx, vector<GeometryGenerator::Vertex> & vecArr);
		void SetIndexBuffer(Graphic & gfx, vector<unsigned short> & vecArr);

		template<typename T>
		void SetVertexConstantBuffer(Graphic & gfx, T & val)
		{
			Bind::VertexConstantBuffer<T> vtConstantBuffer(gfx, val);
			vtConstantBuffer.Bind(gfx);
		}

		void SetVertexShaderAndInputLayout(Graphic & gfx, string fileName = "GeometryVS.cso");
		void SetPixelShader(Graphic & gfx, string fileName = "GeometryPS.cso");
		size_t GetIndexArrSize();
	private:
		vector<GeometryGenerator::Vertex> m_vertexArr;
		vector<unsigned short> m_indexArr;
		VertexRela::VertexLayout m_vtLayout;
		GeometryGenerator::MeshData m_boxMeshData;
	};
}



#endif // !__GEOMETRYCASE_H__
