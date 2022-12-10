#ifndef __BUFFERRESOURCE_H__
#define __BUFFERRESOURCE_H__

#pragma once

class Graphic;

namespace Bind
{
	class BufferResource
	{
	public:
		virtual ~BufferResource() = default;
		virtual void BindAsBuffer(Graphic &) = 0;
		virtual void BindAsBuffer(Graphic &, BufferResource *) = 0;
		virtual void Clear(Graphic &) = 0;
	};
}


#endif // !__BUFFERRESOURCE_H__
