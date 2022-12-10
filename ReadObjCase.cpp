#include "ReadObjCase.h"
#include "Vertex.h"

namespace DrawTest
{
	void ReadObjCase::Init(Graphic & gfx)
	{
		/*m_pixelObjectCBuf = {
			{ 1.0f,1.0f,1.0f },
			0.3f,0.6f,true,0.8f
		};*/

		m_pixelObjectCBuf = {
			true,true,{1.0f,1.0f,1.0f},
			0.05f,0.1f,true,0.9f
		};

		string modelPath = "model\\gobber\\GoblinX.obj";
		Assimp::Importer importer;
		const aiScene* pScene = importer.ReadFile(modelPath,
			aiProcess_Triangulate |					//模型如果不是由三角形组成，需要将所有图元形状变换为三角形
			aiProcess_JoinIdenticalVertices |		//使用每个顶点的一份拷贝，并通过索引获取其引用，需要的时候可以帮助节省内存
			aiProcess_ConvertToLeftHanded |
			aiProcess_GenNormals |                  //模型没有法向量的话为每个顶点创建法线
			aiProcess_CalcTangentSpace              //为每个加载的顶点计算出柔和的切线和副切线向量
		);

		using namespace VertexRela;
		//根据vertexshader的参数设置输入类型参数
		auto layout = VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
			.Append(VertexLayout::Texture2D)
			.Append(VertexLayout::Tangent)
			.Append(VertexLayout::Bitangent);

		VertexBuffer buf{ std::move(layout),*pScene->mMeshes[0] };
		for (auto i = 0ull, end = buf.GetSize(); i < end; i++)
		{
			const auto a = buf[i].Attr<VertexLayout::Position3D>();
			const auto b = buf[i].Attr<VertexLayout::Normal>();
			const auto c = buf[i].Attr<VertexLayout::Tangent>();
			const auto d = buf[i].Attr<VertexLayout::Bitangent>();
			const auto e = buf[i].Attr<VertexLayout::Texture2D>();
		}
		string tag = "$gobber";
		m_pVtBuffer = Bind::VertexBuffer::Resolve(gfx, tag, buf);

		vector<unsigned short> indices;
		for (size_t i = 0; i < pScene->mMeshes[0]->mNumFaces; ++i)
		{
			auto & face = pScene->mMeshes[0]->mFaces[i];
			indices.emplace_back(face.mIndices[0]);
			indices.emplace_back(face.mIndices[1]);
			indices.emplace_back(face.mIndices[2]);
		}
		m_pIdBuffer = Bind::IndexBuffer::Resolve(gfx, tag, indices);

		m_pTopology = Bind::Topology::Resolve(gfx);

		m_vtxShader = Bind::VertexShader::Resolve(gfx, "PhongDifSpcNrmMsk_VS.cso");

		m_pxShader = Bind::PixelShader::Resolve(gfx, "PhongDifSpcNrmMsk_PS.cso");

		m_inputLayout = Bind::InputLayout::Resolve(gfx, buf.GetLayout(), *m_vtxShader);

	    m_pTexture =  Bind::Texture::Resolve(gfx, "model\\gobber\\Textures\\Mdif.png");
		//m_pTexture = Bind::Texture::Resolve(gfx, "model\\brick_wall\\brick_wall_diffuse.jpg");
		
		m_pSampler = Bind::Sampler::Resolve(gfx);

		m_pNormalTex = Bind::Texture::Resolve(gfx, "model\\gobber\\Textures\\MNorm.tif", 2);
		
		m_pSpecTex = Bind::Texture::Resolve(gfx, "model\\gobber\\Textures\\MSpec.tif", 1);

		Bind::Rasterizer::Resolve(gfx, false)->Bind(gfx);
	}

	void ReadObjCase::Draw(Graphic & gfx, float dt)
	{
		m_pVtBuffer->Bind(gfx);
		m_pIdBuffer->Bind(gfx);
		m_pTopology->Bind(gfx);
		m_vtxShader->Bind(gfx);
		m_pxShader->Bind(gfx);
		m_inputLayout->Bind(gfx);
		m_pTexture->Bind(gfx);
		m_pSampler->Bind(gfx);
		m_pNormalTex->Bind(gfx);
		m_pSpecTex->Bind(gfx);

		DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		DirectX::XMMATRIX worldView = world * gfx.GetCamera();
		Bind::TransformCbuf::Transform transform = {
			DirectX::XMMatrixTranspose(world),
			DirectX::XMMatrixTranspose(worldView),
			DirectX::XMMatrixTranspose(worldView * gfx.GetProjection())
		};
		//设置视图变换buf
		m_pTransform = make_shared<Bind::VertexConstantBuffer<Bind::TransformCbuf::Transform>>(gfx, transform);
		m_pTransform->Bind(gfx);
		
		//设置高光属性buf
		Bind::PixelConstantBuffer<ObjectCBuf>::Resolve(gfx, m_pixelObjectCBuf, 1u)->Bind(gfx);
		gfx.DrawIndex(m_pIdBuffer->GetCount());
	}
}