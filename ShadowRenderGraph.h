#ifndef __SHADOWRENDERGRAPH_H__
#define __SHADOWRENDERGRAPH_H__

#pragma once

#include "RenderGraph.h"

class Graphic;
class Camera;

namespace Rgph
{
	class ShadowRenderGraph : public RenderGraph
	{
	public:
		ShadowRenderGraph(Graphic & gfx);
		void BindMainCamera(Camera & cam);
		void BindShadowCamera(Camera & cam);
		void DumpShadowMap(Graphic & gfx, const string & path);
	};
}


#endif // !__SHADOWRENDERGRAPH_H__
