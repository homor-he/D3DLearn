#include "RenderTarget.h"
#include "GraphicsThrowMacros.h"
#include "DepthStencil.h"

namespace wrl = Microsoft::WRL;

namespace Bind
{
	void RenderTarget::BindAsBuffer(Graphic & gfx)
	{
		BindAsBuffer(gfx, static_cast<ID3D11DepthStencilView*>(nullptr));
	}

	void RenderTarget::BindAsBuffer(Graphic & gfx, BufferResource * depthStencil)
	{
		BindAsBuffer(gfx, static_cast<DepthStencil*>(depthStencil));
	}

	void RenderTarget::BindAsBuffer(Graphic & gfx, DepthStencil * depthStencil)
	{
		BindAsBuffer(gfx, depthStencil ? depthStencil->m_depthStencilView.Get() : nullptr);
	}

	void RenderTarget::BindAsBuffer(Graphic & gfx, ID3D11DepthStencilView * view)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->OMSetRenderTargets(1, m_renderTarget.GetAddressOf(), view));
	}

	pair<Microsoft::WRL::ComPtr<ID3D11Texture2D>, D3D11_TEXTURE2D_DESC> RenderTarget::MakeStaging(Graphic & gfx)
	{
		INFOMAN(gfx);
		//获取depthStencilView信息
		D3D11_RENDER_TARGET_VIEW_DESC srcViewDesc = {};
		m_renderTarget->GetDesc(&srcViewDesc);
		//获取图片资源
		wrl::ComPtr<ID3D11Resource> pResource;
		m_renderTarget->GetResource(&pResource);

		wrl::ComPtr<ID3D11Texture2D> pTexSource;
		pResource.As(&pTexSource);
		D3D11_TEXTURE2D_DESC srcTextureDesc = {};
		pTexSource->GetDesc(&srcTextureDesc);
		D3D11_TEXTURE2D_DESC tempTextureDesc = srcTextureDesc;
		tempTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		tempTextureDesc.Usage = D3D11_USAGE_STAGING;
		tempTextureDesc.BindFlags = 0;
		tempTextureDesc.MiscFlags = 0;
		tempTextureDesc.ArraySize = 1;
		wrl::ComPtr<ID3D11Texture2D> pTempTex;
		GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&tempTextureDesc, nullptr, &pTempTex));

		if (srcViewDesc.ViewDimension == D3D11_RTV_DIMENSION_TEXTURE2DARRAY)
		{
			//资源来源于cubemap texture, 使用depthStencilView信息找到正确的子资源部分
			GFX_THROW_INFO_ONLY(GetContext(gfx)->CopySubresourceRegion(pTempTex.Get(), 0, 0, 0, 0, pTexSource.Get(), srcViewDesc.Texture2DArray.FirstArraySlice, nullptr));
		}
		else
		{
			GFX_THROW_INFO_ONLY(GetContext(gfx)->CopyResource(pTempTex.Get(), pTexSource.Get()));

		}
		return{ move(pTempTex),srcTextureDesc };
	}

	void RenderTarget::Clear(Graphic & gfx)
	{
		Clear(gfx, array<float, 4>{ 0.0f, 0.0f, 0.0f, 0.0f });
	}

	void RenderTarget::Clear(Graphic & gfx, array<float, 4>& color)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->ClearRenderTargetView(m_renderTarget.Get(), color.data()));
	}

	void RenderTarget::ReleaseRenderTarget()
	{
		m_renderTarget.Reset();
	}

	Surface RenderTarget::ToSurface(Graphic & gfx)
	{
		INFOMAN(gfx);
		auto pair = MakeStaging(gfx);
		auto pTempTex = pair.first;
		auto srcTextureDesc = pair.second;
		/*if (pair.second.Format != DXGI_FORMAT_B8G8R8A8_UNORM)
		{
			OutPutError("%s:%d\nsurface format is wrong", __FILE__, __LINE__);
		}*/

		Surface s(m_width, m_height);
		D3D11_MAPPED_SUBRESOURCE msr = {};
		GFX_THROW_INFO(GetContext(gfx)->Map(pTempTex.Get(), 0, D3D11_MAP_READ, 0, &msr));
		const char * pSrcData = static_cast<const char*>(msr.pData);
		for (unsigned int y = 0; y < m_height; ++y)
		{
			struct Pixel
			{
				char data[4];
			};

			//auto pSrcRow = reinterpret_cast<const Surface::Color*>(pSrcData + msr.RowPitch * y);
			const Pixel * pSrcRow = reinterpret_cast<const Pixel*>(pSrcData + msr.RowPitch*y);
			for (unsigned int x = 0; x < m_width; ++x)
			{
				float viewZDepth = *reinterpret_cast<const float*>(pSrcRow + x);
				unsigned char channel = unsigned char(viewZDepth * 0xff);
				s.PutPixel(x, y, Surface::Color(channel, channel, channel));
				//s.PutPixel(x, y, *(pSrcRow + x));
			}
		}
		GFX_THROW_INFO_ONLY(GetContext(gfx)->Unmap(pTempTex.Get(), 0));
		return s;
	}

	RenderTarget::RenderTarget(Graphic & gfx, UINT width, UINT height):
		m_width(width), m_height(height)
	{
		INFOMAN(gfx);
		//创建2D纹理
		D3D11_TEXTURE2D_DESC textureDesc = {};
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
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc = {};
		renderTargetDesc.Format = textureDesc.Format;
		renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetDesc.Texture2D = D3D11_TEX2D_RTV{0};
		GFX_THROW_INFO(GetDevice(gfx)->CreateRenderTargetView(pTexture.Get(), &renderTargetDesc, &m_renderTarget));
	}

	RenderTarget::RenderTarget(Graphic & gfx, ID3D11Texture2D * pTexture, BOOL mutisample, UINT face)
	{
		INFOMAN(gfx);
		//获取图片信息
		D3D11_TEXTURE2D_DESC textureDesc = {};
		pTexture->GetDesc(&textureDesc);
		m_width = textureDesc.Width;
		m_height = textureDesc.Height;

		//创建渲染视图
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc = {};
		renderTargetDesc.Format = textureDesc.Format;
		if (face != FACE_DEFAULT)
		{
			if (mutisample)
				renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
			else
				renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			renderTargetDesc.Texture2DArray.ArraySize = 1;
			renderTargetDesc.Texture2DArray.FirstArraySlice = face;
			renderTargetDesc.Texture2DArray.MipSlice = 0;
		}
		else
		{
			if (mutisample)
				renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
			else
				renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			renderTargetDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
		}
		GFX_THROW_INFO(GetDevice(gfx)->CreateRenderTargetView(pTexture, &renderTargetDesc, &m_renderTarget));
	}

	ShaderInputRenderTarget::ShaderInputRenderTarget(Graphic & gfx, UINT width, UINT height, UINT slot):
		RenderTarget(gfx,width,height),m_slot(slot)
	{
		INFOMAN(gfx);
		wrl::ComPtr<ID3D11Resource> pResource;
		m_renderTarget->GetResource(&pResource);

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResDesc = {};
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

	OutputOnlyRenderTarget::OutputOnlyRenderTarget(Graphic & gfx, ID3D11Texture2D * pTexture, BOOL mutisample, UINT face) :
		RenderTarget(gfx, pTexture, mutisample, face)
	{
	}

	void OutputOnlyRenderTarget::Bind(Graphic & gfx)
	{
		OutPutError("%s:%d, Cannot bind OuputOnlyRenderTarget as shader input", __FILE__, __LINE__);
	}
}