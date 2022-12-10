#include "Node.h"
#include "Mesh.h"

Node::Node(int id, const string & name, vector<Mesh*> meshPtrs, const DirectX::XMMATRIX & transform)
	: m_id(id),m_name(name),m_meshPtrs(move(meshPtrs))
{
	DirectX::XMStoreFloat4x4(&m_transform, transform);
	DirectX::XMStoreFloat4x4(&m_appliedTransform, DirectX::XMMatrixIdentity());
}

void Node::SetAppliedTransform(DirectX::FXMMATRIX transform)
{
	DirectX::XMStoreFloat4x4(&m_appliedTransform, transform);
}

void Node::Submit(size_t channels, DirectX::FXMMATRIX transform)
{
	 DirectX::XMMATRIX ts = DirectX::XMLoadFloat4x4(&m_appliedTransform)*DirectX::XMLoadFloat4x4(&m_transform) * transform;
	 for (auto pMesh : m_meshPtrs)
	 {
		 pMesh->Submit(channels, ts);
	 }

	 //µÝ¹énode
	 for (auto & pNode : m_childPtrs)
	 {
		 pNode->Submit(channels, ts);
	 }
}

void Node::AddChild(unique_ptr<Node> pChild)
{
	m_childPtrs.push_back(move(pChild));
}
