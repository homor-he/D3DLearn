#include "LightCase.h"

namespace DrawTest
{
	//点光源
	void PointLightCase::Init(Graphic & gfx)
	{
		m_light = make_shared<PointLight>(gfx);
		//m_light->m_solidSphere.SetRotation(0.0f, 1.0f, 0.0f);
		//m_light->m_solidSphere.SetRotSpeedAndStat(0.01f, RotaStat::Revolution);
		m_lightTransformBuf = make_shared<Bind::TransformCbuf>(gfx);
		m_lightTransformBuf->InitializeParentRef(m_light->m_solidSphere);

		string vtxShaderName = "SolidVS.cso";
		m_vtxShader = Bind::VertexShader::Resolve(gfx, vtxShaderName);

		string pxShaderName = "SolidPS.cso";
		m_pxShader = Bind::PixelShader::Resolve(gfx, pxShaderName);

		VertexRela::VertexLayout m_vtLayout;
		m_vtLayout.Append(VertexRela::VertexLayout::Position3D);
		m_inputLayout = Bind::InputLayout::Resolve(gfx, m_vtLayout, *m_vtxShader);
	}

	void PointLightCase::Draw(Graphic & gfx, float dt)
	{
		m_inputLayout->Bind(gfx);
		m_vtxShader->Bind(gfx);
		m_pxShader->Bind(gfx);
		//m_light->m_solidSphere.Bind(gfx);
		m_lightTransformBuf->Bind(gfx);
		DirectX::XMFLOAT4 color(1.0f, 1.0f, 1.0f, 0.0f);
		Bind::PixelConstantBuffer<DirectX::XMFLOAT4>::Resolve(gfx, color, 1u)->Bind(gfx);
		//gfx.DrawIndex(m_light->m_solidSphere.GetIndexCount());
	}

	void PointLightCase::Render(Graphic & gfx, DirectX::XMFLOAT3 mainCamWorldPos)
	{
		m_light->Bind(gfx, gfx.GetCamera(), mainCamWorldPos);
	}

	shared_ptr<Camera> PointLightCase::ShareCamera()
	{
		return m_light->ShareCamera();
	}

	//聚光灯
	void SpotLightCase::Init(Graphic & gfx)
	{
		m_light = make_shared<SpotLight>(gfx);
		m_light->m_solidSphere.SetRotation(0.0f, 1.0f, 0.0f);
		m_light->m_solidSphere.SetRotSpeedAndStat(0.01f, RotaStat::Revolution);
		m_lightTransformBuf = make_shared<Bind::TransformCbuf>(gfx);
		m_lightTransformBuf->InitializeParentRef(m_light->m_solidSphere);

		string vtxShaderName = "SolidVS.cso";
		m_vtxShader = Bind::VertexShader::Resolve(gfx, vtxShaderName);

		string pxShaderName = "SolidPS.cso";
		m_pxShader = Bind::PixelShader::Resolve(gfx, pxShaderName);

		VertexRela::VertexLayout m_vtLayout;
		m_vtLayout.Append(VertexRela::VertexLayout::Position3D);
		m_inputLayout = Bind::InputLayout::Resolve(gfx, m_vtLayout, *m_vtxShader);
	}

	void SpotLightCase::Draw(Graphic & gfx, float dt)
	{
		m_inputLayout->Bind(gfx);
		m_vtxShader->Bind(gfx);
		m_pxShader->Bind(gfx);
		m_light->m_solidSphere.Bind(gfx);
		m_lightTransformBuf->Bind(gfx);
		DirectX::XMFLOAT4 color(1.0f, 1.0f, 1.0f, 0.0f);
		Bind::PixelConstantBuffer<DirectX::XMFLOAT4>::Resolve(gfx, color, 1u)->Bind(gfx);
		gfx.DrawIndex(m_light->m_solidSphere.GetIndexCount());
	}

	void SpotLightCase::Render(Graphic & gfx)
	{
		m_light->Bind(gfx, gfx.GetCamera());
	}


	//平行光
	void DirectLightCase::Init(Graphic & gfx)
	{
		m_light = make_shared<DirectLight>(gfx);
		m_light->m_solidSphere.SetRotation(0.0f, 1.0f, 0.0f);
		m_light->m_solidSphere.SetRotSpeedAndStat(0.01f, RotaStat::Revolution);
		m_lightTransformBuf = make_shared<Bind::TransformCbuf>(gfx);
		m_lightTransformBuf->InitializeParentRef(m_light->m_solidSphere);

		string vtxShaderName = "SolidVS.cso";
		m_vtxShader = Bind::VertexShader::Resolve(gfx, vtxShaderName);

		string pxShaderName = "SolidPS.cso";
		m_pxShader = Bind::PixelShader::Resolve(gfx, pxShaderName);

		VertexRela::VertexLayout m_vtLayout;
		m_vtLayout.Append(VertexRela::VertexLayout::Position3D);
		m_inputLayout = Bind::InputLayout::Resolve(gfx, m_vtLayout, *m_vtxShader);

		m_blend = make_shared<Bind::Blender>(gfx, true, 0.0f);
	}

	void DirectLightCase::Draw(Graphic & gfx, float dt)
	{
		m_inputLayout->Bind(gfx);
		m_vtxShader->Bind(gfx);
		m_pxShader->Bind(gfx);
		//m_light->m_solidSphere.Bind(gfx);
		m_lightTransformBuf->Bind(gfx);
		//隐藏平行光球形体
		//m_blend->SetFactor(0.0f);
		//m_blend->Bind(gfx);
		DirectX::XMFLOAT4 color(1.0f, 1.0f, 1.0f, 0.0f);
		Bind::PixelConstantBuffer<DirectX::XMFLOAT4>::Resolve(gfx, color, 1u)->Bind(gfx);
		//gfx.DrawIndex(m_light->m_solidSphere.GetIndexCount());
		//防止隐藏其他要渲染的物体
		//m_blend->SetFactor(1.0f);
		//m_blend->Bind(gfx);
	}

	void DirectLightCase::Render(Graphic & gfx)
	{
		m_light->Bind(gfx, gfx.GetCamera());
	}
}


