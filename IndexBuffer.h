#ifndef __INDEXBUFFER_H__
#define __INDEXBUFFER_H__

#pragma once

#include "Bindable.h"

namespace Bind
{
	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer(Graphic & gfx, vector<unsigned short> & indexs);
		void Bind(Graphic& gfx) override;
		UINT GetCount();
	protected:
		UINT m_Count;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	};
}


#endif // !__INDEXBUFFER_H__
