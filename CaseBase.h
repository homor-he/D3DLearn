#ifndef __CASEBASE_H__
#define __CASEBASE_H__
#endif // !__CASEBASE_H__

#pragma once

#include "Material.h"
#include "Mesh.h"
#include "BindableCommon.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

class Graphic;

namespace DrawTest
{
	class CaseBase
	{
		virtual void Init(Graphic & gfx) = 0;
		virtual void Draw(Graphic & gfx, float dt)= 0;
	};
}
