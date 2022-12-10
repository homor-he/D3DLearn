#include "DepthStencil.h"
#include "GraphicsThrowMacros.h"
#include "RenderTarget.h"

namespace wrl = Microsoft::WRL;

namespace Bind
{
	//用于D3D11_TEXTURE2D_DESC 
	DXGI_FORMAT MapUsageTypeless(DepthStencil::Usage usage)
	{
		switch (usage)
		{
		//用于既可以绑定depthstencil又可以绑定shaderResource
		case Bind::DepthStencil::Usage::DepthStencil:
			return DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;   //24位用于深度缓存，8位用于模板缓存
		case Bind::DepthStencil::Usage::ShadowDepth:
			return DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;	  
		}
		throw runtime_error("Base usage for Typeless format map in DepthStencil.");
	}

	//用于D3D11_DEPTH_STENCIL_VIEW_DESC
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

	//用于D3D11_SHADER_RESOURCE_VIEW_DESC
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

	void DepthStencil::BindAsBuffer(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->OMSetRenderTargets(0, nullptr, m_depthStencilView.Get()));
	}

	void DepthStencil::BindAsBuffer(Graphic & gfx, BufferResource * renderTarget)
	{
		BindAsBuffer(gfx, (RenderTarget*)(renderTarget));
	}

	void DepthStencil::BindAsBuffer(Graphic & gfx, RenderTarget * renderTarget)
	{
		renderTarget->BindAsBuffer(gfx, this);
	}

	void DepthStencil::Clear(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u));
	}

	Surface DepthStencil::ToSurface(Graphic & gfx, bool linearlize)
	{
		INFOMAN(gfx);
		//获取拷贝的内容
		auto pair = MakeStaging(gfx);
		auto pTempTex = pair.first;
		auto srcTextureDesc = pair.second;
		
		//创建surface并把图片数据放进去
		Surface s(m_width, m_height);
		D3D11_MAPPED_SUBRESOURCE msr = {};
		GFX_THROW_INFO(GetContext(gfx)->Map(pTempTex.Get(), 0, D3D11_MAP::D3D11_MAP_READ, 0, &msr));
		const char* pSrcData = static_cast<const char*>(msr.pData);

		for (unsigned int y = 0; y < m_height; ++y)
		{
			struct Pixel
			{
				char data[4];
			};
			//rowPitch每行的字节长度
			const Pixel* pSrcRow = reinterpret_cast<const Pixel*>(pSrcData + msr.RowPitch*y);
			for (unsigned int x = 0; x < m_width; ++x)
			{
				if (srcTextureDesc.Format == DXGI_FORMAT_R24G8_TYPELESS)
				{
					//用于depthStencil 深度占24位 模板占8位
					//获取深度 0最近 1最远
					const auto rawDepth = 0xFFFFFF & *reinterpret_cast<const unsigned int*>(pSrcRow + x);
					if (linearlize) //线性化
					{
						float normalized = (float)rawDepth / (float)0xFFFFFF;  //获取0~1的小数
						float linearized = 0.01f / (1.01 - normalized);        //0->0.01  1->1
						unsigned char channel = unsigned char(linearized * 0xFF);
						s.PutPixel(x, y, Surface::Color(channel, channel, channel));
					}
					else
					{
						unsigned char channel = rawDepth >> 16;
						s.PutPixel(x, y, Surface::Color(channel, channel, channel));
					}
				}
				else if (srcTextureDesc.Format == DXGI_FORMAT_R32_TYPELESS) 
				{
					//用于shadowDepth
					float rawDepth = *reinterpret_cast<const float*>(pSrcRow + x);
					if (linearlize)
					{
						float linearized = 0.01f / (1.01f - rawDepth);
						unsigned char channel = unsigned char(linearized * 255.0f);
						s.PutPixel(x, y, Surface::Color(channel, channel, channel));
					}
					else
					{
						unsigned char channel = unsigned char(rawDepth * 0xFF);
						s.PutPixel(x, y, Surface::Color(channel, channel, channel));
						
					}
				}
				else
				{
					OutPutError("%s:%d\nUnknown format in depth stencil for conversion to surface", __FILE__, __LINE__);
				}
			}
		}
		GFX_THROW_INFO_ONLY(GetContext(gfx)->Unmap(pTempTex.Get(), 0));
		return s;
	}

	pair<Microsoft::WRL::ComPtr<ID3D11Texture2D>, D3D11_TEXTURE2D_DESC> DepthStencil::MakeStaging(Graphic & gfx)
	{
		INFOMAN(gfx);
		//获取depthStencilView信息
		D3D11_DEPTH_STENCIL_VIEW_DESC srcViewDesc = {};
		m_depthStencilView->GetDesc(&srcViewDesc);
		//获取图片资源
		wrl::ComPtr<ID3D11Resource> pResource;
		m_depthStencilView->GetResource(&pResource);

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

		if (srcViewDesc.ViewDimension == D3D11_DSV_DIMENSION_TEXTURE2DARRAY)
		{
			//资源来源于cubemap texture, 使用depthStencilView信息找到正确的子资源部分
			GFX_THROW_INFO_ONLY(GetContext(gfx)->CopySubresourceRegion(pTempTex.Get(), 0, 0, 0, 0, pTexSource.Get(), srcViewDesc.Texture2DArray.FirstArraySlice, nullptr));
		}
		else
		{
			GFX_THROW_INFO_ONLY(GetContext(gfx)->CopyResource(pTempTex.Get(), pTexSource.Get()));

		}
		//return pair<Microsoft::WRL::ComPtr<ID3D11Texture2D>, D3D11_TEXTURE2D_DESC>(move(pTempTex),srcTextureDesc);
		return { move(pTempTex),srcTextureDesc };
	}

	DepthStencil::DepthStencil(Graphic & gfx, UINT width, UINT height, bool canBindShaderInput, Usage usage, BOOL mutisample, UINT quality):
		m_width(width), m_height(height)
	{
		INFOMAN(gfx);
		//创建深度模板纹理
		wrl::ComPtr<ID3D11Texture2D> pDepthStencilBuffer;
		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.Width = m_width;
		depthStencilDesc.Height = m_height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = MapUsageTypeless(usage);
		if (mutisample)
		{
			depthStencilDesc.SampleDesc.Count = 4;
			depthStencilDesc.SampleDesc.Quality = quality - 1;
		}
		else
		{
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.SampleDesc.Quality = 0;
		}
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | (canBindShaderInput ? D3D11_BIND_SHADER_RESOURCE : 0);
		GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&depthStencilDesc, nullptr, &pDepthStencilBuffer));

		//创建深度模板视图
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = MapUsageTyped(usage);
		depthStencilViewDesc.Flags = 0;
		if(mutisample)
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		else
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		GFX_THROW_INFO(GetDevice(gfx)->CreateDepthStencilView(pDepthStencilBuffer.Get(), &depthStencilViewDesc, &m_depthStencilView));
	}

	DepthStencil::DepthStencil(Graphic & gfx, Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture, UINT face)
	{
		INFOMAN(gfx);
		D3D11_TEXTURE2D_DESC textureDesc = {};
		pTexture->GetDesc(&textureDesc);
		m_width = textureDesc.Width;
		m_height = textureDesc.Height;

		//创建深度模板视图
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		depthStencilViewDesc.Texture2DArray.ArraySize = 1;
		depthStencilViewDesc.Texture2DArray.FirstArraySlice = face;
		depthStencilViewDesc.Texture2DArray.MipSlice = 0;
		GFX_THROW_INFO(GetDevice(gfx)->CreateDepthStencilView(pTexture.Get(), &depthStencilViewDesc, &m_depthStencilView));
	}

	DepthStencil::DepthStencil(Graphic & gfx, Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture)
	{
		INFOMAN(gfx);
		D3D11_TEXTURE2D_DESC textureDesc = {};
		pTexture->GetDesc(&textureDesc);
		m_width = textureDesc.Width;
		m_height = textureDesc.Height;

		//创建深度模板视图
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		GFX_THROW_INFO(GetDevice(gfx)->CreateDepthStencilView(pTexture.Get(), &depthStencilViewDesc, &m_depthStencilView));
	}


	ShaderInputDepthStencil::ShaderInputDepthStencil(Graphic & gfx, UINT slot) : 
		ShaderInputDepthStencil(gfx,gfx.GetWidth(),gfx.GetHeight(),slot)
	{
	}

	ShaderInputDepthStencil::ShaderInputDepthStencil(Graphic & gfx, UINT width, UINT height, UINT slot, Usage usage):
		DepthStencil(gfx,width,height, true,usage,gfx.Get4xMsaaStat(),gfx.GetMsaaQuality()),m_slot(slot)
	{
		INFOMAN(gfx);
		wrl::ComPtr<ID3D11Resource> pResource;
		m_depthStencilView->GetResource(&pResource);

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResViewDesc = {};
		shaderResViewDesc.Format = MapUsageColored(usage);
		if(gfx.Get4xMsaaStat())
			shaderResViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		else
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
		OutputOnlyDepthStencil(gfx,gfx.GetWidth(), gfx.GetHeight(),gfx.Get4xMsaaStat(),gfx.GetMsaaQuality())
	{
	}

	OutputOnlyDepthStencil::OutputOnlyDepthStencil(Graphic & gfx, UINT width, UINT height, BOOL mutisample, UINT quality):
		DepthStencil(gfx,width,height,false,Usage::DepthStencil, mutisample, quality)
	{
	}

	OutputOnlyDepthStencil::OutputOnlyDepthStencil(Graphic & gfx, Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture):
		DepthStencil(gfx,move(pTexture))
	{
	}

	void OutputOnlyDepthStencil::Bind(Graphic & gfx)
	{
		OutPutError("%s:%d\nOutputOnlyDepthStencil cannot be bound as shader input", __FILE__, __LINE__);
	}
}