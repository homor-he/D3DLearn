#include "CubeTexture.h"
#include "Surface.h"
#include "GraphicsThrowMacros.h"
#include "DepthStencil.h"
#include "RenderTarget.h"

namespace Bind
{
	CubeTexture::CubeTexture(Graphic & gfx, const string & path, UINT slot)
		: m_path(path), m_slot(slot)
	{
		INFOMAN(gfx);
		vector<Surface> vecSurfaces;
		for (size_t i = 0; i < 6; ++i)
		{
			vecSurfaces.push_back(Surface::FromFile(path + "\\" + to_string(i) + ".png"));
		}

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = vecSurfaces[0].GetWidth();
		textureDesc.Height = vecSurfaces[0].GetHeight();
		//mipLevel=1时只包含该位图本身，若=3，且位图尺寸为400X400时,则会生成400X400,200X200,100X100的mipmap
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 6;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		D3D11_SUBRESOURCE_DATA resData[6];
		for (int i = 0; i < 6; ++i)
		{
			resData[i].pSysMem = vecSurfaces[i].GetBufferPtr();
			//前一行的头到后一行的头的字节间隔长度
			resData[i].SysMemPitch = vecSurfaces[i].GetBytePitch();
			resData[i].SysMemSlicePitch = 0;
		}

		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
		GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&textureDesc, resData, &pTexture));

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResDesc = {};
		shaderResDesc.Format = textureDesc.Format;
		shaderResDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		shaderResDesc.Texture2D.MostDetailedMip = 0;
		shaderResDesc.Texture2D.MipLevels = 1;
		GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(), &shaderResDesc, &m_pTextureView));

		//GetContext(gfx)->GenerateMips(m_pTextureView.Get());
	}

	void CubeTexture::Bind(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetShaderResources(m_slot, 1u, m_pTextureView.GetAddressOf()));
	}


	CubeTargetTexture::CubeTargetTexture(Graphic & gfx, UINT width, UINT height, UINT slot, DXGI_FORMAT format)
		: m_slot(slot)
	{
		INFOMAN(gfx);

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		//mipLevel=1时只包含该位图本身，若=3，且位图尺寸为400X400时,则会生成400X400,200X200,100X100的mipmap
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 6;
		textureDesc.Format = format;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
		GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&textureDesc, nullptr, &pTexture));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(), &srvDesc, &m_pTextureView));

		//为了抓取阴影贴图的渲染目标资源
		for (size_t faceIndex = 0; faceIndex < 6; faceIndex++)
		{
			m_vecRenderTarget.push_back(make_shared<Bind::OutputOnlyRenderTarget>(gfx, pTexture.Get(), gfx.Get4xMsaaStat(), faceIndex));
		}
	}

	void CubeTargetTexture::Bind(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetShaderResources(m_slot, 1u, m_pTextureView.GetAddressOf()));
	}

	shared_ptr<OutputOnlyRenderTarget> CubeTargetTexture::GetRenderTarget(size_t index)
	{
		return m_vecRenderTarget[index];
	}

	DepthCubeTexture::DepthCubeTexture(Graphic & gfx, UINT width, UINT height, UINT slot) : m_slot(slot)
	{
		INFOMAN(gfx);

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 6;
		//用于阴影深度
		textureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
		GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&textureDesc, nullptr, &pTexture));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(), &srvDesc, &m_pTextureView));

		//为捕捉阴影贴图，生成深度模板资源
		for (size_t faceIndex = 0; faceIndex < 6; faceIndex++)
		{
			m_vecDepthStencil.push_back(make_shared<OutputOnlyDepthStencil>(gfx, pTexture.Get(), faceIndex));
		}
	}

	void DepthCubeTexture::Bind(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetShaderResources(m_slot, 1u, m_pTextureView.GetAddressOf()));
	}

	shared_ptr<OutputOnlyDepthStencil> DepthCubeTexture::GetDepthBuffer(size_t index)
	{
		return m_vecDepthStencil[index];
	}
}