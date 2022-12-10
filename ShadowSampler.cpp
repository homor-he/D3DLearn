#include "ShadowSampler.h"
#include "GraphicsThrowMacros.h"

namespace Bind
{
	ShadowSampler::ShadowSampler(Graphic & gfx)
	{
		for (int i = 0; i < 4; ++i)
		{
			//m_curSampler=0 ��ʹ����Ӱ��ͼ ʹ�õ���� slot=2
			//=1 ��ʹ����Ӱ��ͼ��ʹ�����Բ��� slot=2
			//=2 ʹ����Ӱ��ͼ��ʹ�õ���� slot = 1
			//=3 ʹ����Ӱ��ͼ��ʹ�����Բ��� slot = 1
			m_curSampler = i;
			m_samplers[i] = MakeSampler(gfx, GetBilinear(), GetHwPcf());
		}
		SetBilinear(true);
		SetHwPcf(false);
	}

	void ShadowSampler::Bind(Graphic & gfx)
	{
		UINT slot = GetCurrentSlot();
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetSamplers(slot, 1, m_samplers[m_curSampler].GetAddressOf()));
	}

	bool ShadowSampler::GetHwPcf()
	{
		//index=2��3 �ŷ���true  0��1����false
		return m_curSampler & 0b10;
	}

	bool ShadowSampler::GetBilinear()
	{
		//index=1��3����true  0��2����false
		return m_curSampler & 0b01;
	}

	void ShadowSampler::SetBilinear(bool bilin)
	{
		//��λ��ʾ�Ƿ�����Ӱ��ͼ����λ��ʾ�ǵ�����������Բ���
		m_curSampler = (m_curSampler & ~0b01) | (bilin ? 0b01 : 0);
	}

	void ShadowSampler::SetHwPcf(bool hwPcf)
	{
		m_curSampler = (m_curSampler & ~0b10) | (hwPcf ? 0b10 : 0);
	}

	UINT ShadowSampler::GetCurrentSlot()
	{
		return GetHwPcf() ? 1 : 2;
	}

	Microsoft::WRL::ComPtr<ID3D11SamplerState> ShadowSampler::MakeSampler(Graphic & gfx, bool bilin, bool hwPcf)
	{
		INFOMAN(gfx);

		D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC(CD3D11_DEFAULT());
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;

		if (hwPcf)
		{
			samplerDesc.Filter = bilin ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR : D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
			//compares sampled data against existing sampled data  �Ƚϲ�������Ҫ������Ӱ��ͼ
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		}
		else
		{
			samplerDesc.Filter = bilin ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR : D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
		}

		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSamplerStat;
		GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSamplerStat));
		return move(pSamplerStat);
	}
}



