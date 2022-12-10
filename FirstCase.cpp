#include "FirstCase.h"

namespace DrawTest
{
	void FirstTest::Init(Graphic & gfx)
	{
		vector<Vertex> vecVtArr;
		SetVertexBuffer(gfx, vecVtArr);

		vector<unsigned short> vecIdArr;
		SetIndexBuffer(gfx, vecIdArr);

		////设置常量缓存
		//float constBuffMatrix[16] = {
		//	cos(m_angle)*0.5f,		sin(m_angle),		0,		0,
		//	-sin(m_angle)*0.5f,	cos(m_angle),		0,		0,
		//	0,				0,				1,		0,
		//	0,				0,				0,		1
		//};
		//DirectX::XMMATRIX matrix(constBuffMatrix);
		//Bind::VertexConstantBuffer<DirectX::XMMATRIX> vtConstantBuffer(gfx, matrix);
		//vtConstantBuffer.Bind(gfx);

		SetVertexShaderAndInputLayout(gfx);

		SetPixelShader(gfx);

		//设置集合图元形状
		Bind::Topology topology(gfx, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		topology.Bind(gfx);

		//struct Vertex
		//{
		//	typedef struct
		//	{
		//		float x;
		//		float y;
		//	}Pos;
		//	typedef struct
		//	{
		//		unsigned char r;
		//		unsigned char g;
		//		unsigned char b;
		//		unsigned char a;
		//	}Color;
		//	Pos pos;
		//	Color color;
		//};

		//Vertex vertexArr[] = {
		//	
		//	/*{0.0f,0.5f,255,0,0,255},
		//	{0.5f,-0.5f,0,255,0,255},
		//	{-0.5f,-0.5f,0,0,255,255},

		//	{0.0f,0.5f,255,0,0,255},
		//	{ 0.5f,0.0f,0,255,0,255 },
		//	{ 0.5f,-0.5f,0,0,255,255 },

		//	{-0.5f,-0.5f,0,0,255,255},
		//	{ 0.5f,-0.5f,0,255,0,255 },
		//	{ 0.0f,-0.75f,0,255,0,255 },

		//	{ 0.0f,0.5f,255,0,0,255 },
		//	{ -0.5f,-0.5f,0,0,255,255 },
		//	{ -0.5f,0.0f,0,0,255,255 },*/

		//	{ 0.0f,1.0f,255,0,0,255 },
		//	{ 0.5f,-0.5f,0,255,0,255 },
		//	{ -0.5f,-0.5f,0,0,255,255 },
		//	{ 0.5f,0.0f,0,255,0,255 },
		//	{ 0.0f,-0.75f,0,255,0,255 },
		//	{ -0.5f,0.0f,0,0,255,255 },
		//};

		////创建顶点缓存
		//wrl::ComPtr<ID3D11Buffer> vertexBuff;
		//D3D11_BUFFER_DESC vertexBufDesc;
		//vertexBufDesc.ByteWidth = sizeof(vertexArr);
		//vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;;
		//vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//vertexBufDesc.CPUAccessFlags = 0;
		//vertexBufDesc.MiscFlags = 0;
		//vertexBufDesc.StructureByteStride = sizeof(Vertex);
		//D3D11_SUBRESOURCE_DATA vertexInitData;
		//vertexInitData.pSysMem = vertexArr;
		//GFX_THROW_INFO(mDevice->CreateBuffer(&vertexBufDesc, &vertexInitData, &vertexBuff));

		//SHORT indexArr[] =
		//{
		//	0,1,2,
		//	1,0,3,
		//	1,4,2,
		//	0,2,5,
		//};
		////创建索引缓存
		//wrl::ComPtr<ID3D11Buffer> indexBuff;
		//D3D11_BUFFER_DESC indexBufDesc;
		//indexBufDesc.ByteWidth = sizeof(indexArr);
		//indexBufDesc.Usage = D3D11_USAGE_IMMUTABLE;;
		//indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		//indexBufDesc.CPUAccessFlags = 0;
		//indexBufDesc.MiscFlags = 0;
		//indexBufDesc.StructureByteStride = 0;
		//D3D11_SUBRESOURCE_DATA indexInitData;
		//indexInitData.pSysMem = indexArr;
		//GFX_THROW_INFO(mDevice->CreateBuffer(&indexBufDesc, &indexInitData, &indexBuff));
		////设置顶点缓存
		//UINT stride = sizeof(Vertex);
		//UINT offset = 0;
		//mDeviceContext->IASetVertexBuffers(0, 1, vertexBuff.GetAddressOf(), &stride, &offset);
		////设置索引缓存
		//mDeviceContext->IASetIndexBuffer(indexBuff.Get(), DXGI_FORMAT_R16_UINT, 0);

		////设置常量缓存
		//float constBuffMatrix[4][4] = {
		//	cos(angle)*0.5f,		sin(angle),		0,		0,
		//	-sin(angle)*0.5f,	cos(angle),		0,		0,
		//	0,				0,				1,		0,
		//	0,				0,				0,		1
		//};
		//wrl::ComPtr<ID3D11Buffer> constBuffer;
		//D3D11_BUFFER_DESC constBufDesc;
		//constBufDesc.ByteWidth = sizeof(constBuffMatrix);
		//constBufDesc.Usage = D3D11_USAGE_DYNAMIC;;
		//constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		//constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		//constBufDesc.MiscFlags = 0;
		//constBufDesc.StructureByteStride = 0;
		//D3D11_SUBRESOURCE_DATA constInitData;
		//constInitData.pSysMem = constBuffMatrix;
		//GFX_THROW_INFO(mDevice->CreateBuffer(&constBufDesc, &constInitData, &constBuffer));
		//mDeviceContext->VSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());

		////读取顶点着色器
		//wrl::ComPtr<ID3DBlob> vsBlob;
		//GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &vsBlob));
		////创建并设置顶点着色器
		//wrl::ComPtr<ID3D11VertexShader> vertexShader;
		//GFX_THROW_INFO(mDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), 0, &vertexShader));
		//mDeviceContext->VSSetShader(vertexShader.Get(), 0, 0);

		////读取像素着色器
		//wrl::ComPtr<ID3DBlob> psBlob;
		//GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &psBlob));
		////创建并设置像素着色器
		//wrl::ComPtr<ID3D11PixelShader>  pixelShader;
		//mDevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), 0, &pixelShader);
		//mDeviceContext->PSSetShader(pixelShader.Get(), 0, 0);

		////设置inputlayout
		//wrl::ComPtr<ID3D11InputLayout> inputLayout;
		//D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		//{
		//	{ "Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA ,0 },
		//	{ "Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,8,D3D11_INPUT_PER_VERTEX_DATA ,0 },
		//};
		//mDevice->CreateInputLayout(vertexDesc, size(vertexDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
		//mDeviceContext->IASetInputLayout(inputLayout.Get());

		////设置几何图元形状
		//mDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void FirstTest::SetVertexBuffer(Graphic & gfx, vector<Vertex>& vecArr)
	{
		vecArr = {
			{ { 0.0f,1.0f },{1.0f,0.0f,0.0f}},
			{ { 0.5f,-0.5f },{0.0f,1.0f,0.0f}},
			{ { -0.5f,-0.5f },{0.0f,0.0f,1.0f}},
			{ { 0.5f,0.0f },{0.0f,1.0f,0.0f}},
			{ { 0.0f,-0.75f },{0.0f,1.0f,0.0f}},
			{ { -0.5f,0.0f },{0.0f,0.0f,1.0f}},
		};
		m_vertexArr.swap(vecArr);

		m_vtLayout = m_vtLayout.Append(VertexRela::VertexLayout::ElementType::Position2D);
		m_vtLayout = m_vtLayout.Append(VertexRela::VertexLayout::ElementType::Float3Color);
		VertexRela::VertexBuffer vtBuffer(m_vtLayout);
		for (size_t i = 0; i < m_vertexArr.size(); ++i)
		{
			vtBuffer.EmplaceBack(m_vertexArr[i].pos, m_vertexArr[i].color);
		}
		Bind::VertexBuffer vtBindBuffer(gfx, vtBuffer);
		vtBindBuffer.Bind(gfx);
	}

	void FirstTest::SetIndexBuffer(Graphic & gfx, vector<unsigned short>& vecArr)
	{
		vecArr = 
		{
			0,1,2,
			1,0,3,
			1,4,2,
			0,2,5,
		};
		m_indexArr.swap(vecArr);
		Bind::IndexBuffer idBuffer(gfx, m_indexArr);
		idBuffer.Bind(gfx);
	}

	void FirstTest::SetVertexShaderAndInputLayout(Graphic & gfx, string fileName)
	{
		Bind::VertexShader vtShader(gfx, fileName);
		vtShader.Bind(gfx);

		Bind::InputLayout inputLayout(gfx, m_vtLayout, vtShader);
		inputLayout.Bind(gfx);
	}

	void FirstTest::SetPixelShader(Graphic & gfx, string fileName)
	{
		Bind::PixelShader pixelShader(gfx, fileName);
		pixelShader.Bind(gfx);
	}

	size_t FirstTest::GetIndexArrSize()
	{
		return m_indexArr.size();
	}

}


