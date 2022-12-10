#ifndef __CONSTANTBUFFER_H__
#define __CONSTANTBUFFER_H__

#pragma once

#include "Bindable.h"
#include "GraphicsThrowMacros.h"
#include "BindsDic.h"

namespace Bind
{
	template<typename C>
	class ConstantBuffer : public Bindable
	{
	public:
		void Update(Graphic & gfx, C & constBuf)
		{
			INFOMAN(gfx);
			D3D11_MAPPED_SUBRESOURCE msr;
			GFX_THROW_INFO(GetContext(gfx)->Map(m_constantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr));
			memcpy_s(msr.pData, sizeof(C), &constBuf, sizeof(C));
			GetContext(gfx)->Unmap(m_constantBuffer.Get(), 0u);
		}

		ConstantBuffer(Graphic & gfx, UINT startSlot = 0):
			m_slot(startSlot)
		{
			INFOMAN(gfx);
			//创建常量缓存
			D3D11_BUFFER_DESC constBufDesc = {};
			constBufDesc.ByteWidth = sizeof(C);
			constBufDesc.Usage = D3D11_USAGE_DYNAMIC;;
			constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constBufDesc.MiscFlags = 0;
			constBufDesc.StructureByteStride = 0;
			GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&constBufDesc, nullptr, &m_constantBuffer));
		}

		ConstantBuffer(Graphic & gfx, C & value, UINT startSlot = 0) :
			m_slot(startSlot)
		{
			INFOMAN(gfx);
			//创建常量缓存
			D3D11_BUFFER_DESC constBufDesc = {};
			constBufDesc.ByteWidth = sizeof(value);
			constBufDesc.Usage = D3D11_USAGE_DYNAMIC;;
			constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constBufDesc.MiscFlags = 0;
			constBufDesc.StructureByteStride = 0;
			D3D11_SUBRESOURCE_DATA constInitData = {};
			constInitData.pSysMem = &value;
			GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&constBufDesc, &constInitData, &m_constantBuffer));
		}
	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
		UINT m_slot;
	};


	template<typename C>
	class VertexConstantBuffer : public ConstantBuffer<C>
	{
		using Bindable::GetContext;
		using ConstantBuffer<C>::m_slot;
		using ConstantBuffer<C>::m_constantBuffer;
	public:
		using ConstantBuffer<C>::ConstantBuffer;
		void Bind(Graphic & gfx) override
		{
			INFOMAN_NOHR(gfx);
			GFX_THROW_INFO_ONLY(GetContext(gfx)->VSSetConstantBuffers(m_slot, 1, m_constantBuffer.GetAddressOf()));
		}

		static shared_ptr<VertexConstantBuffer> Resolve(Graphic & gfx, C & value, UINT startSlot = 0)
		{
			return BindsDic::Resolve<VertexConstantBuffer>(gfx, value, startSlot);
		}

		static shared_ptr<VertexConstantBuffer> Resolve(Graphic & gfx, UINT startSlot = 0)
		{
			return BindsDic::Resolve<VertexConstantBuffer>(gfx, startSlot);
		}

		static string GenerateUID(C & value, UINT slot)
		{
			return GenerateUID(slot);
		}

		static string GenerateUID(UINT slot)
		{
			return typeid(VertexConstantBuffer).name() + "#"s + to_string(slot);
		}
	};

	template<typename C>
	class PixelConstantBuffer : public ConstantBuffer<C>
	{
		using Bindable::GetContext;
		using ConstantBuffer<C>::m_slot;
		using ConstantBuffer<C>::m_constantBuffer;
		//using ConstantBuffer<C>::GetInfoManager;
	public:
		using ConstantBuffer<C>::ConstantBuffer;
		void Bind(Graphic & gfx) override
		{
			INFOMAN_NOHR(gfx);
			GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetConstantBuffers(m_slot, 1, m_constantBuffer.GetAddressOf()));
		}

		static shared_ptr<PixelConstantBuffer> Resolve(Graphic & gfx, C & value, UINT startSlot = 0)
		{
			return make_shared<PixelConstantBuffer>(gfx, value, startSlot);
		}

		static shared_ptr<PixelConstantBuffer> Resolve(Graphic & gfx, UINT startSlot = 0)
		{
			return make_shared<PixelConstantBuffer>(gfx, startSlot);
		}

		static string GenerateUID(C & value, UINT startSlot)
		{
			return GenerateUID(startSlot);
		}

		static string GenerateUID(UINT startSlot)
		{
			return typeid(PixelConstantBuffer).name() + "#"s + to_string(slot);
		}
	};
}

#endif // !__CONSTANTBUFFER_H__
