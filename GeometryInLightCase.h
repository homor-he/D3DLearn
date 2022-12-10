#ifndef __GEOMETRYINLIGHTCASE_H__
#define __GEOMETRYINLIGHTCASE_H__
#endif // !__GEOMETRYINLIGHTCASE_H__

#pragma once

#include "Vertex.h"
#include "BindableCommon.h"
#include "Cuboid.h"

namespace DrawTest
{
	//与光源一起实现伪动态光照
	class GeometryInLightCase
	{
	private:
		struct ObjectCBuf
		{
			alignas(16) DirectX::XMFLOAT3 materialColor;
			alignas(16) DirectX::XMFLOAT3 specularColor;
			float specularWeight;
			float specularGloss;
		};
	public:
		void Init(Graphic & gfx);
		void Draw(Graphic & gfx, float dt);
	private:
		shared_ptr<Cuboid> m_cube;
		shared_ptr<Bind::TransformCbuf> m_cubeTransfromBuf;
		ObjectCBuf m_pixelObjectCBuf;

		shared_ptr<Bind::VertexShader> m_vtxShader;
		shared_ptr<Bind::PixelShader> m_pxShader;
		shared_ptr<Bind::InputLayout> m_inputLayout;
	};
}
