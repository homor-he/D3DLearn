#ifndef __PIXELSHADER_H__
#define __PIXELSHADER_H__

#pragma once

#include "Bindable.h"

namespace Bind
{
	class PixelShader : public Bindable
	{
	public:
		PixelShader(Graphic & gfx, string & fileName);
		void Bind(Graphic & gfx) override;
	protected:
		string m_path;
		Microsoft::WRL::ComPtr<ID3DBlob> m_psBlob;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>  m_pixelShader;
	};
}


#endif // !__PIXELSHADER_H__
