#include "GeometryInLightCase.h"

namespace DrawTest
{
	void GeometryInLightCase::Init(Graphic & gfx)
	{
		m_pixelObjectCBuf = {
			{ 1.0f, 0.0f,0.0f },
			{ 1.0f,1.0f,1.0f },
			0.3f,1.0f,
		};

		m_cube = make_shared<Cuboid>(gfx, 1.0f,GeometryType::Common);
		m_cubeTransfromBuf = make_shared<Bind::TransformCbuf>(gfx);
		m_cubeTransfromBuf->InitializeParentRef(*m_cube);

		string vtxShaderName = "Phong_VS.cso";
		m_vtxShader = Bind::VertexShader::Resolve(gfx, vtxShaderName);

		string pxShaderName = "Phong_PS.cso";
		m_pxShader = Bind::PixelShader::Resolve(gfx, pxShaderName);

		VertexRela::VertexLayout m_vtLayout;
		m_vtLayout.Append(VertexRela::VertexLayout::Position3D);
		m_vtLayout.Append(VertexRela::VertexLayout::Normal);
		//m_vtLayout.Append(VertexRela::VertexLayout::Tangent);
		m_vtLayout.Append(VertexRela::VertexLayout::Texture2D);
		m_inputLayout = Bind::InputLayout::Resolve(gfx, m_vtLayout, *m_vtxShader);
	}

	void GeometryInLightCase::Draw(Graphic & gfx, float dt)
	{
		m_inputLayout->Bind(gfx);
		m_vtxShader->Bind(gfx);
		m_pxShader->Bind(gfx);
		m_cube->Bind(gfx);
		m_cubeTransfromBuf->Bind(gfx);
		Bind::PixelConstantBuffer<ObjectCBuf>::Resolve(gfx, m_pixelObjectCBuf, 1u)->Bind(gfx);
		gfx.DrawIndex(m_cube->GetIndexCount());
	}
}