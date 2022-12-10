#ifndef __PLANECASE_H__
#define __PLANECASE_H__

#pragma once

#include "CaseBase.h"
#include "Cuboid.h"

namespace DrawTest
{
	class PlaneCase : public CaseBase
	{
	/*private:
		struct ObjectCBuf
		{
			alignas(16) DirectX::XMFLOAT3 specularColor;
			float specularWeight;
			float specularGloss;
		};
	public:
		void Init(Graphic & gfx) override;
		void Draw(Graphic & gfx, float dt) override;
	private:
		shared_ptr<Cuboid> m_plane;
		shared_ptr<Bind::TransformCbuf> m_transformBuf;

		shared_ptr<Bind::VertexShader> m_vtxShader;
		shared_ptr<Bind::PixelShader> m_pxShader;
		shared_ptr<Bind::InputLayout> m_inputLayout;
		shared_ptr<Bind::Texture> m_tex;
		shared_ptr<Bind::Sampler> m_sampler;

		ObjectCBuf m_objCBuf;
		shared_ptr<Bind::PixelConstantBuffer<ObjectCBuf>> m_pixelBuffer;*/

	public:
		void Init(Graphic & gfx) override;
		void Draw(Graphic & gfx, float dt) override {}
		void LinkTechnique(Rgph::RenderGraph & rg);
		void Submit(size_t channelFilter);
	public:
		shared_ptr<Cuboid> m_plane;
	};
}



#endif // !__PLANECASE_H__
