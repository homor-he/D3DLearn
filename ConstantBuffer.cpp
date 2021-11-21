#include "ConstantBuffer.h"
#include "GraphicsThrowMacros.h"

namespace Bind
{
	template<typename C>
	ConstantBuffer<C>::ConstantBuffer(Graphic & gfx, UINT startSlot):
		m_slot(startSlot)
	{
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

	template<typename C>
	ConstantBuffer<C>::ConstantBuffer(Graphic & gfx, C & value, UINT startSlot):
		m_slot(startSlot)
	{
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

	template<typename C>
	void VertexConstantBuffer<C>::Bind(Graphic & gfx)
	{
		GFX_THROW_INFO_ONLY(GetContext(gfx)->VSSetConstantBuffers(m_slot, 1, m_constantBuffer.GetAddressOf()));
	}

	template<typename C>
	void PixelConstantBuffer<C>::Bind(Graphic & gfx)
	{
		GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetConstantBuffers(m_slot, 1, m_constantBuffer.GetAddressOf()));
	}
}