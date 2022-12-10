#include "DynamicConstant.h"
#include "CmnFunc.h"
#include "LayoutDic.h"

namespace Dcb
{
	struct ExtraData
	{
		struct Struct : public LayoutElement::ExtraDataBase
		{
			vector<pair<string, LayoutElement>> layoutElements;
		};
		struct Array : public LayoutElement::ExtraDataBase
		{
			LayoutElement layoutElement;
			size_t elementSize;
			size_t size;
		};
	};

	//************************************
	//layoutElement类
	//************************************
	LayoutElement::LayoutElement(Type type):m_type(type)
	{
		if (type == Empty)
		{
			OutPutError("%s:%d\ntype is empty", __FILE__, __LINE__);
		}
		if (type == Struct)
		{
			m_pExtraData = make_unique<ExtraData::Struct>();
		}
		else if (type == Array)
		{
			m_pExtraData = make_unique<ExtraData::Array>();
		}
	}

	string LayoutElement::GetSignature() const
	{
		switch (m_type)
		{
		#define X(el) case el: return Map<el>::code;
		ELEMENT_TYPES
		#undef X
		case Dcb::Struct:
			return GetSignatureForStruct();
		case Dcb::Array:
			return GetSignatureForArray();
		default:
			OutPutError("%s:%d\nunknown type in signature generation", __FILE__, __LINE__);
			return "???";
		}
	}

	bool LayoutElement::Exists() const
	{
		return m_type != Empty;
	}

	pair<size_t, const LayoutElement*> LayoutElement::CalculateIndexingOffset(size_t offset, size_t index) const
	{
		if (m_type != Array)
		{
			OutPutError("%s:%d\nindex in not array", __FILE__, __LINE__);
		}
		const auto& data = static_cast<ExtraData::Array&>(*m_pExtraData);
		if (index >= data.size)
		{
			OutPutError("%s:%d\nindex is out of range", __FILE__, __LINE__);
		}
		return make_pair<size_t, const LayoutElement*>(offset + data.elementSize * index, &data.layoutElement);
	}

	LayoutElement & LayoutElement::operator[](const string & key) const
	{
		if (m_type != Struct)
		{
			OutPutError("%s:%d\noperator key in not struct", __FILE__, __LINE__);
		}

		for (auto & mem : static_cast<ExtraData::Struct&>(*m_pExtraData).layoutElements)
		{
			if (mem.first == key)
				return mem.second;
		}
		return GetEmptyElement();
	}

	LayoutElement & LayoutElement::T()
	{
		if (m_type != Array)
		{
			OutPutError("%s:%d\nis not Array type", __FILE__, __LINE__);
		}
		return static_cast<ExtraData::Array&>(*m_pExtraData).layoutElement;
	}

	size_t LayoutElement::GetOffsetBegin() const
	{
		return m_offset;
	}

	size_t LayoutElement::GetOffsetEnd() const
	{
		switch (m_type)
		{
		#define X(el) case el: return m_offset + Map<el>::hlslSize;
		ELEMENT_TYPES
		#undef X
		case Dcb::Struct:
		{
			const ExtraData::Struct & data = static_cast<ExtraData::Struct&>(*m_pExtraData);
			return AdvanceToBoundary(data.layoutElements.back().second.GetOffsetEnd());
		}
		case Dcb::Array:
		{
			const ExtraData::Array & data = static_cast<ExtraData::Array&>(*m_pExtraData);
			return m_offset + AdvanceToBoundary(data.layoutElement.GetSizeInBytes()) * data.size;
		}
		default:
			OutPutError("%s:%d\nunknown type", __FILE__, __LINE__);
			return 0u;
		}
	}

	size_t LayoutElement::GetSizeInBytes() const
	{
		return GetOffsetEnd() - GetOffsetBegin();
	}

	LayoutElement & LayoutElement::Add(Type addedType, string key)
	{
		if (m_type != Struct)
		{
			OutPutError("%s:%d\nadd to not struct type in layout", __FILE__, __LINE__);
		}
		ExtraData::Struct & data = static_cast<ExtraData::Struct&>(*m_pExtraData);
		for (auto & mem : data.layoutElements)
		{
			if (mem.first == key)
			{
				OutPutError("%s:%d\nadd duplicate name to struct", __FILE__, __LINE__);
			}
		}
		data.layoutElements.emplace_back(move(key), LayoutElement(addedType));
		return *this;
	}

	LayoutElement & LayoutElement::Set(Type addedType, size_t size)
	{
		if(m_type != Array)
			OutPutError("%s:%d\nSet to not array type in layout", __FILE__, __LINE__);
		if(size == 0u)
			OutPutError("%s:%d\nsize is 0", __FILE__, __LINE__);
		ExtraData::Array & data = static_cast<ExtraData::Array&>(*m_pExtraData);
		data.layoutElement = LayoutElement(addedType);
		data.size = size;
		return *this;
	}

	size_t LayoutElement::Finalize(size_t offsetIn)
	{
		switch (m_type)
		{
		#define X(el) case el: m_offset = AdvanceIfCrossBoundary(offsetIn, Map<el>::hlslSize); return m_offset + Map<el>::hlslSize;
		ELEMENT_TYPES
		#undef X
		case Struct:
			return FinalizeForStruct(offsetIn);
		case Array:
			return FinalizeForArray(offsetIn);
		default:
			OutPutError("%s:%d\nunknown type", __FILE__, __LINE__);
			return 0u;
		}
	}

	string LayoutElement::GetSignatureForStruct() const
	{
		string sig = "St{";
		for (const auto& el : static_cast<ExtraData::Struct&>(*m_pExtraData).layoutElements)
		{
			sig += el.first + ":" + el.second.GetSignature() + ";";
		}
		sig += "}";
		return sig;
	}

	string LayoutElement::GetSignatureForArray() const
	{
		const auto & data = static_cast<ExtraData::Array&>(*m_pExtraData);
		return "Ar:" + to_string(data.size) + "{" + data.layoutElement.GetSignature() + "}";
	}

	size_t LayoutElement::FinalizeForStruct(size_t offsetIn)
	{
		ExtraData::Struct& data = static_cast<ExtraData::Struct&>(*m_pExtraData);
		m_offset = AdvanceToBoundary(offsetIn);
		size_t offsetNext = m_offset;
		for (auto & el : data.layoutElements)
		{
			offsetNext = el.second.Finalize(offsetNext);
		}
		return offsetNext;
	}

	size_t LayoutElement::FinalizeForArray(size_t offsetIn)
	{
		ExtraData::Array& data = static_cast<ExtraData::Array&>(*m_pExtraData);
		m_offset = AdvanceToBoundary(offsetIn);
		size_t offsetNext = m_offset;
		data.layoutElement.Finalize(offsetIn);
		data.elementSize = AdvanceToBoundary(data.layoutElement.GetSizeInBytes());
		return GetOffsetEnd();
	}

	size_t LayoutElement::AdvanceToBoundary(size_t offset)
	{
		//在offset上扩充16位,4字节
		return offset + 16u - offset % 16;
	}

	bool LayoutElement::CrossBoundary(size_t offset, size_t size)
	{
		size_t end = offset + size;
		size_t pageStart = offset / 16;
		size_t pageEnd = end / 16;
		return ((pageStart != pageEnd) && (end % 16 != 0)) || size > 16;
	}

	size_t LayoutElement::AdvanceIfCrossBoundary(size_t offset, size_t size)
	{
		return CrossBoundary(offset,size) ? AdvanceToBoundary(offset) : offset;
	}


	//************************************
	//layout类
	//************************************
	size_t Layout::GetSizeInBytes()
	{
		return m_pRoot->GetSizeInBytes();
	}

	string Layout::GetSignature()
	{
		return m_pRoot->GetSignature();
	}

	Layout::Layout(shared_ptr<LayoutElement> pRoot) : m_pRoot(move(pRoot))
	{
	}

	//************************************
	//rawLayout类
	//************************************
	RawLayout::RawLayout() : Layout(make_shared<LayoutElement>(Struct))
	{
	}

	LayoutElement & RawLayout::operator[](string & key)
	{
		return (*m_pRoot)[key];
	}

	void RawLayout::ClearRoot()
	{
		*this = RawLayout();
	}

	shared_ptr<LayoutElement> RawLayout::DeliverRoot()
	{
		shared_ptr<LayoutElement> temp = move(m_pRoot);
		temp->Finalize(0);
		*this = RawLayout();
		return move(temp);
	}

	//************************************
	//cookedLayout类
	//************************************
	CookedLayout::CookedLayout(shared_ptr<LayoutElement> pRoot) : Layout(move(pRoot))
	{
	}

	shared_ptr<LayoutElement> CookedLayout::RelinquishRoot()
	{
		return move(m_pRoot);
	}

	const LayoutElement & CookedLayout::operator[](const string & key)
	{
		return (*m_pRoot)[key];
	}

	shared_ptr<LayoutElement> CookedLayout::ShareRoot() const
	{
		return m_pRoot;
	}

	//************************************
	//buffer类
	//************************************
	Buffer::Buffer(RawLayout && layout) : Buffer(LayoutDic::Resolve(move(layout)))
	{}

	Buffer::Buffer(const CookedLayout & layout) : m_pLayoutRoot(layout.ShareRoot()), m_bytes(m_pLayoutRoot->GetOffsetEnd())
	{}

	Buffer::Buffer(CookedLayout && layout) : m_pLayoutRoot(layout.RelinquishRoot()), m_bytes(m_pLayoutRoot->GetOffsetEnd())
	{}

	Buffer::Buffer(const Buffer & buf) : m_pLayoutRoot(buf.m_pLayoutRoot), m_bytes(buf.m_bytes)
	{}

	Buffer::Buffer(Buffer && buf) : m_pLayoutRoot(move(buf.m_pLayoutRoot)),m_bytes(move(buf.m_bytes))
	{}

	ElementRef Buffer::operator[](const string & key)
	{
		return ElementRef(&(*m_pLayoutRoot)[key], m_bytes.data(), 0u);
	}

	const char * Buffer::GetData() const
	{
		return m_bytes.data();
	}

	size_t Buffer::GetSizeInBytes()
	{
		return m_bytes.size();
	}

	void Buffer::CopyFrom(const Buffer & buffer)
	{
		if (&GetRootLayoutElement() != &buffer.GetRootLayoutElement())
		{
			OutPutError("%s:%d\nnot point to same addr", __FILE__, __LINE__);
		}
		copy(buffer.m_bytes.begin(), buffer.m_bytes.end(), m_bytes.begin());
	}

	shared_ptr<LayoutElement> Buffer::ShareLayoutRoot()
	{
		return m_pLayoutRoot;
	}

	const LayoutElement & Buffer::GetRootLayoutElement() const
	{
		return *m_pLayoutRoot;
	}

	//************************************
	//elementref类
	//************************************
	bool ElementRef::Exists()
	{
		return m_pLayout->Exists();
	}

	ElementRef ElementRef::operator[](const string & key) const
	{
		return ElementRef(&(*m_pLayout)[key], m_pBytes, m_offset);
	}

	ElementRef ElementRef::operator[](size_t index)
	{
		const pair<size_t, const LayoutElement*> pairObj = m_pLayout->CalculateIndexingOffset(m_offset, index);
		return ElementRef(pairObj.second, m_pBytes, pairObj.first);
	}

	ElementRef::ElementRef(const LayoutElement* pLayout, char* pBytes, size_t offset) : 
		m_offset(offset), m_pLayout(pLayout), m_pBytes(pBytes)
	{}
}


