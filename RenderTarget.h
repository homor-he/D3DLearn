#ifndef __RENDERTARGET_H__
#define __RENDERTARGET_H__

#pragma once

#include "Bindable.h"
#include "BufferResource.h"
#include <array>
#include "Surface.h"

#define FACE_DEFAULT -1

namespace Bind
{
	class DepthStencil;

	class RenderTarget : public Bindable,public BufferResource
	{
	public:
		void BindAsBuffer(Graphic & gfx) override;
		void BindAsBuffer(Graphic & gfx, BufferResource * depthStencil) override;
		void BindAsBuffer(Graphic & gfx, DepthStencil * depthStencil);
		void Clear(Graphic & gfx, array<float, 4>& color);
		void Clear(Graphic & gfx) override;
		void ReleaseRenderTarget();
		Surface ToSurface(Graphic & gfx);
	private:
		void BindAsBuffer(Graphic & gfx, ID3D11DepthStencilView* view);
		pair<Microsoft::WRL::ComPtr<ID3D11Texture2D>, D3D11_TEXTURE2D_DESC> MakeStaging(Graphic & gfx);
	protected:
		RenderTarget(Graphic & gfx, UINT width, UINT height);
		RenderTarget(Graphic & gfx, ID3D11Texture2D * pTexture, BOOL mutisample, UINT face = FACE_DEFAULT);
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

	class OutputOnlyRenderTarget : public RenderTarget
	{
	public:
		OutputOnlyRenderTarget(Graphic & gfx, ID3D11Texture2D * pTexture, BOOL mutisample, UINT face = FACE_DEFAULT);
		void Bind(Graphic & gfx) override;
	};
}


#endif // !__RENDERTARGET_H__
