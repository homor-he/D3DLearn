#include "Stencil.h"
#include "GraphicsThrowMacros.h"
#include "BindsDic.h"

namespace Bind
{
	Stencil::Stencil(Graphic & gfx, Mode mode):
		m_mode(mode)
	{
		D3D11_DEPTH_STENCIL_DESC stencilDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };
		if (mode == Mode::Write)
		{
			stencilDesc.DepthEnable = FALSE;
			stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			//开启模板测试
			stencilDesc.StencilEnable = TRUE;
			//设置掩码，ff为保留原来所有位
			stencilDesc.StencilWriteMask = 0xFF;
			//表示模板测试一直通过
			stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			//通过后则用传进去的值替换stencil buffer原来的值
			stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		}
		else if (mode == Mode::Mask)
		{
			stencilDesc.DepthEnable = FALSE;
			stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			stencilDesc.StencilEnable = TRUE;
			stencilDesc.StencilReadMask = 0xFF;
			//不相等可以理解为假设传入的值为0b1111，当前模板值为0b1010，那么最终结果是0b0101，传入的值一、三位是比较通过的
			stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
			//即使通过也不进行绘制,保留stencil buffer原来的值，通过后也仍为0b1010
			stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		}
		else if (mode == Mode::DepthOff)
		{
			stencilDesc.DepthEnable = FALSE;
			stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		}
		else if (mode == Mode::DepthReversed)
		{
			stencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;
		}
		else if (mode == Mode::DepthFirst)
		{
			//当像素片段的深度值比之前写入后台缓冲区的像素的深度值小时，接受该像素片段（即，将该像素片段写入后台缓冲区，将该像素片段的深度值写入深度缓冲区）
			stencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			//mask_zero禁用深度缓冲区写入功能，mask_all启用深度缓冲区写入功能
			stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		}
		GetDevice(gfx)->CreateDepthStencilState(&stencilDesc, &m_pStencil);
	}

	void Stencil::Bind(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->OMSetDepthStencilState(m_pStencil.Get(), 0xFF));
	}

	shared_ptr<Stencil> Stencil::Resolve(Graphic & gfx, Mode mode)
	{
		return BindsDic::Resolve<Stencil>(gfx, mode);
	}

	string Stencil::GenerateUID(Mode mode)
	{
		auto modeName = [](Mode mode)
		{
			switch (mode) 
			{
			case Mode::Off:
				return "off"s;
			case Mode::Write:
				return "write"s;
			case Mode::Mask:
				return "mask"s;
			case Mode::DepthOff:
				return "depth-off"s;
			case Mode::DepthReversed:
				return "depth-reversed"s;
			case Mode::DepthFirst:
				return "depth-first"s;
			}
			return "ERROR"s;
		}(mode);
		return typeid(Stencil).name() + "#"s + modeName;
	}
}