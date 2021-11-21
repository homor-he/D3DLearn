#ifndef __VERTEX_H__
#define __VERTEX_H__

#pragma once

#include "Graphic.h"
#include "CmnFunc.h"

struct BGRAColor
{
	unsigned char a;
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

namespace VertexRela
{
	class VertexLayout
	{
	public:
		enum ElementType
		{
			Position2D,
			Position3D,
			Texture2D,
			Normal,
			Tangent,
			Bitangent,
			Float3Color,
			Float4Color,
			BGRAColor,
			Count,
		};

		template<ElementType type> 
		struct Map;
		//tempelate<>标识函数模板特化
		template<> struct Map<Position2D>
		{
			typedef DirectX::XMFLOAT2 SysType;
			static const DXGI_FORMAT s_dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char*  s_semantic  = "Position";
		};

		template<> struct Map<Position3D>
		{
			typedef DirectX::XMFLOAT3 SysType;
			static const DXGI_FORMAT s_dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char*  s_semantic = "Position";
		};

		template<> struct Map<Texture2D>
		{
			typedef DirectX::XMFLOAT2 SysType;
			static const DXGI_FORMAT s_dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char*  s_semantic = "Texcoord";
		};

		template<> struct Map<Normal>
		{
			typedef DirectX::XMFLOAT3 SysType;
			static const DXGI_FORMAT s_dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char*  s_semantic = "Normal";
		};

		template<> struct Map<Tangent>
		{
			typedef DirectX::XMFLOAT3 SysType;
			static const DXGI_FORMAT s_dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char*  s_semantic = "Tangent";
		};

		template<> struct Map<Bitangent>
		{
			typedef DirectX::XMFLOAT3 SysType;
			static const DXGI_FORMAT s_dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char*  s_semantic = "Bitangent";
		};

		template<> struct Map<Float3Color>
		{
			typedef DirectX::XMFLOAT3 SysType;
			static const DXGI_FORMAT s_dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char*  s_semantic = "Color";
		};

		template<> struct Map<Float4Color>
		{
			typedef DirectX::XMFLOAT4 SysType;
			static const DXGI_FORMAT s_dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
			static constexpr const char*  s_semantic = "Color";
		};

		template<> struct Map<BGRAColor>
		{
			typedef ::BGRAColor SysType;
			static const DXGI_FORMAT s_dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			static constexpr const char*  s_semantic = "Color";
		};

		template<> struct Map<Count>
		{
			typedef long double SysType;
			static const DXGI_FORMAT s_dxgiFormat = DXGI_FORMAT_UNKNOWN;
			static constexpr const char*  s_semantic = "!INVALID!";
		};

		//模板函数中套了模板类
		template<template<ElementType> class F, typename ... Args>
		static auto Bridge(ElementType type, Args&&... args)
		{
			switch (type)
			{
			case VertexRela::VertexLayout::Position2D:
				return F<VertexLayout::Position2D>::Exec(std::forward<Args>(args)...);
			case VertexRela::VertexLayout::Position3D:
				return F<VertexLayout::Position3D>::Exec(std::forward<Args>(args)...);
			case VertexRela::VertexLayout::Texture2D:
				return F<VertexLayout::Texture2D>::Exec(std::forward<Args>(args)...);
			case VertexRela::VertexLayout::Normal:
				return F<VertexLayout::Normal>::Exec(std::forward<Args>(args)...);
			case VertexRela::VertexLayout::Tangent:
				return F<VertexLayout::Tangent>::Exec(std::forward<Args>(args)...);
			case VertexRela::VertexLayout::Bitangent:
				return F<VertexLayout::Bitangent>::Exec(std::forward<Args>(args)...);
			case VertexRela::VertexLayout::Float3Color:
				return F<VertexLayout::Float3Color>::Exec(std::forward<Args>(args)...);
			case VertexRela::VertexLayout::Float4Color:
				return F<VertexLayout::Float4Color>::Exec(std::forward<Args>(args)...);
			case VertexRela::VertexLayout::BGRAColor:
				return F<VertexLayout::BGRAColor>::Exec(std::forward<Args>(args)...);
			default:
				break;
			}
			OutPutError("%s:%d\nInvalid element type", __FILE__, __LINE__);
			return F<VertexLayout::Count>::Exec(std::forward<Args>(args)...);
		}

		class Element
		{
		public: 
			Element(ElementType type, size_t offset);
			size_t GetOffsetAfter();
			size_t GetOffset();
			size_t Size();
			ElementType GetType();
			static size_t SizeOf(ElementType type);
			D3D11_INPUT_ELEMENT_DESC GetDesc();
		private:
			ElementType m_type;
			size_t m_offset;
		};

	public:
		size_t GetSize();
		size_t SizeBytes();
		vector<D3D11_INPUT_ELEMENT_DESC> GetLayoutDesc();
	private:
		std::vector<Element> m_elements;
	};


	class Vertex
	{
		friend class VertexBuffer;
	protected:
		Vertex(char* pData, VertexLayout& layout);
	private:
		char* m_pData = nullptr;
		VertexLayout & m_layout;
	};


	class VertexBuffer
	{
	public:
		VertexBuffer(VertexLayout layout, size_t size = 0U);
		const char* GetData();
		VertexLayout& GetLayout();
		size_t GetSize();
		size_t SizeBytes();
	private:
		vector<char> m_buffer;
		VertexLayout m_layout;
	};

}

#endif // !__VERTEX_H__
