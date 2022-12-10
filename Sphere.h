#ifndef __SPHERE_H__
#define __SPHERE_H__

#pragma once

#include "Drawable.h"
#include "Bindable.h"
#include "Vertex.h"

class Sphere : public Drawable
{
public:
	Sphere(Graphic & gfx, float radius, GeometryType type, string name = "");
	void SetPos(DirectX::XMFLOAT3 pos);
	DirectX::XMFLOAT3 GetCBPos();
	DirectX::XMMATRIX GetTransform() override;
	void SetRotation(float pitch, float yaw, float roll);
	void SetRotSpeedAndStat(float rotspeed, RotaStat rotStat);
private:
	DirectX::XMFLOAT3 m_pos = {0.0f,0.0f,0.0f};
	DirectX::XMFLOAT3 m_cbPos = { 0.0f,0.0f,0.0f };
	//绕x轴旋转的角度
	float m_pitch = 0.0f;
	//绕y轴旋转的角度
	float m_yaw = 0.0f;
	//绕z轴旋转的角度
	float m_roll = 0.0f;

	RotaStat m_rotStat = RotaStat::Rotation;
	float m_rotSpeedPerFrame = 0.0f;
	float m_rotSpeed = 0.0f;
public:
	shared_ptr<VertexRela::VertexBuffer> m_vtxBuffer;
};


#endif // !__SPHERE_H__
