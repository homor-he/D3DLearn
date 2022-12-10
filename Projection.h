#ifndef __PROJECTION_H__
#define __PROJECTION_H__

#pragma once

#include <DirectXMath.h>

class Graphic;

class Projection
{
public:
	Projection(Graphic & gfx, float width, float height, float nearZ, float farZ);
	DirectX::XMMATRIX GetProjMatrix();
private:
	float m_width;
	float m_height;
	float m_nearZ;
	float m_farZ;

};


#endif // !__PROJECTION_H__
