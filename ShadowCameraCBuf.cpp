#include "ShadowCameraCBuf.h"
#include "Camera.h"

namespace Bind
{
	ShadowCameraCBuf::ShadowCameraCBuf(Graphic & gfx, UINT slot) :
		m_vtxBuf(make_unique<VertexConstantBuffer<Transform>>(gfx,slot))
	{
	}

	void ShadowCameraCBuf::Bind(Graphic & gfx)
	{
		m_vtxBuf->Bind(gfx);
	}

	void ShadowCameraCBuf::SetCamera(Camera * pCamera)
	{
		m_pCamera = pCamera;
	}

	void ShadowCameraCBuf::Update(Graphic & gfx)
	{
		Transform transform;
		//t.viewProj = DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-wPos.x, -wPos.y, -wPos.z));
		
		//获取正交投影矩阵
		/*DirectX::XMVECTOR targetPos = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
		
		DirectX::XMFLOAT3 targetViewPos;
		DirectX::XMStoreFloat3(&targetViewPos, DirectX::XMVector3TransformCoord(targetPos, m_pCamera->m_pViewMatrix));
		
		float l = targetViewPos.x - 1.0f;
		float b = targetViewPos.y - 1.0f;
		float n = targetViewPos.z - 1.0f;
		float r = targetViewPos.x + 1.0f;
		float t = targetViewPos.y + 1.0f;
		float f = targetViewPos.z + 1.0f;
		//获取正交投影函数
		DirectX::XMMATRIX otProj = DirectX::XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);*/


		////////////////////////////////////////////////////
		//获取透视投影矩阵 在pixelshader中除以w

		////先获取viewproj矩阵
		////transform.viewProjNDCScreen = DirectX::XMMatrixMultiply(m_pCamera->m_pViewMatrix, otProj);
		//DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiply(gfx.GetCamera(), gfx.GetProjection());

		////由于正交投影w=1,不需要做透视除法变成NDC空间
		////透视投影w!=1,但要在pixelShader中做透视除法，在vertex中做除法会有问题
		////NDC空间[-1,+1]->纹理空间[0,1]
		//DirectX::XMMATRIX S(
		//	0.5f, 0.0f, 0.0f, 0.0f,
		//	0.0f, -0.5f, 0.0f, 0.0f,
		//	0.0f, 0.0f, 1.0f, 0.0f,
		//	0.5f, 0.5f, 0.0f, 1.0f);
		////获取viewProjNDCTexture行主矩阵
		//transform.viewProjNDCTexture = DirectX::XMMatrixTranspose(DirectX::XMMatrixMultiply(viewProj, S));
		//m_vtxBuf->Update(gfx, transform);
		//////////////////////////////////////////////////////

		DirectX::XMFLOAT4 cameraPos = m_pCamera->GetWorldPos();
		//只获取光源位置视图矩阵
		transform.viewProjNDCTexture = DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-cameraPos.x, -cameraPos.y, -cameraPos.z));
		m_vtxBuf->Update(gfx, transform);
	}
}