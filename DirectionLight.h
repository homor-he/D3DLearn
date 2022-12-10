#ifndef __DIRECTIONLIGHT_H__
#define __DIRECTIONLIGHT_H__

#pragma once

#include "Graphic.h"
#include "Sphere.h"
#include "ConstantBuffer.h"

class DirectLight
{
public:
	DirectLight(Graphic & gfx, DirectX::XMFLOAT3 pos = { 3.0f,10.0f,3.0f }, float radius = 0.5f);
	void Bind(Graphic & gfx, DirectX::XMMATRIX view);
private:
	struct DirectLightCBuf
	{
		alignas(16) DirectX::XMFLOAT3 ambient;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		alignas(16) DirectX::XMFLOAT3 Direction;
		int useDirectLight;
	};
public:
	Sphere m_solidSphere;
private:
	DirectLightCBuf m_home;
	DirectLightCBuf m_cbData;

	Bind::PixelConstantBuffer<DirectLightCBuf> m_cBuf;
};

#endif // !__DIRECTIONLIGHT_H__

