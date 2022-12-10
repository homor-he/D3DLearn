#include "TransformCbuf.h"
#include "ConvertFunc.h"

namespace Bind
{
	unique_ptr<VertexConstantBuffer<TransformCbuf::Transform>> TransformCbuf::m_pTransferBuf;

	TransformCbuf::TransformCbuf(Graphic & gfx, UINT slot)
	{
		if (!m_pTransferBuf)
			m_pTransferBuf = make_unique<VertexConstantBuffer<TransformCbuf::Transform>>(gfx, slot);
	}

	void TransformCbuf::Bind(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(UpdateBind(gfx, GetTransform(gfx)));
	}

	void TransformCbuf::InitializeParentRef(Drawable & parent)
	{
		m_parent = &parent;
	}

	unique_ptr<CloneBindable> TransformCbuf::Clone()
	{
		return make_unique<TransformCbuf>(*this);
	}

	void TransformCbuf::UpdateBind(Graphic & gfx, Transform & tf)
	{
		m_pTransferBuf->Update(gfx, tf);
		m_pTransferBuf->Bind(gfx);
	}

	TransformCbuf::Transform TransformCbuf::GetTransform(Graphic & gfx)
	{
		using namespace DirectX;
		if (m_parent == nullptr)
			OutPutError("%s:%d\nparent is null", __FILE__, __LINE__);
		XMMATRIX world = m_parent->GetTransform();

		XMMATRIX worldInvTranspose = InverseTranspose(world);
			
		DirectX::XMMATRIX worldView = world * gfx.GetCamera();

		//DirectX::XMMATRIX worldViewTranspose = DirectX::XMMatrixTranspose(worldView);
		//DirectX::XMMATRIX worldViewInverse = InverseTranspose(worldView);

		return{ DirectX::XMMatrixTranspose(world),
		DirectX::XMMatrixTranspose(worldView),
		DirectX::XMMatrixTranspose(worldView * gfx.GetProjection()),
		DirectX::XMMatrixTranspose(worldInvTranspose)};
	}
}


