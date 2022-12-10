#include "VertexShader.h"
#include "GraphicsThrowMacros.h"
#include "d3dcompiler.h"
#include "BindsDic.h"

namespace Bind
{
	VertexShader::VertexShader(Graphic & gfx, const string & fileName) :
		m_path(fileName)
	{
		INFOMAN(gfx);
		//读取顶点着色器
		GFX_THROW_INFO(D3DReadFileToBlob(String2WString(fileName).c_str(), &m_vsBlob));
		//创建顶点着色器
		GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(m_vsBlob->GetBufferPointer(),
			m_vsBlob->GetBufferSize(), nullptr, &m_vertexShader));

	}

	void VertexShader::Bind(Graphic & gfx)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->VSSetShader(m_vertexShader.Get(), nullptr, 0));
	}

	ID3DBlob * VertexShader::GetByteCode()
	{
		return m_vsBlob.Get();
	}

	shared_ptr<VertexShader> VertexShader::Resolve(Graphic & gfx, const string & fileName)
	{
		return BindsDic::Resolve<VertexShader>(gfx, fileName);
	}

	string VertexShader::GenerateUID(const string & fileName)
	{
		return typeid(VertexShader).name() + "#"s + fileName;
	}

	string VertexShader::GetUID()
	{
		return GenerateUID(m_path);
	}
}



