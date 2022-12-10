#include "SeveralGeometryCase.h"
#include "Bindable.h"

namespace DrawTest
{
	void SeveralGeometryCase::Init(Graphic & gfx)
	{
		m_cube = make_shared<Cuboid>(gfx, 1.0f, GeometryType::Common);
		m_cube->SetPos(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
		m_cube->SetRotation(0.0f, 1.0f, 0.0f);
		m_cube->SetRotSpeedAndStat(0.2f, RotaStat::Rotation);
		m_cubeTransform = make_shared<Bind::TransformCbuf>(gfx);
		m_cubeTransform->InitializeParentRef(*m_cube);

		m_sphere = make_shared<Sphere>(gfx, 0.5f, GeometryType::Common);
		m_sphere->SetPos(DirectX::XMFLOAT3(2.0f, 0.0f, 0.0f));
		m_sphereTransform = make_shared<Bind::TransformCbuf>(gfx);
		m_sphereTransform->InitializeParentRef(*m_sphere);

	/*	string vtFileName = "GeometryVS.cso";
		shared_ptr<Bind::VertexShader> vtShader = Bind::VertexShader::Resolve(gfx, vtFileName);
		vtShader->Bind(gfx);

		string psFileName = "GeometryPS.cso";
		Bind::PixelShader::Resolve(gfx, psFileName)->Bind(gfx);

		VertexRela::VertexLayout m_vtLayout;
		m_vtLayout.Append(VertexRela::VertexLayout::Position3D);
		m_vtLayout.Append(VertexRela::VertexLayout::Normal);
		m_vtLayout.Append(VertexRela::VertexLayout::Tangent);
		m_vtLayout.Append(VertexRela::VertexLayout::Texture2D);
		Bind::InputLayout::Resolve(gfx, m_vtLayout, *vtShader)->Bind(gfx);

		Bind::Sampler::Resolve(gfx)->Bind(gfx);*/

	}

	void SeveralGeometryCase::Draw(Graphic & gfx, float dt)
	{
		string vtFileName = "GeometryVS.cso";
		shared_ptr<Bind::VertexShader> vtShader = Bind::VertexShader::Resolve(gfx, vtFileName);
		vtShader->Bind(gfx);

		string psFileName = "GeometryPS.cso";
		Bind::PixelShader::Resolve(gfx, psFileName)->Bind(gfx);

		VertexRela::VertexLayout m_vtLayout;
		m_vtLayout.Append(VertexRela::VertexLayout::Position3D);
		m_vtLayout.Append(VertexRela::VertexLayout::Normal);
		m_vtLayout.Append(VertexRela::VertexLayout::Tangent);
		m_vtLayout.Append(VertexRela::VertexLayout::Texture2D);
		Bind::InputLayout::Resolve(gfx, m_vtLayout, *vtShader)->Bind(gfx);

		Bind::Sampler::Resolve(gfx)->Bind(gfx);

		string imagePath = "images\\WoodCrate02.dds";
		Bind::Texture::Resolve(gfx, imagePath)->Bind(gfx);
		m_cube->Bind(gfx);
		m_cubeTransform->Bind(gfx);
		//Bind::Blender::Resolve(gfx, false)->Bind(gfx);
		gfx.DrawIndex(m_cube->GetIndexCount());

		imagePath = "images\\brickwall.jpg";
		Bind::Texture::Resolve(gfx, imagePath)->Bind(gfx);
		m_sphere->Bind(gfx);
		m_sphereTransform->Bind(gfx);
		//Bind::Blender::Resolve(gfx, true, 0.3f)->Bind(gfx);
		gfx.DrawIndex(m_sphere->GetIndexCount());
	}
}

