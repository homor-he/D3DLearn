#ifndef __DEPTHSTENCIL_H__
#define __DEPTHSTENCIL_H__

#pragma once

#include "Bindable.h"

namespace Bind
{
	class DepthStencil : public Bindable
	{
	public:
		enum class Usage
		{
			DepthStencil,
			ShadowDepth,
		};
	protected:
		DepthStencil(Graphic & gfx, UINT width, UINT height, bool canBindShaderInput,Usage usage);
		DepthStencil(Graphic & gfx, Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture, UINT face);
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
		OutputOnlyDepthStencil(Graphic & gfx, UINT width, UINT height);
		void Bind(Graphic & gfx) override;
	};
}

#endif // !__DEPTHSTENCIL_H__
