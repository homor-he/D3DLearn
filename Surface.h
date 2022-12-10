#ifndef __SURFACE_H__
#define __SURFACE_H__

#pragma once

#include "CmnFunc.h"
#include <DirectXTex.h>

class Surface
{
public:
	class Color
	{
	public:
		UINT m_dword;
		Color(UINT dword):m_dword(dword){}
		Color(UCHAR x, UCHAR r, UCHAR g, UCHAR b) :m_dword((x << 24) | (r << 16) | (g << 8) | b) {}
		Color(UCHAR r, UCHAR g, UCHAR b) :m_dword((255 << 24) | (r << 16) | (g << 8) | b) {}
		Color(Color col, UCHAR x) :m_dword((x << 24) | col.m_dword) {}

		UCHAR GetX()
		{
			return m_dword >> 24;
		}

		UCHAR GetA()
		{
			return GetX();
		}

		UCHAR GetR()
		{
			return (m_dword >> 16) & 0xFF;
		}

		UCHAR GetG()
		{
			return (m_dword >> 8) & 0xFF;
		}

		UCHAR GetB()
		{
			return m_dword & 0xFF;
		}

		void SetX(UCHAR x)
		{
			m_dword = (m_dword & 0xFFFFFF) | (x << 24);
		}

		void SetA(UCHAR a)
		{
			SetX(a);
		}

		void SetR(UCHAR r)
		{
			m_dword = (m_dword & 0xFF00FFFF) | (r << 16);
		}

		void SetG(UCHAR g)
		{
			m_dword = (m_dword & 0xFFFF00FF) | (g << 8);
		}

		void SetB(UCHAR b)
		{
			m_dword = (m_dword & 0xFFFFFF00) | b;
		}
	};
public:
	Surface() = default;
	Surface(int width, int height);
	static Surface FromFile(const std::string& name);
	bool AlphaLoaded();
	UINT GetWidth();
	UINT GetHeight();
	Color* GetBufferPtr();
	UINT GetBufferSize();
	UINT GetBytePitch();
	void Save(const string & fileName);
	void PutPixel(unsigned int x, unsigned int y, Color c);
private:
	Surface(DirectX::ScratchImage scratch);
private:
	DirectX::ScratchImage m_scratch;
	static const DXGI_FORMAT s_format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
};

#endif // !__SURFACE_H__
