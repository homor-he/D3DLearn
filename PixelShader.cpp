#include "PixelShader.h"
#include "GraphicsThrowMacros.h"
#include "d3dcompiler.h"

Bind::PixelShader::PixelShader(Graphic & gfx, string & fileName):
	m_path(fileName)
{
	INFOMAN(gfx);
	GFX_THROW_INFO(D3DReadFileToBlob(String2WString(fileName).c_str(), &m_psBlob));
	GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(m_psBlob->GetBufferPointer(), 
		m_psBlob->GetBufferSize(), nullptr, &m_pixelShader));
}

void Bind::PixelShader::Bind(Graphic & gfx)
{
	INFOMAN_NOHR(gfx);
	GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetShader(m_pixelShader.Get(), nullptr, 0));
}
