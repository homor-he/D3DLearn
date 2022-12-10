#ifndef __LIGHTCASE_H__
#define __LIGHTCASE_H__

#pragma once

#include "CaseBase.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "DirectionLight.h"
#include "BindableCommon.h"

namespace DrawTest
{
	//ʵ�ֵ��Դ����
	class PointLightCase : public CaseBase
	{
	public:
		void Init(Graphic & gfx) override;
		void Draw(Graphic & gfx, float dt) override;
		void Render(Graphic & gfx,DirectX::XMFLOAT3 mainCamWorldPos);
		shared_ptr<Camera> ShareCamera();
	private:
		shared_ptr<PointLight> m_light;
		shared_ptr<Bind::TransformCbuf> m_lightTransformBuf;

		shared_ptr<Bind::VertexShader> m_vtxShader;
		shared_ptr<Bind::PixelShader> m_pxShader;
		shared_ptr<Bind::InputLayout> m_inputLayout;
	};

	//ʵ�־۹�ƻ���
	class SpotLightCase : public CaseBase
	{
	public:
		void Init(Graphic & gfx) override;
		void Draw(Graphic & gfx, float dt) override;
		void Render(Graphic & gfx);
	private:
		shared_ptr<SpotLight> m_light;
		shared_ptr<Bind::TransformCbuf> m_lightTransformBuf;

		shared_ptr<Bind::VertexShader> m_vtxShader;
		shared_ptr<Bind::PixelShader> m_pxShader;
		shared_ptr<Bind::InputLayout> m_inputLayout;
	};

	//ʵ��ƽ�й����
	class DirectLightCase : public CaseBase
	{
	public:
		void Init(Graphic & gfx) override;
		void Draw(Graphic & gfx, float dt) override;
		void Render(Graphic & gfx);
	private:
		shared_ptr<DirectLight> m_light;
		shared_ptr<Bind::TransformCbuf> m_lightTransformBuf;

		shared_ptr<Bind::VertexShader> m_vtxShader;
		shared_ptr<Bind::PixelShader> m_pxShader;
		shared_ptr<Bind::InputLayout> m_inputLayout;
		shared_ptr<Bind::Blender> m_blend;
	};
}


#endif // !__POINTLIGHTCASE_H__
