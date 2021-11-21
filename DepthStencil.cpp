#include "DepthStencil.h"
#include "GraphicsThrowMacros.h"

namespace wrl = Microsoft::WRL;

namespace Bind
{
	DXGI_FORMAT MapUsageTypeless(DepthStencil::Usage usage)
	{
		switch (usage)
		{
		case Bind::DepthStencil::Usage::DepthStencil:
			return DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
		case Bind::DepthStencil::Usage::ShadowDepth:
			return DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
		}
		throw runtime_error("Base usage for Typeless format map in DepthStencil.");
	}

	DXGI_FORMAT MapUsageTyped(DepthStencil::Usage usage)
	{
		switch (usage)
		{
		case Bind::DepthStencil::Usage::DepthStencil:
			return DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		case Bind::DepthStencil::Usage::ShadowDepth:
			return DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
		}
		throw runtime_error("Base usage for Typed format map in DepthStencil.");
	}

	DXGI_FORMAT MapUsageColored(DepthStencil::Usage usage)
	{
		switch (usage)
		{
		case Bind::DepthStencil::Usage::DepthStencil:
			return DXGI_FORMAT::DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		case Bind::DepthStencil::Usage::ShadowDepth:
			return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		}
		throw runtime_error("Base usage for Colored format map in DepthStencil.");
	}

	DepthStencil::DepthStencil(Graphic & gfx, UINT width, UINT height, bool canBindShaderInput, Usage usage):
		m_width(width), m_height(height)
	{
		INFOMAN(gfx);
		//创建深度模板纹理
		wrl::ComPtr<ID3D11Texture2D> pDepthStencilBuffer;
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = m_width;
		depthStencilDesc.Height = m_height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = MapUsageTypeless(usage);
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | (canBindShaderInput ? D3D11_BIND_SHADER_RESOURCE : 0);
		GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&depthStencilDesc, nullptr, &pDepthStencilBuffer));

		//创建深度模板视图
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		depthStencilViewDesc.Format = MapUsageTyped(usage);
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		GFX_THROW_INFO(GetDevice(gfx)->CreateDepthStencilView(pDepthStencilBuffer.Get(), &depthStencilViewDesc, &m_depthStencilView));
	}

	DepthStencil::DepthStencil(Graphic & gfx, Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture, UINT face)
	{
		INFOMAN(gfx);
		D3D11_TEXTURE2D_DESC textureDesc;
		pTexture->GetDesc(&textureDesc);
		m_width = textureDesc.Width;
		m_height = textureDesc.Height;

		//创建深度模板视图
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		depthStencilViewDesc.Texture2DArray.ArraySize = 1;
		depthStencilViewDesc.Texture2DArray.FirstArraySlice = face;
		depthStencilViewDesc.Texture2DArray.MipSlice = 0;
		GFX_THROW_INFO(GetDevice(gfx)->CreateDepthStencilView(pTexture.Get(), &depthStencilViewDesc, &m_depthStencilView));
	}


	ShaderInputDepthStencil::ShaderInputDepthStencil(Graphic & gfx, UINT slot) : 
		ShaderInputDepthStencil(gfx,gfx.GetWidth(),gfx.GetHeight(),slot)
	{
	}

	ShaderInputDepthStencil::ShaderInputDepthStencil(Graphic & gfx, UINT width, UINT height, UINT slot, Usage usage):
		DepthStencil(gfx,width,height,true,usage),m_slot(slot)
	{
		INFOMAN(gfx);
		wrl::ComPtr<ID3D11Resource> pResource;
		m_depthStencilView->GetResource(&pResource);

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResViewDesc;
		shaderResViewDesc.Format = MapUsageColored(usage);
		shaderResViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResViewDesc.Texture2D.MipLevels = 1;
		GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(pResource.Get(), &shaderResViewDesc, &m_shaderResView));
	}

	void ShaderInputDepthStencil::Bind(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetShaderResources(m_slot, 1, m_shaderResView.GetAddressOf()));
	}

	OutputOnlyDepthStencil::OutputOnlyDepthStencil(Graphic & gfx, Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture, UINT face):
		DepthStencil(gfx,move(pTexture),face)
	{
	}

	OutputOnlyDepthStencil::OutputOnlyDepthStencil(Graphic & gfx):
		OutputOnlyDepthStencil(gfx,gfx.GetWidth(), gfx.GetHeight())
	{
	}

	OutputOnlyDepthStencil::OutputOnlyDepthStencil(Graphic & gfx, UINT width, UINT height):
		DepthStencil(gfx,width,height,false,Usage::DepthStencil)
	{
	}

	void OutputOnlyDepthStencil::Bind(Graphic & gfx)
	{
		OutPutError("%s:%d, OutputOnlyDepthStencil cannot be bound as shader input", __FILE__, __LINE__);
	}
}