#ifndef __CONSTANTBUFFEREX_H__
#define __CONSTANTBUFFEREX_H__

#pragma once

#include "Bindable.h"
#include "GraphicsThrowMacros.h"
#include "DynamicConstant.h"

namespace Bind
{
	class ConstantBufferEx : public Bindable
	{
	public:
		void Update(Graphic & gfx, Dcb::Buffer& buf)
		{
			if (&buf.GetRootLayoutElement() != &GetRootLayoutElement())
			{
				OutPutError("%s:%d\nnot point to same addr", __FILE__, __LINE__);
			}

			INFOMAN(gfx);
			D3D11_MAPPED_SUBRESOURCE msr;
			GFX_THROW_INFO(GetContext(gfx)->Map(m_pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr));
			memcpy_s(msr.pData, buf.GetSizeInBytes(), buf.GetData(), buf.GetSizeInBytes());
			GetContext(gfx)->Unmap(m_pConstantBuffer.Get(), 0u);
		}

		virtual const Dcb::LayoutElement& GetRootLayoutElement() const=0;
	protected:
		ConstantBufferEx(Graphic& gfx, const Dcb::LayoutElement& layoutRoot, UINT slot, const Dcb::Buffer * pBuf)
			: m_slot(slot)
		{
			INFOMAN(gfx);
			//创建常量缓存
			D3D11_BUFFER_DESC constBufDesc = {};
			constBufDesc.ByteWidth = (UINT)layoutRoot.GetSizeInBytes();
			constBufDesc.Usage = D3D11_USAGE_DYNAMIC;;
			constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constBufDesc.MiscFlags = 0;
			constBufDesc.StructureByteStride = 0;
			if (pBuf != nullptr)
			{
				D3D11_SUBRESOURCE_DATA constInitData = {};
				constInitData.pSysMem = pBuf->GetData();
				GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&constBufDesc, &constInitData, &m_pConstantBuffer));
			}
			else
			{
				GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&constBufDesc, nullptr, &m_pConstantBuffer));
			}
		}
	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
		UINT m_slot;
	};

	class VertexConstantBufferEx : public ConstantBufferEx
	{
	public:
		//using可直接实现继承的构造函数
		using ConstantBufferEx::ConstantBufferEx;
		void Bind(Graphic & gfx) override
		{
			INFOMAN_NOHR(gfx);
			GFX_THROW_INFO_ONLY(GetContext(gfx)->VSSetConstantBuffers(m_slot, 1u, m_pConstantBuffer.GetAddressOf()));
		}
	};

	class PixelConstantBufferEx : public ConstantBufferEx
	{
	public:
		using ConstantBufferEx::ConstantBufferEx;
		void Bind(Graphic & gfx) override
		{
			INFOMAN_NOHR(gfx);
			GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetConstantBuffers(m_slot, 1u, m_pConstantBuffer.GetAddressOf()));
		}
	};

	template<class T>
	class CachingConstantBufferEx : public T
	{
	public:
		CachingConstantBufferEx(Graphic & gfx, const Dcb::CookedLayout& layout, UINT slot)
			: T(gfx,*layout.ShareRoot(),slot,nullptr), m_buf(Dcb::Buffer(layout))
		{}

		CachingConstantBufferEx(Graphic & gfx, const Dcb::Buffer& buf, UINT slot)
			: T(gfx, buf.GetRootLayoutElement(), slot, &buf), m_buf(buf)
		{}

		const Dcb::LayoutElement& GetRootLayoutElement() const
		{
			return m_buf.GetRootLayoutElement();
		}

		const Dcb::Buffer& GetBuffer()
		{
			return m_buf;
		}

		void SetBuffer(const Dcb::Buffer & buf)
		{
			m_buf.CopyFrom(buf);
			m_dirty = true;
		}

		void Bind(Graphic & gfx)
		{
			if (m_dirty)
			{
				T::Update(gfx, m_buf);
				m_dirty = false;
			}
			T::Bind(gfx);
		}
	private:
		bool m_dirty = false;
		Dcb::Buffer m_buf;
	};

	using CachingPixelConstantBufferEx = CachingConstantBufferEx<PixelConstantBufferEx>;
	using CachingVertexConstantBufferEx = CachingConstantBufferEx<VertexConstantBufferEx>;
}


#endif // !__CONSTANTBUFFEREX_H__
