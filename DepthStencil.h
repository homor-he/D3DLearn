#ifndef __DEPTHSTENCIL_H__
#define __DEPTHSTENCIL_H__

#pragma once

#include "Bindable.h"
#include "BufferResource.h"
#include "Surface.h"

namespace Bind
{
	class RenderTarget;
	class OutputOnlyDepthStencil;

	class DepthStencil : public Bindable, public BufferResource
	{
		friend RenderTarget;
	public:
		enum class Usage
		{
			DepthStencil,
			ShadowDepth,
		};
	public:
		void BindAsBuffer(Graphic & gfx) override;
		void BindAsBuffer(Graphic & gfx, BufferResource * renderTarget) override;
		void BindAsBuffer(Graphic & gfx, RenderTarget * renderTarget);
		void Clear(Graphic & gfx) override;
		Surface ToSurface(Graphic & gfx, bool linearlize = true);
	private:
		//获取depthStencil的内容和属性
		pair<Microsoft::WRL::ComPtr<ID3D11Texture2D>, D3D11_TEXTURE2D_DESC> MakeStaging(Graphic& gfx);
	protected:
		DepthStencil(Graphic & gfx, UINT width, UINT height, bool canBindShaderInput,Usage usage, BOOL mutisample, UINT quality);
		DepthStencil(Graphic & gfx, Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture, UINT face);
		//给outputOnlyDepthStencil shadowDepth用
		DepthStencil(Graphic & gfx, Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture);
	protected:
		UINT m_width;
		UINT m_height;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	};

	class ShaderInputDepthStencil : public DepthStencil
	{
	public:
		ShaderInputDepthStencil(Graphic & gfx, UINT slot);
		ShaderInputDepthStencil(Graphic & gfx, UINT width, UINT height, UINT slot, Usage usage = Usage::DepthStencil);
		void Bind(Graphic & gfx) override;
	protected:
		UINT m_slot;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResView;
	};

	class OutputOnlyDepthStencil : public DepthStencil
	{
	public:
		OutputOnlyDepthStencil(Graphic & gfx, Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture, UINT face);
		OutputOnlyDepthStencil(Graphic & gfx);
		OutputOnlyDepthStencil(Graphic & gfx, UINT width, UINT height, BOOL mutisample, UINT quality);
		//给shadowdepth用
		OutputOnlyDepthStencil(Graphic & gfx, Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture);
		void Bind(Graphic & gfx) override;
	};
}

#endif // !__DEPTHSTENCIL_H__
