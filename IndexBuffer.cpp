#include "IndexBuffer.h"
#include "GraphicsThrowMacros.h"
#include "BindsDic.h"

namespace Bind
{
	IndexBuffer::IndexBuffer(Graphic & gfx, vector<unsigned short>& indexs) : IndexBuffer(gfx,"?",indexs)
	{
	}

	IndexBuffer::IndexBuffer(Graphic & gfx, string tag, vector<unsigned short>& indexs):
		m_tag(tag), m_Count((UINT)indexs.size())
	{
		//引入DxgiInfoManager对象
		INFOMAN(gfx);
		//创建索引缓存
		D3D11_BUFFER_DESC indexBufDesc = {};
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

	shared_ptr<IndexBuffer> IndexBuffer::Resolve(Graphic & gfx, string & tag, vector<unsigned short> & indexs)
	{
		return BindsDic::Resolve<IndexBuffer>(gfx, tag, indexs);
	}

	string IndexBuffer::GenerateUID_(string & tag)
	{
		return typeid(IndexBuffer).name() + "#"s + tag;
	}
}



