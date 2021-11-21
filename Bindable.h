#ifndef __BINDABLE_H__
#define __BINDABLE_H__

#pragma once

#include "GraphicResource.h"

namespace Bind
{
	class Bindable : public GraphicResource
	{
	public:
		virtual void Bind(Graphic& gfx) = 0;
	};
}

#endif // !__BINDABLE_H__
