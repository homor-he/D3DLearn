#ifndef __NULLPIXELSHADER_H__
#define __NULLPIXELSHADER_H__

#pragma once

#include "Bindable.h"

namespace Bind
{
	class NullPixelShader : public Bindable
	{
	public:
		NullPixelShader(Graphic & gfx);
		void Bind(Graphic & gfx) override;
		static shared_ptr<NullPixelShader> Resolve(Graphic & gfx);
		static string GenerateUID();
	};
}



#endif // !__NULLPIXELSHADER_H__
