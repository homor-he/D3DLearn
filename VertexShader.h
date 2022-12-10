#ifndef __VERTEXSHADER_H__
#define __VERTEXSHADER_H__

#pragma once

#include "Bindable.h"

namespace Bind
{
	class VertexShader : public Bindable
	{
	public:
		VertexShader(Graphic & gfx, const string & fileName);
		void Bind(Graphic & gfx) override;
		ID3DBlob* GetByteCode();
		static shared_ptr<VertexShader> Resolve(Graphic & gfx, const string & fileName);
		static string GenerateUID(const string & fileName);
		string GetUID();
	protected:
		string m_path;
		Microsoft::WRL::ComPtr<ID3DBlob> m_vsBlob;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	};
}

#endif // !__VERTEXSHADER_H__
