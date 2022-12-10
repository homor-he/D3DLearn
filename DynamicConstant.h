#ifndef __DYNAMICCONSTANT_H__
#define __DYNAMICCONSTANT_H__

#pragma once

#include <DirectXMath.h>
#include <vector>
#include <utility>
#include <memory>

#define ELEMENT_TYPES  X( Float ) X( Float2 ) X( Float3 ) X( Float4 ) X( Matrix ) X( Bool ) X( Integer )

using namespace std;

namespace Dcb
{
	enum Type
	{
		#define X(el) el,
		ELEMENT_TYPES
		#undef X
		Struct,
		Array,
		Empty,
	};

	template<Type type>
	struct Map
	{
		static constexpr bool valid = false;
	};
	template<> struct Map<Float>
	{
		using SysType = float;					//用于CPU
		static constexpr size_t hlslSize = sizeof(SysType); //大小用于GPU
		static constexpr bool valid = true;	
		static constexpr const char* code = "F1";  //code用于生成layout的签名
	};

	template<> struct Map<Float2>
	{
		using SysType = DirectX::XMFLOAT2;
		static constexpr size_t hlslSize = sizeof(SysType);
		static constexpr bool valid = true;
		static constexpr const char* code = "F2";
	};

	template<> struct Map<Float3>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr size_t hlslSize = sizeof(SysType);
		static constexpr bool valid = true;
		static constexpr const char* code = "F3";
	};

	template<> struct Map<Float4>
	{
		using SysType = DirectX::XMFLOAT4;
		static constexpr size_t hlslSize = sizeof(SysType);
		static constexpr bool valid = true;
		static constexpr const char* code = "F4";
	};

	template<> struct Map<Matrix>
	{
		using SysType = DirectX::XMMATRIX;
		static constexpr size_t hlslSize = sizeof(SysType);
		static constexpr bool valid = true;
		static constexpr const char* code = "M4";
	};

	template<> struct Map<Bool>
	{
		using SysType = bool;
		static constexpr size_t hlslSize = 4u;
		static constexpr bool valid = true;
		static constexpr const char* code = "BL";
	};

	template<> struct Map<Integer>
	{
		using SysType = int;
		static constexpr size_t hlslSize = sizeof(SysType);
		static constexpr bool valid = true;
		static constexpr const char* code = "IT";
	};

	#define X(el) static_assert(Map<el>::valid,"unknown type:"#el);
	ELEMENT_TYPES
	#undef X 

	template<typename T>
	struct ReverseMap
	{
		static constexpr bool valid = false;
	};
	#define X(el) \
	template<> struct ReverseMap<typename Map<el>::SysType> \
	{ \
		static constexpr Type type = el; \
		static constexpr bool valid = true; \
	};
	ELEMENT_TYPES
	#undef X

	//该类用于描述data buffer的层级关系
	class LayoutElement
	{
		friend struct ExtraData;
		friend class RawLayout;
	private:
		struct ExtraDataBase
		{
			virtual ~ExtraDataBase() = default;
		};
	public:
		string GetSignature() const;
		bool Exists() const;
		pair<size_t, const LayoutElement*> CalculateIndexingOffset(size_t offset, size_t index) const;
		LayoutElement& operator[](const string & key) const;
		LayoutElement & T();
		size_t GetOffsetBegin() const;
		size_t GetOffsetEnd() const;
		size_t GetSizeInBytes() const;
		LayoutElement & Add(Type addedType, string key);
		template<Type type>
		LayoutElement & Add(string key)
		{
			return Add(type, move(key));
		}

		LayoutElement & Set(Type addedType, size_t size);
		template<Type type>
		LayoutElement & Set(size_t size)
		{
			return Set(type, size);
		}

		template<typename T>
		size_t Resolve() const
		{
			switch (m_type)
			{
			#define X(el) case el: if(typeid(Map<el>::SysType) != typeid(T)){OutPutError("%s:%d\nunknown type",__FILE__,__LINE__);} return m_offset;
			ELEMENT_TYPES
			#undef X
			default:
				OutPutError("%s:%d\ntried to resolve no-leaf element", __FILE__, __LINE__);
				return 0u;
			}
		}

		LayoutElement() = default;
		LayoutElement(Type type);
	private:
		size_t Finalize(size_t offsetIn);
		string GetSignatureForStruct() const;
		string GetSignatureForArray() const;
		size_t FinalizeForStruct(size_t offsetIn);
		size_t FinalizeForArray(size_t offsetIn);
		static LayoutElement & GetEmptyElement()
		{
			static LayoutElement empty;
			return empty;
		}
		static size_t AdvanceToBoundary(size_t offset);
		static bool CrossBoundary(size_t offset, size_t size);
		static size_t AdvanceIfCrossBoundary(size_t offset, size_t size);
	private:
		size_t m_offset;
		Type m_type = Empty;
		unique_ptr<ExtraDataBase> m_pExtraData;
	};

	class Layout
	{
	public:
		size_t GetSizeInBytes();
		string GetSignature();
	protected:
		Layout(shared_ptr<LayoutElement> pRoot);
		shared_ptr<LayoutElement> m_pRoot;
	};

	//rawLayout是未确定的，可用于编辑增加其他层级节点
	class RawLayout : public Layout
	{
		friend class LayoutDic;
	public:
		RawLayout();
		LayoutElement & operator[](string & key);
		template<Type type>
		LayoutElement & Add(const string & key)
		{
			return m_pRoot->Add<type>(key);
		}
	private:
		void ClearRoot();
		shared_ptr<LayoutElement> DeliverRoot();
	};

	//cookedLayout指已经确定的层级,不可再编辑
	class CookedLayout : public Layout
	{
		friend class LayoutDic;
		friend class Buffer;
	public:
		const LayoutElement& operator[](const string & key);
		shared_ptr<LayoutElement> ShareRoot() const;
	private:
		CookedLayout(shared_ptr<LayoutElement> pRoot);
		shared_ptr<LayoutElement> RelinquishRoot();
	};

	class ElementRef
	{
		friend class Buffer;
	public:
		bool Exists();
		ElementRef operator[](const string & key) const;
		ElementRef operator[](size_t index);
		template<typename S>
		bool SetIfExists(const S & val)
		{
			if (Exists())
			{
				*this = val;
				return true;
			}
			return false;
		}

		template<typename T>
		operator T&() const
		{
			if (!ReverseMap<remove_const_t<T>>::valid)
				OutPutError("%s:%d\nunsupport SysType", __FILE__, __LINE__);
			size_t size = m_pLayout->Resolve<T>();
			return *reinterpret_cast<T*>(m_pBytes + m_offset + m_pLayout->Resolve<T>());
		}

		template<typename T>
		T& operator=(const T&  val)
		{
			if (!ReverseMap<remove_const_t<T>>::valid)
				OutPutError("%s:%d\nunsupport SysType", __FILE__, __LINE__);
			return static_cast<T&>(*this) = val;
		}
	private:
		ElementRef(const LayoutElement* pLayout, char* pBytes, size_t offset);
	private:
		size_t m_offset;
		const LayoutElement * m_pLayout;
		char* m_pBytes;
	};

	class Buffer
	{
	public:
		Buffer(RawLayout && layout);
		Buffer(const CookedLayout & layout);
		Buffer(CookedLayout && layout);
		Buffer(const Buffer & buf);
		Buffer(Buffer &&);
		ElementRef operator[](const string & key);
		const char* GetData() const;
		size_t GetSizeInBytes();
		const LayoutElement & GetRootLayoutElement() const;
		void CopyFrom(const Buffer & buffer);
		shared_ptr<LayoutElement> ShareLayoutRoot();
	public:
		shared_ptr<LayoutElement> m_pLayoutRoot;
		vector<char> m_bytes;
	};
}
#endif // !__DYNAMICCONSTANT_H__
