#ifndef __CUBETEXTURE_H__
#define __CUBETEXTURE_H__

#pragma once

#include "Bindable.h"

namespace Bind
{
	class OutputOnlyDepthStencil;
	class OutputOnlyRenderTarget;

	class CubeTexture : public Bindable
	{
	public:
		CubeTexture(Graphic & gfx, const string & path, UINT slot = 0);
		void Bind(Graphic & gfx) override;
	private:
		UINT m_slot;
	protected:
		string m_path;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureView;
	};

	class CubeTargetTexture : public Bindable
	{
	public:
		CubeTargetTexture(Graphic & gfx, UINT width, UINT height, UINT slot = 0, DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM);
		void Bind(Graphic & gfx) override;
		shared_ptr<OutputOnlyRenderTarget> GetRenderTarget(size_t index);
	private:
		UINT m_slot;
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureView;
		vector<shared_ptr<OutputOnlyRenderTarget>> m_vecRenderTarget;
	};

	class DepthCubeTexture : public Bindable
	{
	public:
		DepthCubeTexture(Graphic & gfx, UINT width, UINT height, UINT slot);
		void Bind(Graphic & gfx) override;
		shared_ptr<OutputOnlyDepthStencil> GetDepthBuffer(size_t index);
	private:
		UINT m_slot;
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureView;
		vector<shared_ptr<OutputOnlyDepthStencil>> m_vecDepthStencil;
	};
}


#endif // !__CUBETEXTURE_H__
