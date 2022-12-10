#include "CloneCase.h"
#include "Bindable.h"

namespace DrawTest
{
	void CloneCase::Init(Graphic & gfx)
	{
		for (int i = 0; i < 2; ++i)
		{
			shared_ptr<Cuboid> pCube =make_shared<Cuboid>(gfx, 1.0f, GeometryType::Common);
			pCube->SetPos(DirectX::XMFLOAT3(0.0f, 2.0f * i, 0.0f));
			pCube->SetRotation(0.0f*i, 0.0f * i, 1.0f*i);
			shared_ptr<Bind::TransformCbuf> pTransform = make_shared<Bind::TransformCbuf>(gfx);
			pTransform->InitializeParentRef(*pCube);
			m_cubeList.push_back(pCube);
			m_cubeTransformList.push_back(pTransform);
		}
		m_sphere = make_shared<Sphere>(gfx, 0.5f, GeometryType::Common);
		m_sphere->SetPos(DirectX::XMFLOAT3(2.0f, 0.0f, 0.0f));
		m_sphereTransform = make_shared<Bind::TransformCbuf>(gfx);
		m_sphereTransform->InitializeParentRef(*m_sphere);

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
	}

	void CloneCase::Draw(Graphic & gfx, float dt)
	{
		string imagePath = "images\\WoodCrate02.dds";
		Bind::Texture::Resolve(gfx, imagePath)->Bind(gfx);
		for (int i = 0; i < m_cubeList.size(); ++i)
		{
			m_cubeList[i]->Bind(gfx);
			m_cubeList[i]->SetRotSpeedAndStat(dt,RotaStat::Rotation);
			m_cubeTransformList[i]->Bind(gfx);
			gfx.DrawIndex(m_cubeList[i]->GetIndexCount());
		}

		imagePath = "images\\brickwall.jpg";
		Bind::Texture::Resolve(gfx, imagePath)->Bind(gfx);
		m_sphere->Bind(gfx);
		m_sphereTransform->Bind(gfx);
		gfx.DrawIndex(m_sphere->GetIndexCount());
	}
}


