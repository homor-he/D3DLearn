#ifndef __SPOTLIGHT_H__
#define __SPOTLIGHT_H__

#pragma once

#include "Graphic.h"
#include "Sphere.h"
#include "ConstantBuffer.h"

class SpotLight
{
public:
	SpotLight(Graphic & gfx, DirectX::XMFLOAT3 pos = { 2.0f,10.0f,2.0f }, float radius = 0.5f);
	void Bind(Graphic & gfx, DirectX::XMMATRIX view);
private:
	struct SpotLightCBuf
	{
		alignas(16) DirectX::XMFLOAT3 pos;
		alignas(16) DirectX::XMFLOAT3 ambient;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
		alignas(16) DirectX::XMFLOAT3 Direction;
		float spotFactor;	//聚光灯系数，该值越大，光照范围越小；
		int useSpotLight;
	};
public:
	Sphere m_solidSphere;
private:
	SpotLightCBuf m_home;
	SpotLightCBuf m_cbData;

	Bind::PixelConstantBuffer<SpotLightCBuf> m_cBuf;
};

#endif // ! __SPOTLIGHT_H__
