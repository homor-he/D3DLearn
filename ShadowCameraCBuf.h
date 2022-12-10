#ifndef __SHADOWCAMERACBUF_H__
#define __SHADOWCAMERACBUF_H__

#pragma once

#include "Bindable.h"
#include "ConstantBuffer.h"

class Camera;

namespace Bind
{
	class ShadowCameraCBuf : public Bindable
	{
	protected:
		struct Transform
		{
			DirectX::XMMATRIX viewProjNDCTexture;
		};
	public:
		ShadowCameraCBuf(Graphic & gfx, UINT slot = 1u);
		void Bind(Graphic & gfx) override;
		void SetCamera(Camera * pCamera);
		void Update(Graphic & gfx);
	private:
		unique_ptr<VertexConstantBuffer<Transform>> m_vtxBuf;
		Camera* m_pCamera = nullptr;
	};
}


#endif // !__SHADOWCAMERACBUF_H__
