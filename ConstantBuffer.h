#ifndef __CONSTANTBUFFER_H__
#define __CONSTANTBUFFER_H__

#pragma once

#include "Bindable.h"
#include "GraphicsThrowMacros.h"

namespace Bind
{
	template<typename C>
	class ConstantBuffer : public Bindable
	{
	public:
		ConstantBuffer(Graphic & gfx, UINT startSlot = 0):
			m_slot(startSlot)
		{
			INFOMAN(gfx);
			//创建常量缓存
			D3D11_BUFFER_DESC constBufDesc;
			constBufDesc.ByteWidth = sizeof(value);
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
			D3D11_BUFFER_DESC constBufDesc;
			constBufDesc.ByteWidth = sizeof(value);
			constBufDesc.Usage = D3D11_USAGE_DYNAMIC;;
			constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constBufDesc.MiscFlags = 0;
			constBufDesc.StructureByteStride = 0;
			D3D11_SUBRESOURCE_DATA constInitData;
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
	};
	
	template<typename C>
	class PixelConstantBuffer : public ConstantBuffer<C>
	{
		using Bindable::GetContext;
		using ConstantBuffer<C>::m_slot;
		using ConstantBuffer<C>::m_constantBuffer;
	public:
		void Bind(Graphic & gfx) override
		{
			INFOMAN_NOHR(gfx);
			GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetConstantBuffers(m_slot, 1, m_constantBuffer.GetAddressOf()));
		}
	};
}

#endif // !__CONSTANTBUFFER_H__
