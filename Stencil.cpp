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
			//����ģ�����
			stencilDesc.StencilEnable = TRUE;
			//�������룬ffΪ����ԭ������λ
			stencilDesc.StencilWriteMask = 0xFF;
			//��ʾģ�����һֱͨ��
			stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			//ͨ�������ô���ȥ��ֵ�滻stencil bufferԭ����ֵ
			stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		}
		else if (mode == Mode::Mask)
		{
			stencilDesc.DepthEnable = FALSE;
			stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			stencilDesc.StencilEnable = TRUE;
			stencilDesc.StencilReadMask = 0xFF;
			//����ȿ������Ϊ���贫���ֵΪ0b1111����ǰģ��ֵΪ0b1010����ô���ս����0b0101�������ֵһ����λ�ǱȽ�ͨ����
			stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
			//��ʹͨ��Ҳ�����л���,����stencil bufferԭ����ֵ��ͨ����Ҳ��Ϊ0b1010
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
			//������Ƭ�ε����ֵ��֮ǰд���̨�����������ص����ֵСʱ�����ܸ�����Ƭ�Σ�������������Ƭ��д���̨����������������Ƭ�ε����ֵд����Ȼ�������
			stencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			//mask_zero������Ȼ�����д�빦�ܣ�mask_all������Ȼ�����д�빦��
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