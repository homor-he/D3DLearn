#ifndef __SKYBOXCASE_H__
#define __SKYBOXCASE_H__

#pragma once

#include "CaseBase.h"
#include "Cuboid.h"
#include "Sphere.h"
#include "CubeTexture.h"
#include "SkyboxTransformCbuf.h"

namespace DrawTest
{
	using namespace Bind;
	class SkyboxCase : public CaseBase
	{
	public:
		void Init(Graphic & gfx) override;
		void Draw(Graphic & gfx, float dt) override;
	private:
		bool m_userSphere = true;
		shared_ptr<Cuboid> m_pCube;
		shared_ptr<Sphere> m_pSphere;
		shared_ptr<SkyboxTransformCbuf> m_pTransformCbuf;

		shared_ptr<VertexShader> m_pVtxShader;
		shared_ptr<PixelShader> m_pPxShader;
		shared_ptr<InputLayout> m_pInputLayout;

		shared_ptr<CubeTexture> m_pCubeTexture;
		shared_ptr<Sampler> m_pSampler;
		shared_ptr<Stencil> m_pStencilDepthFirst;
		shared_ptr<Stencil> m_pStencilOff;
		shared_ptr<Rasterizer> m_pRasterizer;
	};
}


#endif // !__SKYBOXCASE_H__
