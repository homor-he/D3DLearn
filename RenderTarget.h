#ifndef __RENDERTARGET_H__
#define __RENDERTARGET_H__

#pragma once

#include "Bindable.h"

#define FACE_DEFAULT -1

namespace Bind
{
	class RenderTarget : public Bindable
	{
	protected:
		RenderTarget(Graphic & gfx, UINT width, UINT height);
		RenderTarget(Graphic & gfx, ID3D11Texture2D * pTexture, UINT face = FACE_DEFAULT);

	protected:
		UINT m_width;
		UINT m_height;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTarget;
	};

	class ShaderInputRenderTarget : public RenderTarget
	{
	public:
		ShaderInputRenderTarget(Graphic & gfx, UINT width, UINT height, UINT slot);
		void Bind(Graphic & gfx) override;
	protected:
		UINT m_slot;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
	};

	class OutPutOnlyRenderTarget : public RenderTarget
	{
	public:
		OutPutOnlyRenderTarget(Graphic & gfx, ID3D11Texture2D * pTexture, UINT face = FACE_DEFAULT);
		void Bind(Graphic & gfx) override;
	};
}


#endif // !__RENDERTARGET_H__
