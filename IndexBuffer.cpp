#include "IndexBuffer.h"
#include "GraphicsThrowMacros.h"

namespace Bind
{
	IndexBuffer::IndexBuffer(Graphic & gfx, vector<unsigned short>& indexs) :
		m_Count(indexs.size())
	{
		//引入DxgiInfoManager对象
		INFOMAN(gfx);
		//创建索引缓存
		D3D11_BUFFER_DESC indexBufDesc;
		indexBufDesc.ByteWidth = m_Count * sizeof(unsigned short);
		indexBufDesc.Usage = D3D11_USAGE_DEFAULT;;
		indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufDesc.CPUAccessFlags = 0;
		indexBufDesc.MiscFlags = 0;
		indexBufDesc.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA indexInitData;
		indexInitData.pSysMem = indexs.data();
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&indexBufDesc, &indexInitData, &m_indexBuffer));
	}

	void IndexBuffer::Bind(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0));
	}

	UINT IndexBuffer::GetCount()
	{
		return m_Count;
	}
}



