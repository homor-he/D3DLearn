#ifndef __WAVES_H__
#define __WAVES_H__

#pragma once

#include "Drawable.h"
#include "Vertex.h"

class Waves : public Drawable
{
public:
	Waves(Graphic & gfx, UINT row, UINT col, float dx, float dt, float speed, float damping);
	void SetPos(DirectX::XMFLOAT3 pos);
	DirectX::XMMATRIX GetTransform() override;
	void SetRotation(float pitch, float yaw, float roll);
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetD3DVtxBuffer();
	DirectX::XMFLOAT3 GetCurrVertexIndexPos(DWORD index);
	UINT GetVertextCnt();

	void Disturb(UINT i, UINT j, float magnitude);
	void Update(float dt);
private:
	//顶点行列数
	UINT m_numRow;
	UINT m_numCol;

	UINT m_vertexCount;
	UINT m_triangleCount;

	float m_k1;
	float m_k2;
	float m_k3;

	float m_timeStep;
	float m_spatialStep;

	DirectX::XMFLOAT3 * m_prePosList;
	DirectX::XMFLOAT3 * m_currPosList;
	//DirectX::XMFLOAT3 * m_normals;
	//DirectX::XMFLOAT3 * m_tangentX;

	//中心点坐标
	DirectX::XMFLOAT3 m_pos = { 0.0f,0.0f,0.0f };
	//绕x轴旋转的角度
	float m_pitch = 0.0f;
	//绕y轴旋转的角度
	float m_yaw = 0.0f;
	//绕z轴旋转的角度
	float m_roll = 0.0f;
public:
	shared_ptr<VertexRela::VertexBuffer> m_vtxBuffer;
};



#endif // !__WAVES_H__
