#include "NullPixelShader.h"
#include "GraphicsThrowMacros.h"
#include "BindsDic.h"

namespace Bind
{
	NullPixelShader::NullPixelShader(Graphic & gfx)
	{
	}

	void NullPixelShader::Bind(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetShader(nullptr, nullptr, 0u));
	}

	shared_ptr<NullPixelShader> NullPixelShader::Resolve(Graphic & gfx)
	{
		return BindsDic::Resolve<NullPixelShader>(gfx);
	}

	string NullPixelShader::GenerateUID()
	{
		return typeid(NullPixelShader).name();
	}
}