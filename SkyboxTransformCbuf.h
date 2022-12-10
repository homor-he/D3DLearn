#ifndef __SKYBOXTRANSFORMCBUF_H__
#define __SKYBOXTRANSFORMCBUF_H__

#pragma once

#include "ConstantBuffer.h"

namespace Bind
{
	class SkyboxTransformCbuf : public Bindable
	{
	protected:
		struct Transform
		{
			DirectX::XMMATRIX viewProj;
		};
	public:
		SkyboxTransformCbuf(Graphic & gfx, UINT slot = 0u);
		void Bind(Graphic & gfx) override;
	protected:
		void UpdateBind(Graphic& gfx, Transform & tf);
		Transform GetTransform(Graphic & gfx);
	private:
		unique_ptr<VertexConstantBuffer<Transform>> m_vtxBuf;
	};
}



#endif // !__SKYBOXTRANSFORMCBUF_H__
