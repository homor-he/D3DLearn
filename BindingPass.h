#ifndef __BINDINGPASS_H__
#define __BINDINGPASS_H__

#pragma once

#include "Pass.h"
#include "Sink.h"

namespace Bind
{
	class Bindable;
}

namespace Rgph
{
	class BindingPass : public Pass
	{
	public:
		BindingPass(string name, vector<shared_ptr<Bind::Bindable>> binds = {});
		void AddBind(shared_ptr<Bind::Bindable> bind);
		void BindAll(Graphic & gfx);
	public:
		template<class T>
		void AddBindSink(string name)
		{
			auto index = m_binds.size();
			m_binds.emplace_back();
			RegisterSink(make_unique<ContainerBindableSink<T>>(move(name), m_binds, index));
		}
		shared_ptr<Bind::RenderTarget> m_renderTarget;
		shared_ptr<Bind::DepthStencil> m_depthStencil;
	private:
		void BindBufferResource(Graphic & gfx);
	private:
		vector<shared_ptr<Bind::Bindable>> m_binds;
	};
}

#endif // !__BINDINGPASS_H__
