#ifndef __RENDERQUEUEPASS_H__
#define __RENDERQUEUEPASS_H__

#pragma once

#include "BindingPass.h"
#include "Job.h"
#include <vector>

namespace Rgph
{
	class RenderQueuePass : public BindingPass
	{
	public:
		//using BindingPass::BindingPass;
		RenderQueuePass(string name, vector<shared_ptr<Bind::Bindable>> binds = {});
		void Execute(Graphic & gfx) override;
		void Reset() override;
		void Accept(Job job);
	private:
		vector<Job> m_jobs;
	};
}


#endif // !__RENDERQUEUEPASS_H__
