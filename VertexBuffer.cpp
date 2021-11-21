#include "VertexBuffer.h"
#include "GraphicsThrowMacros.h"

namespace Bind
{
	VertexBuffer::VertexBuffer(Graphic & gfx, VertexRela::VertexBuffer & vBuf):
		m_stride(vBuf.GetLayout().GetSize()), m_layout(vBuf.GetLayout())
	{
		//引入DxgiInfoManager对象
		INFOMAN(gfx);
		//创建顶点缓存
		D3D11_BUFFER_DESC vertexBufDesc;
		vertexBufDesc.ByteWidth = vBuf.SizeBytes();
		vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;;
		vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufDesc.CPUAccessFlags = 0;
		vertexBufDesc.MiscFlags = 0;
		vertexBufDesc.StructureByteStride = m_stride;
		D3D11_SUBRESOURCE_DATA vertexInitData;
		vertexInitData.pSysMem = vBuf.GetData();
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&vertexBufDesc, &vertexInitData, &m_vertexBuffer));
	}

	void VertexBuffer::Bind(Graphic & gfx)
	{
		UINT offset = 0;
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &m_stride, &offset));
	}

	VertexRela::VertexLayout & VertexBuffer::GetLayout()
	{
		return m_layout;
	}
}


