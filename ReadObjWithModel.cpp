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
		//	aiProcess_Triangulate |					//ģ�������������������ɣ���Ҫ������ͼԪ��״�任Ϊ������
		//	aiProcess_JoinIdenticalVertices |		//ʹ��ÿ�������һ�ݿ�������ͨ��������ȡ�����ã���Ҫ��ʱ����԰�����ʡ�ڴ�
		//	aiProcess_ConvertToLeftHanded |
		//	aiProcess_GenNormals |                  //ģ��û�з������Ļ�Ϊÿ�����㴴������
		//	aiProcess_CalcTangentSpace              //Ϊÿ�����صĶ���������͵����ߺ͸���������
		//);

		//size_t meshSize = pScene->mNumMeshes;
		////һ��������n����������ɣ���粼��ģ����ͷ�����顢��Ĥ�������ĸ����������
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
		//	//����ͬ���ֵ�step��pass��
		//	pMesh->LinkTechnique(*gfx.GetRenderGraph());
		//	//���ض�channel��technique�µ�����step��Ӧ��pass�����Ⱦ����
		//	pMesh->Submit(Chan::main, DirectX::XMMatrixIdentity());
		//}

		//��ÿһ��drawable������step�Ͷ�Ӧ��pass��
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