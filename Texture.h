#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#pragma once

#include "Bindable.h"

namespace Bind
{
	class Texture : public Bindable
	{
	public:
		Texture(Graphic & gfx, const string & path, UINT slot = 0);
		static shared_ptr<Texture> Resolve(Graphic & gfx,const string & path, UINT slot = 0);
		static string GenerateUID(const string & path, UINT slot = 0);
		void Bind(Graphic & gfx) override;
		bool HasAlpha();
	protected:
		bool m_hasAlpha;
		string m_path;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_TextureView;
	private:
		unsigned int m_slot;
	};
}

#endif // !__TEXTURE_H__
