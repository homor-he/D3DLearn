#include "Camera.h"
#include "Graphic.h"

Camera::Camera(Graphic & gfx, string name, float aspect, DirectX::XMFLOAT3 homePos, float homePitch, float homeYaw) :
	m_name(move(name)), m_aspect(aspect), m_homePos(homePos), m_homePitch(homePitch), m_homeYaw(homeYaw), m_proj(gfx, 1.0f, 1.0f, 1.0f, 1000.0f)
{
	m_up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	m_target = DirectX::XMVectorZero();
	//BindToGraphic(gfx);
}

void Camera::BindToGraphic(Graphic & gfx)
{
	gfx.SetCamera(GetViewMatrix());
	gfx.SetProjection(GetProjMatrix());
}

DirectX::XMMATRIX & Camera::GetViewMatrix()
{
	auto lookVector = DirectX::XMVector4Transform(m_forward, DirectX::XMMatrixRotationRollPitchYaw(m_homePitch, m_homeYaw, 0.0f));
	auto cameraPos = DirectX::XMVectorSet(m_homePos.x, m_homePos.y, m_homePos.z, 1.0f);
	auto target = DirectX::XMVectorAdd(cameraPos, lookVector);
	auto upVector = DirectX::XMVector4Transform(m_up, DirectX::XMMatrixRotationRollPitchYaw(m_homePitch, m_homeYaw, 0.0f));
	m_pViewMatrix = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(m_homePos.x, m_homePos.y, m_homePos.z, 1.0f), target, upVector);
	return m_pViewMatrix;
}

DirectX::XMMATRIX Camera::GetProjMatrix()
{
	//m_pProjMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI/2, m_aspect, 0.2f, 100.0f);
	m_pProjMatrix = m_proj.GetProjMatrix();
	return m_pProjMatrix;
}

DirectX::XMFLOAT4 Camera::GetWorldPos() const
{
	return { m_homePos.x,m_homePos.y,m_homePos.z,1.0f};
}

DirectX::XMMATRIX & Camera::SetLocationAndRotation()
{
	if (!DirectX::XMVector3Equal(m_moveVector, DirectX::XMVectorZero()))
	{
		//设置向右基础移动向量
		auto rightBase = DirectX::XMVectorSet(m_moveVector.m128_f32[0], 0.0f, 0.0f, 0.0f);
		//旋转后的基础移动向量
		auto rightVector = DirectX::XMVector4Transform(rightBase, DirectX::XMMatrixRotationRollPitchYaw(m_homePitch, m_homeYaw, 0.0f));

		//设置向前基础移动向量
		auto forwardBase = DirectX::XMVectorSet(0.0f, 0.0f, m_moveVector.m128_f32[2], 0.0f);
		//旋转后的基础移动向量
		auto lookVector = DirectX::XMVector4Transform(forwardBase, DirectX::XMMatrixRotationRollPitchYaw(m_homePitch, m_homeYaw, 0.0f));

		auto cameraPos = DirectX::XMVectorSet(m_homePos.x, m_homePos.y, m_homePos.z, 1.0f);
		auto finalPos = DirectX::XMVectorAdd(cameraPos, rightVector);
		finalPos = DirectX::XMVectorAdd(finalPos, lookVector);
		//设置相机最终位置
		DirectX::XMStoreFloat3(&m_homePos, finalPos);
	}
	return GetViewMatrix();
}

void Camera::SetMoveStat(bool isMove)
{
	m_isMove = isMove;
}

void Camera::SetMoveVector(float x, float y, float z)
{
	m_moveVector = DirectX::XMVectorSet(x, y, z, 0.0f);
}

void Camera::SetWorldPos(DirectX::XMFLOAT3 pos)
{
	m_homePos = pos;
}
