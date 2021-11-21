#include "RenderTarget.h"
#include "GraphicsThrowMacros.h"

namespace wrl = Microsoft::WRL;

namespace Bind
{
	RenderTarget::RenderTarget(Graphic & gfx, UINT width, UINT height):
		m_width(width), m_height(height)
	{
		INFOMAN(gfx);
		//创建2D纹理
		D3D11_TEXTURE2D_DESC textureDesc;
		textureDesc.Width = m_width;
		textureDesc.Height = m_height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		wrl::ComPtr<ID3D11Texture2D> pTexture;
		GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&textureDesc, nullptr, &pTexture));
		
		//创建渲染目标视图
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc;
		renderTargetDesc.Format = textureDesc.Format;
		renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetDesc.Texture2D = D3D11_TEX2D_RTV{0};
		GFX_THROW_INFO(GetDevice(gfx)->CreateRenderTargetView(pTexture.Get(), &renderTargetDesc, &m_renderTarget));
	}

	RenderTarget::RenderTarget(Graphic & gfx, ID3D11Texture2D * pTexture, UINT face)
	{
		INFOMAN(gfx);
		//获取图片信息
		D3D11_TEXTURE2D_DESC textureDesc;
		pTexture->GetDesc(&textureDesc);
		m_width = textureDesc.Width;
		m_height = textureDesc.Height;

		//创建渲染视图
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc;
		renderTargetDesc.Format = textureDesc.Format;
		renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetDesc.Texture2D = D3D11_TEX2D_RTV{0};
		GFX_THROW_INFO(GetDevice(gfx)->CreateRenderTargetView(pTexture, &renderTargetDesc, &m_renderTarget));
	}

	ShaderInputRenderTarget::ShaderInputRenderTarget(Graphic & gfx, UINT width, UINT height, UINT slot):
		RenderTarget(gfx,width,height),m_slot(slot)
	{
		INFOMAN(gfx);
		wrl::ComPtr<ID3D11Resource> pResource;
		m_renderTarget->GetResource(&pResource);

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResDesc;
		shaderResDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		shaderResDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResDesc.Texture2D.MostDetailedMip = 0;
		shaderResDesc.Texture2D.MipLevels = 1;
		GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(pResource.Get(),
			&shaderResDesc, &m_shaderResourceView));

	}

	void ShaderInputRenderTarget::Bind(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetShaderResources(m_slot, 1, m_shaderResourceView.GetAddressOf()));
	}

	OutPutOnlyRenderTarget::OutPutOnlyRenderTarget(Graphic & gfx, ID3D11Texture2D * pTexture, UINT face) :
		RenderTarget(gfx, pTexture, face)
	{
	}

	void OutPutOnlyRenderTarget::Bind(Graphic & gfx)
	{
		OutPutError("%s:%d, Cannot bind OuputOnlyRenderTarget as shader input", __FILE__, __LINE__);
	}
}