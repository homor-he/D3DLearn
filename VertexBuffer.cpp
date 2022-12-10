#include "VertexBuffer.h"
#include "GraphicsThrowMacros.h"
#include "BindsDic.h"

namespace Bind
{
	VertexBuffer::VertexBuffer(Graphic & gfx, VertexRela::VertexBuffer & vBuf):
		VertexBuffer(gfx, "?", vBuf)
	{
	}

	VertexBuffer::VertexBuffer(Graphic & gfx, string tag, VertexRela::VertexBuffer & vBuf, bool cpuAccess) :
		m_tag(tag), m_stride((UINT)vBuf.GetLayout().GetSize()), m_layout(vBuf.GetLayout())
	{
		//引入DxgiInfoManager对象
		INFOMAN(gfx);
		//创建顶点缓存
		D3D11_BUFFER_DESC vertexBufDesc = {};
		if (!cpuAccess)
		{
			vertexBufDesc.ByteWidth = (UINT)vBuf.SizeBytes();
			vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufDesc.CPUAccessFlags = 0;
			vertexBufDesc.MiscFlags = 0;
			vertexBufDesc.StructureByteStride = m_stride;

			D3D11_SUBRESOURCE_DATA vertexInitData;
			vertexInitData.pSysMem = vBuf.GetData();
			GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&vertexBufDesc, &vertexInitData, &m_vertexBuffer));

		}
		else
		{
			vertexBufDesc.ByteWidth = (UINT)vBuf.SizeBytes();
			vertexBufDesc.Usage = D3D11_USAGE_DYNAMIC;
			vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			vertexBufDesc.MiscFlags = 0;
			GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&vertexBufDesc, 0, &m_vertexBuffer));
		}
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

	shared_ptr<VertexBuffer> VertexBuffer::Resolve(Graphic & gfx, string & tag, VertexRela::VertexBuffer & vBuf, bool cpuAccess)
	{
		return BindsDic::Resolve<VertexBuffer>(gfx, tag, vBuf, cpuAccess);
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer::GetD3DVtxBuffer()
	{
		return m_vertexBuffer;
	}

	string VertexBuffer::GenerateUID_(string & tag)
	{
		return typeid(VertexBuffer).name() + "#"s + tag;
	}
}


