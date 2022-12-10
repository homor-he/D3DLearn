#include "Sampler.h"
#include "GraphicsThrowMacros.h"
#include "BindsDic.h"

namespace Bind
{
	Sampler::Sampler(Graphic & gfx, Type type, bool reflect, UINT slot):
		m_type(type), m_reflect(reflect), m_slot(slot)
	{
		INFOMAN(gfx);
		D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };
		//纹理过滤分为放大和缩小
		//放大：摄像机靠近纹理时，纹理会投影到整个屏幕上
		//放大指原本256X256的图片投影到1024X1024范围内，一般采用 取最近点过滤和线性过滤两种方法

		//缩小：摄像机远离纹理时，纹理会投影到某些屏幕像素上
		//缩小指原本1024X1024的图片投影到256X256范围内，一般采用mipmaping过滤，mipmap选择合适纹理时会使用点过滤和线性过滤
		switch (type)
		{
		case Bind::Sampler::Type::Anisotropic:
			samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			break;
		case Bind::Sampler::Type::Point:
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			break;
		case Bind::Sampler::Type::Bilinear:
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			break;
		}
		samplerDesc.AddressU = reflect ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = reflect ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
		GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &m_samplerStat));
	}

	shared_ptr<Sampler> Sampler::Resolve(Graphic & gfx, Type type, bool reflect, UINT slot)
	{
		return BindsDic::Resolve<Sampler>(gfx, type, reflect, slot);
	}

	string Sampler::GenerateUID(Type type, bool reflect, UINT slot)
	{
		return typeid(Sampler).name() + "#"s + std::to_string((int)type) + (reflect ? "R"s : "W"s) + "@"s + std::to_string(slot);
	}

	void Sampler::Bind(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetSamplers(m_slot, 1, m_samplerStat.GetAddressOf()));
	}
}