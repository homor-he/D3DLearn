#ifndef __BUFFERCLEARPASS_H__
#define __BUFFERCLEARPASS_H__

#pragma once

#include "Pass.h"
#include "Sink.h"
#include "Source.h"

namespace Bind
{
	class BufferResource;
}

namespace Rgph
{
	class BufferClearPass : public Pass
	{
	public:
		BufferClearPass(string name);
		void Execute(Graphic & gfx) override;
	private:
		shared_ptr<Bind::BufferResource> m_buffer;
	};

	inline BufferClearPass::BufferClearPass(string name) : Pass(name)
	{
		RegisterSink(DirectBufferSink<Bind::BufferResource>::Make("buffer", m_buffer));
		RegisterSource(DirectBufferSource<Bind::BufferResource>::Make("buffer", m_buffer));
	}

	inline void BufferClearPass::Execute(Graphic & gfx) 
	{
		m_buffer->Clear(gfx);
	}
}

#endif // !__BUFFERCLEARPASS_H__
