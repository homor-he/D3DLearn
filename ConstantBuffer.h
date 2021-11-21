#ifndef __CONSTANTBUFFER_H__
#define __CONSTANTBUFFER_H__

#pragma once

#include "Bindable.h"

namespace Bind
{
	template<typename C>
	class ConstantBuffer : public Bindable
	{
	public:
		ConstantBuffer(Graphic & gfx, UINT startSlot = 0);
		ConstantBuffer(Graphic & gfx, C & value, UINT startSlot = 0);
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
		//using ConstantBuffer<C>::ConstantBuffer;
	public:
		void Bind(Graphic & gfx) override;
	};
	
	template<typename C>
	class PixelConstantBuffer : public ConstantBuffer<C>
	{
		using Bindable::GetContext;
		using ConstantBuffer<C>::m_slot;
		using ConstantBuffer<C>::m_constantBuffer;
	public:
		void Bind(Graphic & gfx) override;
	};
}

#endif // !__CONSTANTBUFFER_H__
