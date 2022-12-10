#ifndef __READOBJCASE_H__
#define __READOBJCASE_H__

#pragma once

#include "CaseBase.h"

namespace DrawTest
{
	class ReadObjCase : public CaseBase
	{
	private:
		struct ObjectCBuf
		{
			bool useGlossAlpha;
			bool useSpecularMap;
			alignas(16) DirectX::XMFLOAT3 specularColor;
			float specularWeight;
			float specularGloss;
			bool useNormalMap;
			float normalMapWeight;
		};
	public:
		void Init(Graphic & gfx) override;
		void Draw(Graphic & gfx, float dt) override;
	protected:
		//shared_ptr<Mesh> m_brickWall;
		//shared_ptr<Bind::TransformCbuf> m_brickWallTransfromBuf;

		shared_ptr<Bind::VertexBuffer> m_pVtBuffer;
		shared_ptr<Bind::IndexBuffer> m_pIdBuffer;
		shared_ptr<Bind::Topology> m_pTopology;
		shared_ptr<Bind::VertexShader> m_vtxShader;
		shared_ptr<Bind::PixelShader> m_pxShader;
		shared_ptr<Bind::InputLayout> m_inputLayout;
		shared_ptr<Bind::Texture> m_pTexture;
		shared_ptr<Bind::Sampler> m_pSampler;
		//shared_ptr<Bind::TransformCbuf> m_pTransform;
		shared_ptr<Bind::VertexConstantBuffer<Bind::TransformCbuf::Transform>> m_pTransform;
		shared_ptr<Bind::Texture> m_pNormalTex;
		shared_ptr<Bind::Texture> m_pSpecTex;

		ObjectCBuf m_pixelObjectCBuf;
	};
}

#endif // !__READOBJCASE_H__
