#ifndef __BINDABLE_H__
#define __BINDABLE_H__

#pragma once

#include "GraphicResource.h"
#include <memory>

class Drawable;

namespace Bind
{
	class Bindable : public GraphicResource
	{
	public:
		virtual void Bind(Graphic& gfx) = 0;
		virtual void InitializeParentRef(Drawable &) {}
	};

	class CloneBindable : public Bindable
	{
	public:
		virtual unique_ptr<CloneBindable> Clone() = 0;
	};
}

#endif // !__BINDABLE_H__
