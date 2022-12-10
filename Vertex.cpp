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

	size_t VertexLayout::Element::GetOffset() const
	{
		return m_offset;
	}

	size_t VertexLayout::Element::Size() 
	{
		return SizeOf(m_type);
	}

	VertexLayout::ElementType VertexLayout::Element::GetType() const
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
			0, (UINT)offset,D3D11_INPUT_PER_VERTEX_DATA,0 };
		}
	};

	D3D11_INPUT_ELEMENT_DESC VertexLayout::Element::GetDesc()
	{
		return Bridge<DescGenerate>(m_type, m_offset);
	}


	template<VertexLayout::ElementType type>
	struct CodeFilter
	{
		static const char* Exec()
		{
			return VertexLayout::Map<type>::code;
		}
	};

	const char* VertexLayout::Element::GetCode()
	{
		return Bridge<CodeFilter>(m_type);
	}

	size_t VertexLayout::GetSize()
	{
		return m_elements.empty() ? 0 : m_elements.back().GetOffsetAfter();
	}

	size_t VertexLayout::SizeBytes()
	{
		return m_elements.size() * m_elements.back().Size();
	}

	VertexLayout & VertexLayout::Append(ElementType type)
	{
		if (!Has(type))
		{
			m_elements.emplace_back(type,GetSize());
		}
		return *this;
	}

	const VertexLayout::Element & VertexLayout::FindElementByIndex(size_t i)
	{
		return m_elements[i];
	}

	size_t VertexLayout::GetElementCnt()
	{
		return m_elements.size();
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

	bool VertexLayout::Has(ElementType type) const 
	{
		for (auto & element : m_elements)
		{
			if (type == element.GetType())
			{
				return true;
			}
		}
		return false;
	}

	string VertexLayout::GetCode()
	{
		string code;
		for (auto & e : m_elements)
		{
			code += e.GetCode();
		}
		return code;
	}

#pragma endregion


#pragma region Vertex
	Vertex::Vertex(char * pData, VertexLayout & layout) :
		m_pData(pData), m_layout(layout)
	{
	}
#pragma endregion


#pragma region VertexBuffer
	VertexBuffer::VertexBuffer(VertexLayout layout, size_t size):
		m_layout(move(layout))
	{
		SetBufferSize(size);
	}

	template<VertexLayout::ElementType type>
	struct AttributeAiMeshFill
	{
		template<typename ... Params>
		static auto Exec(VertexBuffer * pBuf, aiMesh & mesh)
		{
			size_t vertexSize = mesh.mNumVertices;
			for (size_t i = 0; i < vertexSize; ++i)
			{
				(*pBuf)[i].Attr<type>() = VertexLayout::Map<type>::Extract(mesh, i);
			}
		}
	};

	VertexBuffer::VertexBuffer(VertexLayout layout, aiMesh & mesh):
		m_layout(move(layout))
	{
		SetBufferSize(mesh.mNumVertices);
		size_t elementSize = m_layout.GetElementCnt();
		for (size_t i = 0; i < elementSize; ++i)
		{
			VertexLayout::Bridge<AttributeAiMeshFill>(m_layout.FindElementByIndex(i).GetType(), this, mesh);
		}
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
		return m_buffer.size() / m_layout.GetSize();
	}

	size_t VertexBuffer::SizeBytes()
	{
		return m_buffer.size();
	}

	void VertexBuffer::SetBufferSize(size_t newSize)
	{
		size_t size = GetSize();
		if (newSize > size)
		{
			m_buffer.resize(m_buffer.size() + m_layout.GetSize() * (newSize - size));
		}
	}

	Vertex VertexBuffer::Back()
	{
		return Vertex{ m_buffer.data() + m_buffer.size() - m_layout.GetSize(),m_layout };
	}

	Vertex VertexBuffer::operator[](size_t i)
	{
		return Vertex{ m_buffer.data() + m_layout.GetSize() * i,m_layout };
	}
#pragma endregion

	

}


