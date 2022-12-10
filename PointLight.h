#ifndef __POINTLIGHT_H__
#define __POINTLIGHT_H__

#pragma once

#include "Graphic.h"
#include "Sphere.h"
#include "ConstantBuffer.h"

class Camera;

class PointLight
{
public:
	PointLight(Graphic & gfx, DirectX::XMFLOAT3 pos = { 0.0f,20.0f,0.0f }, float radius = 0.5f);
	void Bind(Graphic & gfx, DirectX::XMMATRIX view, DirectX::XMFLOAT3 mainCamWorldPos);
	void Submit(size_t channel);
	shared_ptr<Camera> ShareCamera();
private:
	struct PointLightCBuf
	{
		alignas(16) DirectX::XMFLOAT3 viewPos;
		alignas(16) DirectX::XMFLOAT3 ambient;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		alignas(16) DirectX::XMFLOAT3 worldPos;
		alignas(16) DirectX::XMFLOAT3 mainCameraPos;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
		int usePointLight;
	};
public:
	Sphere m_solidSphere;
private:
	PointLightCBuf m_home;
	PointLightCBuf m_cbData;
	
	Bind::PixelConstantBuffer<PointLightCBuf> m_cBuf;
	//用于获取深度图以显示阴影
	shared_ptr<Camera> m_pCamera;
};

#endif // !__POINTLIGHT_H__
