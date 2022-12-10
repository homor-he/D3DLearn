#include "TextureCase.h"

namespace DrawTest
{
	void TextureCase::Init(Graphic & gfx)
	{
		vector<Vertex> vtVecArr;
		SetVertexBuffer(gfx, vtVecArr);

		vector<unsigned short> idVecArr;
		SetIndexBuffer(gfx, idVecArr);

		SetVertexShaderAndInputLayout(gfx);
		SetPixelShader(gfx);

		string imagePath = "images\\Muro_head_dm.png";
		Bind::Texture tex(gfx, imagePath);
		tex.Bind(gfx);

		Bind::Sampler sampler(gfx, Bind::Sampler::Type::Anisotropic, false, 0);
		sampler.Bind(gfx);

		Bind::Topology topology(gfx, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		topology.Bind(gfx);
	}

	void TextureCase::SetVertexBuffer(Graphic & gfx, vector<Vertex>& vecArr)
	{
		//贴图四个角的纵坐标和屏幕四个角的纵坐标是反的，texcoord的y值要1-？才不会颠倒
		vecArr = {
			{ { -1.0f,-1.0f,0.0f },{ 0.0f,1-0.0f }},  
			{ { 1.0f,-1.0f,0.0f },{ 1.0f,1-0.0f }},
			{ { -1.0f,1.0f,0.05f },{ 0.0f,1-1.0f }},
			{ { 1.0f,1.0f,0.0f },{ 1.0f,1-1.0f }},
		};

		m_vertexArr.swap(vecArr);

		m_vtLayout.Append(VertexRela::VertexLayout::ElementType::Position3D);
		m_vtLayout.Append(VertexRela::VertexLayout::ElementType::Texture2D);
		VertexRela::VertexBuffer vecBuff(m_vtLayout);
		for (int i = 0; i < m_vertexArr.size(); ++i)
		{
			vecBuff.EmplaceBack(m_vertexArr[i].pos, m_vertexArr[i].tex);
		}
		Bind::VertexBuffer vtBindBuff(gfx, vecBuff);
		vtBindBuff.Bind(gfx);
	}

	void TextureCase::SetIndexBuffer(Graphic & gfx, vector<unsigned short>& vecArr)
	{
		vecArr = {
			0,2,1,
			1,2,3,
		};
		m_indexArr.swap(vecArr);
		Bind::IndexBuffer idBuffer(gfx, m_indexArr);
		idBuffer.Bind(gfx);
	}

	void TextureCase::SetVertexShaderAndInputLayout(Graphic & gfx, string fileName)
	{
		Bind::VertexShader vtShader(gfx, fileName);
		vtShader.Bind(gfx);

		Bind::InputLayout inputLayout(gfx, m_vtLayout, vtShader);
		inputLayout.Bind(gfx);
	}

	void TextureCase::SetPixelShader(Graphic & gfx, string fileName)
	{
		Bind::PixelShader pxShader(gfx, fileName);
		pxShader.Bind(gfx);
	}

	size_t TextureCase::GetIndexArrSize()
	{
		return m_indexArr.size();
	}
}


