#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Material.h"
#include "ConvertFunc.h"

Model::Model(Graphic & gfx, const string & pathString, float scale)
{
	path fileName = pathString;
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(pathString,
		aiProcess_Triangulate |					//ģ�������������������ɣ���Ҫ������ͼԪ��״�任Ϊ������
		aiProcess_JoinIdenticalVertices |		//ʹ��ÿ�������һ�ݿ�������ͨ��������ȡ�����ã���Ҫ��ʱ����԰�����ʡ�ڴ�
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |                  //ģ��û�з������Ļ�Ϊÿ�����㴴������
		aiProcess_CalcTangentSpace              //Ϊÿ�����صĶ���������͵����ߺ͸���������
	);

	if (pScene == nullptr)
		OutPutError("%s:%d\nscene is null", __FILE__, __LINE__);

	vector<Material> materials;
	for (size_t i = 0; i < pScene->mNumMaterials; ++i)
	{
		materials.emplace_back(gfx, *pScene->mMaterials[i], fileName);
	}

	for (size_t i = 0; i < pScene->mNumMeshes; ++i)
	{
		aiMesh * mesh = pScene->mMeshes[i];
		m_drawableList.push_back(make_unique<Mesh>(gfx, materials[mesh->mMaterialIndex], *mesh, scale));
	}

	int nextID = 0;
	m_pRoot = ParseNode(nextID, *pScene->mRootNode, scale);
}

void Model::SetRootTransform(DirectX::FXMMATRIX tf)
{
	m_pRoot->SetAppliedTransform(tf);
}

void Model::Submit(size_t channelFilter)
{
	m_pRoot->Submit(channelFilter, DirectX::XMMatrixIdentity());
}

void Model::LinkTechniques(Rgph::RenderGraph & renderGraph)
{
	for (auto & mesh : m_drawableList)
	{
		mesh->LinkTechnique(renderGraph);
	}
}

unique_ptr<Node> Model::ParseNode(int & nextId, const aiNode & node, float scale)
{
	vector<Mesh*> curMeshPtrs;
	//ÿһ��aiNode�洢��һ��aiMesh������
	for (size_t i = 0; i < node.mNumMeshes; ++i)
	{
		int meshIdx = node.mMeshes[i];
		curMeshPtrs.push_back(m_drawableList.at(meshIdx).get());
	}

	DirectX::XMMATRIX nodeTransform = DirectX::XMMatrixTranspose(
		DirectX::XMLoadFloat4x4(reinterpret_cast<const DirectX::XMFLOAT4X4*>(&node.mTransformation)));
	nodeTransform = ScaleTranslation(nodeTransform, scale);

	unique_ptr<Node> pNode = make_unique<Node>(nextId++, node.mName.C_Str(), move(curMeshPtrs), nodeTransform);
	for (size_t i = 0; i < node.mNumChildren; ++i)
	{
		//���õݹ�ķ��������ӽڵ㣬���νṹ
		pNode->AddChild(ParseNode(nextId, *node.mChildren[i], scale));
	}
	return pNode;
}
