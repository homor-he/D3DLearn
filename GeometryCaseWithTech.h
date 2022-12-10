#ifndef __GEOMETRYCASEWITHTECH_H__
#define __GEOMETRYCASEWITHTECH_H__

#pragma once

#include "Cuboid.h"
#include "RenderGraph.h"

namespace DrawTest
{
	//实现RenderGraph及相关类的使用
	class GeometryCaseWithTech
	{
	public:
		void Init(Graphic & gfx);
		//void Draw(Graphic & gfx, float dt);
		void LinkTechnique(Rgph::RenderGraph & rg);
		void Submit(size_t channelFilter);
	public:
		shared_ptr<Cuboid> m_cube;
		shared_ptr<Cuboid> m_cube2;
	};
}

#endif // !__GEOMETRYCASEWITHTECH_H__
