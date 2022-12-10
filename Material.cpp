#include "Material.h"
#include "BindableCommon.h"
#include "TechChannels.h"
#include "DynamicConstant.h"
#include "ConstantBufferEx.h"

#pragma comment(lib,"assimp-vc140-mt.lib")

Material::Material(Graphic & gfx, aiMaterial & material, path & modelPath):
	m_modelPath(modelPath.string())
{
	string rootPath = modelPath.parent_path().string() + "\\";
	aiString tempName;
	material.Get(AI_MATKEY_NAME, tempName);
	m_name = tempName.C_Str();

	//phong technique & lambertian step 
	{
		Technique phong("Phong", Chan::main);
		{
			Step step("lambertian");
			std::string shaderCode = "Phong";
			aiString texFileName;

			Dcb::RawLayout pixelLayout;
			m_vtLayout.Append(VertexRela::VertexLayout::Position3D);
			m_vtLayout.Append(VertexRela::VertexLayout::Normal);
			//m_vtLayout.Append(VertexRela::VertexLayout::Texture2D);
			//m_vtLayout.Append(VertexRela::VertexLayout::Tangent);
			//m_vtLayout.Append(VertexRela::VertexLayout::Bitangent);
			bool hasTexture = false;
			bool hasGlossAlpha = false;
			bool hasAlpha = false;
			if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
			{
				hasTexture = true;
				shared_ptr<Bind::Texture> tex = Bind::Texture::Resolve(gfx, rootPath + texFileName.C_Str());
				hasAlpha = tex->HasAlpha();
				step.AddBindable(move(tex));

				shaderCode += "Dif";
				m_vtLayout.Append(VertexRela::VertexLayout::Texture2D);
			}
			else
			{
				//没有diffuse贴图时，增加materialColor字段
				pixelLayout.Add<Dcb::Float3>("materialColor");
			}
			step.AddBindable(Bind::Rasterizer::Resolve(gfx, hasAlpha));

			if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
			{
				hasTexture = true;
				shared_ptr<Bind::Texture> tex = Bind::Texture::Resolve(gfx, rootPath + texFileName.C_Str(), 1);
				hasGlossAlpha = tex->HasAlpha();
				step.AddBindable(move(tex));

				shaderCode += "Spc";
				m_vtLayout.Append(VertexRela::VertexLayout::Texture2D);
				pixelLayout.Add<Dcb::Bool>("useGlossAlpha");
				pixelLayout.Add<Dcb::Bool>("useSpecularMap");
			}
			pixelLayout.Add<Dcb::Float3>("specularColor");
			pixelLayout.Add<Dcb::Float>("specularWeight");
			pixelLayout.Add<Dcb::Float>("specularGloss");

			if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
			{
				hasTexture = true;
				step.AddBindable(Bind::Texture::Resolve(gfx, rootPath + texFileName.C_Str(), 2));

				shaderCode += "Nrm";
				m_vtLayout.Append(VertexRela::VertexLayout::Tangent);
				m_vtLayout.Append(VertexRela::VertexLayout::Bitangent);
				pixelLayout.Add<Dcb::Bool>("useNormalMap");
				pixelLayout.Add<Dcb::Float>("normalMapWeight");
			}

			if (hasTexture)
				step.AddBindable(Bind::Sampler::Resolve(gfx));

			shared_ptr<Bind::VertexShader> vtxShader = Bind::VertexShader::Resolve(gfx, shaderCode + "_VS.cso");
			step.AddBindable(Bind::InputLayout::Resolve(gfx, m_vtLayout, *vtxShader));
			step.AddBindable(move(vtxShader));
			step.AddBindable(Bind::PixelShader::Resolve(gfx, shaderCode + "_PS.cso"));
			step.AddBindable(make_shared<Bind::TransformCbuf>(gfx));
			//step.AddBindable(Bind::Stencil::Resolve(gfx, Bind::Stencil::Mode::Off));

			Dcb::Buffer buf(move(pixelLayout));
			auto eleMatColor = buf["materialColor"];
			if (eleMatColor.Exists())
			{
				aiColor3D aiColor = { 0.0f,0.0f,0.0f };
				material.Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
				eleMatColor = reinterpret_cast<DirectX::XMFLOAT3&>(aiColor);
			}
			buf["useGlossAlpha"].SetIfExists(hasGlossAlpha);
			buf["useSpecularMap"].SetIfExists(true);
			auto eleSpcColor = buf["specularColor"];
			if (eleSpcColor.Exists())
			{
				aiColor3D aiColor = { 0.0f,0.0f,0.0f };
				material.Get(AI_MATKEY_COLOR_SPECULAR, aiColor);
				eleSpcColor = reinterpret_cast<DirectX::XMFLOAT3&>(aiColor);
			}
			buf["specularWeight"].SetIfExists(1.0f);
			auto eleSpcGloss = buf["specularGloss"];
			if (eleSpcGloss.Exists())
			{
				float gloss = 0.0f;
				material.Get(AI_MATKEY_SHININESS, gloss);
				eleSpcGloss = gloss;
			}
			buf["useNormalMap"].SetIfExists(true);
			buf["normalMapWeight"].SetIfExists(1.0f);
			auto pixelConstantBuffer = make_shared<Bind::CachingPixelConstantBufferEx>(gfx, buf, 1u);
			//注意:设置buffer,否则调用一次后，由于第一次设置的buffer引用析构, constantbuffer没有更新buffer，渲染会出问题
			pixelConstantBuffer->SetBuffer(buf);
			step.AddBindable(pixelConstantBuffer);

			phong.AddStep(move(step));
		}
		m_techinques.push_back(move(phong));
	}

	//{
	//	Technique outline("Outline", Chan::main, false);
	//	{
	//		Step mask("outlineMask");

	//		mask.AddBindable(Bind::InputLayout::Resolve(gfx, m_vtLayout, *Bind::VertexShader::Resolve(gfx, "SolidVS.cso")));
	//		mask.AddBindable(make_shared<Bind::TransformCbuf>(gfx));

	//		outline.AddStep(move(mask));
	//	}
	//	{
	//		Step draw("outlineDraw");
	//		Dcb::RawLayout colorLayout;
	//		colorLayout.Add<Dcb::Float3>("matColor");
	//		//Dcb::Buffer buf(move(pixelLayout));
	//		Dcb::Buffer colorBuf(move(colorLayout));
	//		colorBuf["matColor"] = DirectX::XMFLOAT3(1.0f, 0.4f, 0.4f);

	//		draw.AddBindable(make_shared<Bind::CachingPixelConstantBufferEx>(gfx, colorBuf, 1u));
	//		draw.AddBindable(Bind::InputLayout::Resolve(gfx, m_vtLayout, *Bind::VertexShader::Resolve(gfx, "SolidVS.cso")));
	//		draw.AddBindable(make_shared<Bind::TransformCbuf>(gfx));

	//		outline.AddStep(move(draw));
	//	}
	//	m_techinques.push_back(move(outline));
 //   }

	{
		Technique shadow("ShadowMap", Chan::shadow);
		Step step("shadowMap");
		step.AddBindable(Bind::InputLayout::Resolve(gfx, m_vtLayout, *Bind::VertexShader::Resolve(gfx, "SolidVS.cso")));
		step.AddBindable(make_shared<Bind::TransformCbuf>(gfx));
		shadow.AddStep(move(step));
		m_techinques.push_back(move(shadow));
	}
}

VertexRela::VertexBuffer Material::ExtractVertices(aiMesh & mesh)
{
	return { m_vtLayout, mesh};
}

vector<unsigned short> Material::ExtractIndices(aiMesh & mesh)
{
	vector<unsigned short> indices;
	indices.reserve(mesh.mNumFaces * 3);
	for (size_t i = 0; i < mesh.mNumFaces; ++i)
	{
		auto face = mesh.mFaces[i];
		if (face.mNumIndices != 3)
			OutPutError("%s:%d\nface index count is not 3",__FILE__,__LINE__);
		indices.emplace_back(face.mIndices[0]);
		indices.emplace_back(face.mIndices[1]);
		indices.emplace_back(face.mIndices[2]);
	}
	return indices;
}

shared_ptr<Bind::VertexBuffer> Material::MakeVertexBindable(Graphic & gfx, aiMesh & mesh, float scale)
{
	VertexRela::VertexBuffer vtxBuffer = ExtractVertices(mesh);
	if (scale != 1.0f)
	{
		size_t vtSize = vtxBuffer.GetSize();
		for (int i = 0; i < vtSize; ++i)
		{
			 auto & vtPos = vtxBuffer[i].Attr<VertexRela::VertexLayout::Position3D>();
			 vtPos.x *= scale;
			 vtPos.y *= scale;
			 vtPos.z *= scale;
		}
	}
	return Bind::VertexBuffer::Resolve(gfx, MakeMaterialTag(mesh), vtxBuffer);
}

shared_ptr<Bind::IndexBuffer> Material::MakeIndexBindable(Graphic & gfx, aiMesh & mesh)
{
	return Bind::IndexBuffer::Resolve(gfx, MakeMaterialTag(mesh), ExtractIndices(mesh));
}

vector<Technique> Material::GetTechniques()
{
	return m_techinques;
}

string Material::MakeMaterialTag(aiMesh & mesh)
{
	return m_modelPath + "%" + mesh.mName.C_Str();
}
