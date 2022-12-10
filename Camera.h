#ifndef __CAMERA_H__
#define __CAMERA_H__

#pragma once

#include "CmnHead.h"
#include <DirectXMath.h>
#include <memory>
#include "Projection.h"

class Graphic;

class Camera
{
public:
	Camera(Graphic & gfx, string name, float aspect, DirectX::XMFLOAT3 homePos = { 0.0f,10.0f,-20.0f }, float homePitch = 0.0f, float homeYaw = 0.0f);
	void BindToGraphic(Graphic & gfx);
	DirectX::XMMATRIX & GetViewMatrix();
	DirectX::XMMATRIX GetProjMatrix();
	DirectX::XMFLOAT4 GetWorldPos() const;
	DirectX::XMMATRIX & SetLocationAndRotation();
	void SetMoveStat(bool isMove);
	void SetMoveVector(float x,float y,float z);
	void SetWorldPos(DirectX::XMFLOAT3 pos);
private:
	string m_name;
	DirectX::XMFLOAT3 m_homePos;
public:
	float m_homePitch;  //ÈÆxÖáÐý×ª£¬¸©Ñö½Ç
	float m_homeYaw;	//ÈÆyÖáÐý×ª£¬Æ«º½½Ç
	float m_aspect;

	DirectX::XMMATRIX m_pViewMatrix;
	DirectX::XMMATRIX m_pProjMatrix;
	DirectX::XMVECTOR m_up;
	DirectX::XMVECTOR m_forward;
	DirectX::XMVECTOR m_target;

	bool m_isMove = false;
	DirectX::XMVECTOR m_moveVector =DirectX::XMVectorSet(0.0f,0.0f,0.0f,0.0f);
	Projection m_proj;
};

#endif // !__CAMERA_H__
