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
		
		//��ȡ����ͶӰ����
		/*DirectX::XMVECTOR targetPos = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
		
		DirectX::XMFLOAT3 targetViewPos;
		DirectX::XMStoreFloat3(&targetViewPos, DirectX::XMVector3TransformCoord(targetPos, m_pCamera->m_pViewMatrix));
		
		float l = targetViewPos.x - 1.0f;
		float b = targetViewPos.y - 1.0f;
		float n = targetViewPos.z - 1.0f;
		float r = targetViewPos.x + 1.0f;
		float t = targetViewPos.y + 1.0f;
		float f = targetViewPos.z + 1.0f;
		//��ȡ����ͶӰ����
		DirectX::XMMATRIX otProj = DirectX::XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);*/


		////////////////////////////////////////////////////
		//��ȡ͸��ͶӰ���� ��pixelshader�г���w

		////�Ȼ�ȡviewproj����
		////transform.viewProjNDCScreen = DirectX::XMMatrixMultiply(m_pCamera->m_pViewMatrix, otProj);
		//DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiply(gfx.GetCamera(), gfx.GetProjection());

		////��������ͶӰw=1,����Ҫ��͸�ӳ������NDC�ռ�
		////͸��ͶӰw!=1,��Ҫ��pixelShader����͸�ӳ�������vertex����������������
		////NDC�ռ�[-1,+1]->����ռ�[0,1]
		//DirectX::XMMATRIX S(
		//	0.5f, 0.0f, 0.0f, 0.0f,
		//	0.0f, -0.5f, 0.0f, 0.0f,
		//	0.0f, 0.0f, 1.0f, 0.0f,
		//	0.5f, 0.5f, 0.0f, 1.0f);
		////��ȡviewProjNDCTexture��������
		//transform.viewProjNDCTexture = DirectX::XMMatrixTranspose(DirectX::XMMatrixMultiply(viewProj, S));
		//m_vtxBuf->Update(gfx, transform);
		//////////////////////////////////////////////////////

		DirectX::XMFLOAT4 cameraPos = m_pCamera->GetWorldPos();
		//ֻ��ȡ��Դλ����ͼ����
		transform.viewProjNDCTexture = DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-cameraPos.x, -cameraPos.y, -cameraPos.z));
		m_vtxBuf->Update(gfx, transform);
	}
}