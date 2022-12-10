#include "Waves.h"
#include "BindableCommon.h"

using namespace DirectX;

//160, 160, 1.0f, 0.03f, 3.25f, 0.4f
Waves::Waves(Graphic & gfx, UINT row, UINT col, float dx, float dt, float speed, float damping)
{
	m_numRow = row;
	m_numCol = col;

	m_vertexCount = row *col;
	m_triangleCount = (row - 1)*(col - 1) * 2;

	m_timeStep = dt;
	m_spatialStep = dx;
	
	float d = damping * dt + 2.0f;
	float e = (speed * speed) * (dt*dt) / (dx*dx);
	m_k1 = (damping * dt - 2.0f) / d;
	m_k2 = (4.0f - 8.0f*e) / d;
	m_k3 = (2.0f*e) / d;

	m_prePosList = new XMFLOAT3[row * col];
	m_currPosList = new XMFLOAT3[row * col];
	//m_normals = new XMFLOAT3[row * col];
	//m_tangentX = new XMFLOAT3[row * col];

	float halfWidth = (col - 1)*dx*0.5f;
	float halfDepth = (row - 1)*dx*0.5f;

	//初始化顶点
	for (UINT i = 0; i < row; ++i)
	{
		float z = halfDepth - i*dx;
		for (UINT j = 0; j < col; ++j)
		{
			float x = -halfWidth + j*dx;

			m_prePosList[i*col + j] = XMFLOAT3(x, 0.0f, z);
			m_currPosList[i*col + j] = XMFLOAT3(x, 0.0f, z);
			//m_normals[i*col + j] = XMFLOAT3(0.0f, 1.0f, 0.0f);
			//m_tangentX[i*col + j] = XMFLOAT3(1.0f, 0.0f, 0.0f);
		}
	}

	//初始化索引
	vector<unsigned short> indices(3 * m_triangleCount);
	int k = 0;
	for (UINT i = 0; i < row - 1; ++i)
	{
		for (UINT j = 0; j < col - 1; ++j)
		{
			indices[k] = i*col + j;
			indices[k + 1] = i*col + j + 1;
			indices[k + 2] = (i + 1)*col + j;

			indices[k + 3] = (i + 1)*col + j;
			indices[k + 4] = i*col + j + 1;
			indices[k + 5] = (i + 1)*col + j + 1;

			k += 6;
		}
	}

	VertexRela::VertexLayout m_vtLayout;
	m_vtLayout.Append(VertexRela::VertexLayout::ElementType::Position3D);
	//m_vtLayout.Append(VertexRela::VertexLayout::ElementType::Normal);
	//m_vtLayout.Append(VertexRela::VertexLayout::ElementType::Tangent);
	m_vtLayout.Append(VertexRela::VertexLayout::ElementType::Float4Color);
	m_vtxBuffer = make_shared<VertexRela::VertexBuffer>(m_vtLayout);
	for (int i = 0; i < m_vertexCount; ++i)
	{
		m_vtxBuffer->EmplaceBack(m_prePosList[i], DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
	}
	string tag = "$Wave" + "r"s + to_string(row) + "c" + to_string(col);
	m_pVtBuffer = Bind::VertexBuffer::Resolve(gfx, tag, *m_vtxBuffer, true);
	m_pIdBuffer = Bind::IndexBuffer::Resolve(gfx, tag, indices);
	m_pTopology = Bind::Topology::Resolve(gfx);
}

void Waves::SetPos(DirectX::XMFLOAT3 pos)
{
	m_pos = pos;
}

DirectX::XMMATRIX Waves::GetTransform()
{
	//表示世界坐标原点(0.0f,0.0f,0.0f),先平移后旋转
	return  DirectX::XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll) *  DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
}

void Waves::SetRotation(float pitch, float yaw, float roll)
{
	m_pitch = pitch;
	m_yaw = yaw;
	m_roll = roll;
}

void Waves::Disturb(UINT i, UINT j, float magnitude)
{
	if (i > 1 && i < m_numRow - 2 && j > 1 && j < m_numCol-2)
	{
		float halfMag = 0.5f* magnitude;

		//在某个点及周边各点加上一个高度值
		m_currPosList[i*m_numCol + j].y += magnitude;
		m_currPosList[i*m_numCol + j + 1].y = halfMag;
		m_currPosList[i*m_numCol + j - 1].y = halfMag;
		m_currPosList[(i + 1)*m_numCol + j].y = halfMag;
		m_currPosList[(i - 1)*m_numCol + j].y = halfMag;
	}
}

void Waves::Update(float dt)
{
	static float t = 0;
	//累计时间
	t += dt;

	if (t >= m_timeStep)
	{
		//不触及边界
		for (UINT i = 1; i < m_numRow - 1; ++i)
		{
			for (UINT j = 1; j < m_numCol - 1; ++j)
			{
				//舍弃之前的buffer,更新后重写到buffer中
				m_prePosList[i * m_numCol + j].y = m_k1 * m_prePosList[i*m_numCol + j].y + m_k2*m_currPosList[i*m_numCol + j].y + 
					m_k3*(m_currPosList[(i+1)*m_numCol+j].y + 
						m_currPosList[(i-1)*m_numCol +j].y + 
						m_currPosList[i*m_numCol +j+1].y +
						m_currPosList[i*m_numCol + j-1].y);

				/*if (m_prePosList[i * m_numCol + j].y > 2.0f)
					m_prePosList[i * m_numCol + j].y = 0.0f;*/
			}
		}
		//交换数组首地址，相当于交换其中的内容
		swap(m_prePosList, m_currPosList);

		t = 0.0f;
	}
}

Microsoft::WRL::ComPtr<ID3D11Buffer> Waves::GetD3DVtxBuffer()
{
	return m_pVtBuffer->GetD3DVtxBuffer();
}

DirectX::XMFLOAT3 Waves::GetCurrVertexIndexPos(DWORD index)
{
	if (index >= m_vertexCount)
	{
		OutPutError("%s:%d\nIndex err", __FILE__, __LINE__);
	}

	return m_currPosList[index];
}

UINT Waves::GetVertextCnt()
{
	return m_vertexCount;
}
