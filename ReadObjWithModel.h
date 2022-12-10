#ifndef __READOBJWITHMODEL_H__
#define __READOBJWITHMODEL_H__

#pragma once

#include "CaseBase.h"
#include "Model.h"
#include "RenderGraph.h"

namespace DrawTest
{
	class ReadObjWithModel : public CaseBase
	{
	public:
		void Init(Graphic & gfx) override;
		void Draw(Graphic & gfx, float dt) override;
		void LinkTechnique(Rgph::RenderGraph & rg);
		void Submit(size_t channelFilter);
	private:
		//vector<shared_ptr<Mesh>> m_drawbleList;
		//shared_ptr<Bind::VertexConstantBuffer<Bind::TransformCbuf::Transform>> m_pTransform;
		shared_ptr<Model> m_pModel;
	};
}

#endif // !__READOBJWITHMODEL_H__
