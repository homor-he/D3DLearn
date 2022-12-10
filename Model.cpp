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
		aiProcess_Triangulate |					//模型如果不是由三角形组成，需要将所有图元形状变换为三角形
		aiProcess_JoinIdenticalVertices |		//使用每个顶点的一份拷贝，并通过索引获取其引用，需要的时候可以帮助节省内存
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |                  //模型没有法向量的话为每个顶点创建法线
		aiProcess_CalcTangentSpace              //为每个加载的顶点计算出柔和的切线和副切线向量
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
	//每一个aiNode存储着一组aiMesh的索引
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
		//采用递归的方法遍历子节点，树形结构
		pNode->AddChild(ParseNode(nextId, *node.mChildren[i], scale));
	}
	return pNode;
}
