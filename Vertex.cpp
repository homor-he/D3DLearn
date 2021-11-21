#include "Vertex.h"

namespace VertexRela
{

#pragma region VertexLayout
	VertexLayout::Element::Element(ElementType type, size_t offset)
	{
		m_type = type;
		m_offset = offset;
	}

	size_t VertexLayout::Element::GetOffsetAfter()
	{
		return GetOffset() + Size();
	}

	size_t VertexLayout::Element::GetOffset() 
	{
		return m_offset;
	}

	size_t VertexLayout::Element::Size() 
	{
		return SizeOf(m_type);
	}

	VertexLayout::ElementType VertexLayout::Element::GetType() 
	{
		return m_type;
	}

	template<VertexLayout::ElementType type>
	struct SysSize
	{
		static auto Exec()
		{
			return sizeof(VertexLayout::Map<type>::SysType);
		}
	};

	size_t VertexLayout::Element::SizeOf(ElementType type)
	{
		return Bridge<SysSize>(type);
	}

	template<VertexLayout::ElementType type>
	struct DescGenerate
	{
		static D3D11_INPUT_ELEMENT_DESC Exec(size_t offset)
		{
			return
			{ VertexLayout::Map<type>::s_semantic, 0,  VertexLayout::Map<type>::s_dxgiFormat,
			0, offset,D3D11_INPUT_PER_VERTEX_DATA,0 };
			
		}
	};

	D3D11_INPUT_ELEMENT_DESC VertexLayout::Element::GetDesc()
	{
		return Bridge<DescGenerate>(m_type, m_offset);
	}

	size_t VertexLayout::GetSize()
	{
		return m_elements.empty() ? 0 : m_elements.back().GetOffsetAfter();
	}

	size_t VertexLayout::SizeBytes()
	{
		return m_elements.size() * m_elements.back().Size();
	}

	vector<D3D11_INPUT_ELEMENT_DESC> VertexLayout::GetLayoutDesc()
	{
		vector<D3D11_INPUT_ELEMENT_DESC> sDesc;
		for (size_t i = 0; i < m_elements.size(); ++i)
		{
			m_elements.at(i).GetDesc();
		}

		for(Element & var : m_elements)
		{
			sDesc.push_back(var.GetDesc());
		}
		return sDesc;
	}
#pragma endregion


#pragma region Vertex
	Vertex::Vertex(char * pData, VertexLayout & layout) :
		m_pData(pData), m_layout(layout)
	{
	}
#pragma endregion


#pragma region VertexBuffer
	VertexBuffer::VertexBuffer(VertexLayout layout, size_t size)
	{
	}

	const char * VertexBuffer::GetData()
	{
		return m_buffer.data();
	}

	VertexLayout & VertexBuffer::GetLayout() 
	{
		return m_layout;
	}

	size_t VertexBuffer::GetSize()
	{
		return m_layout.GetSize();
	}

	size_t VertexBuffer::SizeBytes()
	{
		return m_buffer.size();
	}
#pragma endregion

	

}


