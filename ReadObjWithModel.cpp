#include "ReadObjWithModel.h"
#include "Vertex.h"
#include "TechChannels.h"
#include "LayoutDic.h"
#include "DynamicConstant.h"
#include "LambertianPass.h"
#include "OutlineMaskGenerationPass.h"
#include "OutlineDrawingPass.h"
#include "SkyboxPass.h"
#include "BufferClearPass.h"

namespace DrawTest
{
	void ReadObjWithModel::Init(Graphic & gfx)
	{
		//path modelPath = "model\\gobber\\GoblinX.obj";
		//path modelPath = "model\\muro\\muro.obj";
		//Assimp::Importer importer;
		//const aiScene* pScene = importer.ReadFile(modelPath.string(),
		//	aiProcess_Triangulate |					//模型如果不是由三角形组成，需要将所有图元形状变换为三角形
		//	aiProcess_JoinIdenticalVertices |		//使用每个顶点的一份拷贝，并通过索引获取其引用，需要的时候可以帮助节省内存
		//	aiProcess_ConvertToLeftHanded |
		//	aiProcess_GenNormals |                  //模型没有法向量的话为每个顶点创建法线
		//	aiProcess_CalcTangentSpace              //为每个加载的顶点计算出柔和的切线和副切线向量
		//);

		//size_t meshSize = pScene->mNumMeshes;
		////一个场景有n个网格体组成，如哥布林模型由头、眼珠、虹膜、耳环四个网格体组成
		//for (size_t i = 0; i < meshSize; ++i)
		//{
		//	aiMesh* aiMs = pScene->mMeshes[i];
		//	Material mat(gfx, *pScene->mMaterials[aiMs->mMaterialIndex], modelPath);
		//	shared_ptr<Mesh> pMesh = make_shared<Mesh>(gfx, mat, *aiMs);
		//	m_drawbleList.push_back(pMesh);
		//}
		m_pModel = make_shared<Model>(gfx, "model\\nano_textured\\nanosuit.obj",1.0f);
		//m_pModel->SetRootTransform(DirectX::XMMatrixIdentity());
		DirectX::XMMATRIX tf = DirectX::XMMatrixIdentity();
		tf.r[3] = DirectX::XMVectorSet(30.0f, 0.0f, 0.0f, 1.0f);
		m_pModel->SetRootTransform(tf);

	/*	unique_ptr<Rgph::LambertianPass> lp = make_unique<Rgph::LambertianPass>(gfx, "lambertian");
		lp->SetSinkLinkage("renderTarget", "$.backbuffer");
		lp->SetSinkLinkage("depthStencil", "$.masterDepth");
		gfx.GetRenderGraph()->AppendPass(move(lp));

		unique_ptr<Rgph::SkyboxPass> sp = make_unique<Rgph::SkyboxPass>(gfx, "skybox");
		sp->SetSinkLinkage("renderTarget", "lambertian.renderTarget");
		sp->SetSinkLinkage("depthStencil", "lambertian.depthStencil");
		gfx.GetRenderGraph()->AppendPass(move(sp));*/

		//unique_ptr<Rgph::OutlineMaskGenerationPass> ogp = make_unique<Rgph::OutlineMaskGenerationPass>(gfx, "outlineMask");
		////ogp->SetSinkLinkage("renderTarget", "lambertian.renderTarget");
		//ogp->SetSinkLinkage("depthStencil", "skybox.depthStencil");
		//gfx.GetRenderGraph()->AppendPass(move(ogp));

		//unique_ptr<Rgph::OutlineDrawingPass> odp = make_unique<Rgph::OutlineDrawingPass>(gfx, "outlineDraw");
		//odp->SetSinkLinkage("renderTarget", "skybox.renderTarget");
		//odp->SetSinkLinkage("depthStencil", "outlineMask.depthStencil");
		//gfx.GetRenderGraph()->AppendPass(move(odp));

		/*gfx.GetRenderGraph()->SetSinkTarget("backbuffer", "skybox.renderTarget");
		gfx.GetRenderGraph()->Finalize();*/

		//for (size_t i = 0; i < m_drawbleList.size(); ++i)
		//{
		//	shared_ptr<Mesh> pMesh = m_drawbleList.at(i);
		//	//将相同名字的step和pass绑定
		//	pMesh->LinkTechnique(*gfx.GetRenderGraph());
		//	//给特定channel的technique下的所有step对应的pass添加渲染任务
		//	pMesh->Submit(Chan::main, DirectX::XMMatrixIdentity());
		//}

		//将每一个drawable的所有step和对应的pass绑定
		//m_pModel->LinkTechniques(*gfx.GetRenderGraph());
		//m_pModel->Submit(Chan::main);

		/*Dcb::RawLayout s;
		s.Add<Dcb::Struct>("butts"s);
		s["butts"s].Add<Dcb::Float3>("pubes"s);
		s["butts"s].Add<Dcb::Float>("dank"s);
		s.Add<Dcb::Float>("woot"s);
		s.Add<Dcb::Array>("arr"s);
		s["arr"s].Set<Dcb::Struct>(4);
		s["arr"s].T().Add<Dcb::Float3>("twerk"s);
		s["arr"s].T().Add<Dcb::Array>("werk"s);
		s["arr"s].T()["werk"s].Set<Dcb::Float>(6);
		s["arr"s].T().Add<Dcb::Array>("meta"s);
		s["arr"s].T()["meta"s].Set<Dcb::Array>(6);
		s["arr"s].T()["meta"s].T().Set<Dcb::Matrix>(4);
		s["arr"s].T().Add<Dcb::Bool>("booler");*/
	}

	void ReadObjWithModel::Draw(Graphic & gfx, float dt)
	{
	}

	void ReadObjWithModel::LinkTechnique(Rgph::RenderGraph & rg)
	{
		m_pModel->LinkTechniques(rg);
	}

	void ReadObjWithModel::Submit(size_t channelFilter)
	{
		m_pModel->Submit(channelFilter);
	}

}