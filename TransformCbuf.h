#ifndef __TRANSFORMCBUF_H__
#define __TRANSFORMCBUF_H__

#pragma once

#include "ConstantBuffer.h"
#include "Drawable.h"

namespace Bind
{
	class TransformCbuf : public CloneBindable
	{
	public:
		struct Transform
		{
			DirectX::XMMATRIX model;
			DirectX::XMMATRIX modelView;
			DirectX::XMMATRIX modelViewProj;
			DirectX::XMMATRIX modelInvTranspose;      //modelµÄÄæ¾ØÕó×ªÖÃ
			//DirectX::XMMATRIX modelViewInvTranspose;  //modelViewµÄÄæ¾ØÕó×ªÖÃ
		};
	public:
		TransformCbuf(Graphic & gfx, UINT slot = 0);
		void Bind(Graphic & gfx) override;
		void InitializeParentRef(Drawable & parent) override;
		unique_ptr<CloneBindable> Clone() override;
	protected:
		void UpdateBind(Graphic& gfx, Transform & tf);
		Transform GetTransform(Graphic & gfx);
	private:
		static unique_ptr<VertexConstantBuffer<Transform>> m_pTransferBuf;
		Drawable * m_parent = nullptr;
	};
}

#endif // !__TRANSFORMCBUF_H__

