#include "SkyboxTransformCbuf.h"

namespace Bind
{
	SkyboxTransformCbuf::SkyboxTransformCbuf(Graphic & gfx, UINT slot)
	{
		if (!m_vtxBuf)
			m_vtxBuf = make_unique<VertexConstantBuffer<Transform>>(gfx, slot);
	}

	void SkyboxTransformCbuf::Bind(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(UpdateBind(gfx, GetTransform(gfx)));
	}

	void SkyboxTransformCbuf::UpdateBind(Graphic & gfx, SkyboxTransformCbuf::Transform & tf)
	{
		m_vtxBuf->Update(gfx, tf);
		m_vtxBuf->Bind(gfx);
	}

	SkyboxTransformCbuf::Transform SkyboxTransformCbuf::GetTransform(Graphic & gfx)
	{
		return { DirectX::XMMatrixTranspose(gfx.GetCamera() * gfx.GetProjection()) };
	}

}