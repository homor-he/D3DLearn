#include "Surface.h" 

#pragma comment(lib,"DirectXTex.lib")

Surface::Surface(int width, int height)
{
	HRESULT hr = m_scratch.Initialize2D(s_format, width, height, 1, 1);
	if (FAILED(hr))
	{
		OutPutError("%s:%d\nFailed to initialize ScratchImage\n *Error string:\n%s",
			__FILE__, __LINE__, TranslateErrorCode(hr).c_str());
	}
}

Surface::Surface(DirectX::ScratchImage scratch): 
	m_scratch(move(scratch))
{
}

Surface Surface::FromFile(const std::string & name)
{
	DirectX::ScratchImage scratch;
    HRESULT hr = DirectX::LoadFromWICFile(String2WString(name).c_str(),
		DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, scratch);
	if (FAILED(hr))
	{
		OutPutError("%s:%d\nFailed to load ScratchImage:%s\n *Error string:\n%s",
			__FILE__, __LINE__,name.c_str(), TranslateErrorCode(hr).c_str());
		return Surface();
	}

	if (scratch.GetImage(0, 0, 0)->format != s_format)
	{
		DirectX::ScratchImage converted;
		hr = DirectX::Convert(*scratch.GetImage(0, 0, 0), s_format, DirectX::TEX_FILTER_DEFAULT,
			DirectX::TEX_THRESHOLD_DEFAULT, converted);
		if (FAILED(hr))
		{
			OutPutError("%s:%d\nFailed to convert image:%s\n *Error string:\n%s",
				__FILE__, __LINE__, name.c_str(), TranslateErrorCode(hr).c_str());
		}
		return Surface(move(converted));
	}
	return Surface(move(scratch));
}

bool Surface::AlphaLoaded()
{
	return !m_scratch.IsAlphaAllOpaque();
}

UINT Surface::GetWidth()
{
	return (UINT)m_scratch.GetMetadata().width;
}

UINT Surface::GetHeight()
{
	return (UINT)m_scratch.GetMetadata().height;
}

Surface::Color * Surface::GetBufferPtr()
{
	return reinterpret_cast<Surface::Color *>(m_scratch.GetPixels());
}

UINT Surface::GetBufferSize()
{
	return (UINT)m_scratch.GetPixelsSize();
}

UINT Surface::GetBytePitch()
{
	return (UINT)m_scratch.GetImage(0, 0, 0)->rowPitch;
}

void Surface::Save(const string & fileName)
{
	DirectX::WICCodecs format = DirectX::WIC_CODEC_BMP;
	size_t pos = fileName.rfind('.');
	string suffix = fileName.substr(pos, fileName.size() - pos);
	if (suffix == ".png")
	{
		format = DirectX::WIC_CODEC_PNG;
	}
	else if (suffix == ".jpg")
	{
		format = DirectX::WIC_CODEC_JPEG;
	}
	else if (suffix == ".bmp")
	{
		format = DirectX::WIC_CODEC_BMP;
	}
	else
	{
		OutPutError("%s:%d\nImage format not supported", __FILE__, __LINE__);
		return;
	}
	//存储到特定文件名
	HRESULT hr = DirectX::SaveToWICFile(*m_scratch.GetImage(0, 0, 0), DirectX::WIC_FLAGS_NONE, DirectX::GetWICCodec(format), String2WString(fileName).c_str());
	if (FAILED(hr))
	{
		OutPutError("%s:%d\nFailed to save image", __FILE__, __LINE__);
	}
}

void Surface::PutPixel(unsigned int x, unsigned int y, Color c)
{
	const DirectX::Image& imgData = *m_scratch.GetImage(0, 0, 0);
	reinterpret_cast<Color*>(&imgData.pixels[y*imgData.rowPitch])[x] = c;
}
