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
		IndexBuffer(Graphic & gfx, string tag, vector<unsigned short> & indexs);
		void Bind(Graphic& gfx) override;
		UINT GetCount();
		static shared_ptr<IndexBuffer> Resolve(Graphic & gfx, string & tag, vector<unsigned short> & indexs);
		template<typename...Ignore>
		static string GenerateUID(string & tag, Ignore &&... ignore)
		{
			return GenerateUID_(tag);
		}
	private:
		static string GenerateUID_(string & tag);
	protected:
		string m_tag;
		UINT m_Count;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	};
}


#endif // !__INDEXBUFFER_H__
