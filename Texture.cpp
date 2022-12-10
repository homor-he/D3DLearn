#include "Texture.h"
#include "GraphicsThrowMacros.h"
#include "Surface.h"
#include "BindsDic.h"

namespace wrl = Microsoft::WRL;

namespace Bind
{
	Bind::Texture::Texture(Graphic & gfx, const string & path, UINT slot):
		m_path(path),m_slot(slot)
	{
		INFOMAN(gfx);

		Surface surface = Surface::FromFile(path);
		m_hasAlpha = surface.AlphaLoaded();

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = surface.GetWidth();
		textureDesc.Height = surface.GetHeight();
		textureDesc.MipLevels = 0;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		wrl::ComPtr<ID3D11Texture2D> pTexture;
		GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&textureDesc, nullptr, &pTexture));
		
		//写图片数据到纹理子资源中
		GetContext(gfx)->UpdateSubresource(pTexture.Get(), 0, nullptr, surface.GetBufferPtr(),
			surface.GetWidth() * sizeof(Surface::Color), 0);

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResDesc = {};
		shaderResDesc.Format = textureDesc.Format;
		shaderResDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//使用完整的纹理资源
		shaderResDesc.Texture2D.MostDetailedMip = 0;
		shaderResDesc.Texture2D.MipLevels = -1;
		GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(), &shaderResDesc, &m_TextureView));
		
		//使用gpu渲染管线生成mip chain(从大到小一连串的mipmap图)
		GetContext(gfx)->GenerateMips(m_TextureView.Get());
	}

	shared_ptr<Texture> Texture::Resolve(Graphic & gfx, const string & path, UINT slot)
	{
		return BindsDic::Resolve<Texture>(gfx, path, slot);
	}

	string Texture::GenerateUID(const string & path, UINT slot)
	{
		return typeid(Texture).name() + "#"s + path + "#" + std::to_string(slot);
	}

	void Texture::Bind(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetShaderResources(m_slot, 1, m_TextureView.GetAddressOf()));
	}

	bool Texture::HasAlpha()
	{
		return m_hasAlpha;
	}
}


