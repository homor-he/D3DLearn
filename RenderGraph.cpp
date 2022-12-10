#include "RenderGraph.h"
#include "RenderQueuePass.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "Source.h"

namespace Rgph
{
	RenderGraph::RenderGraph(Graphic & gfx) :
		m_backBufferTarget(gfx.GetRenderTarget()),
		m_masterDepth(gfx.GetDepthStencil())
	{
		AddGlobalSource(DirectBufferSource<Bind::RenderTarget>::Make("backbuffer", m_backBufferTarget));
		AddGlobalSource(DirectBufferSource<Bind::DepthStencil>::Make("masterDepth", m_masterDepth));
		AddGlobalSink(DirectBufferSink<Bind::RenderTarget>::Make("backbuffer", m_backBufferTarget));
		AddGlobalSink(DirectBufferSink<Bind::DepthStencil>::Make("masterDepth", m_masterDepth));
	}

	RenderGraph::~RenderGraph()
	{
	}

	void RenderGraph::Execute(Graphic & gfx)
	{
		for (auto & p : m_passes)
		{
			p->Execute(gfx);
		}
	}

	RenderQueuePass & RenderGraph::GetRenderQueue(const string & passName)
	{
		try
		{
			for (const auto & pass : m_passes)
			{
				if (pass->GetName() == passName)
				{
					return dynamic_cast<RenderQueuePass&>(*pass);
				}
			}
		}
		catch (bad_cast &)
		{
			OutPutError("%s:%d\nGetRenderQueue, pass is not RenderQueuePass:%s", __FILE__, __LINE__, passName.c_str());
			throw runtime_error("Pass is not RenderQueuePass");
		}
		OutPutError("%s:%d\nGetRenderQueue, pass not found:%s", __FILE__, __LINE__, passName.c_str());
		throw runtime_error("Pass not found");
	}

	void RenderGraph::AppendPass(std::unique_ptr<Pass> pass)
	{
		//assert(!m_finalized);
		for (auto & p : m_passes)
		{
			if (p->GetName() == pass->GetName())
			{
				OutPutError("%s:%d\nPass name already exists:%s\n", __FILE__, __LINE__, pass->GetName());
				return;
			}
		}
		//给pass中的所有sink绑定source, 如果pass中对同一个资源注册了sink和source，那么sink绑定的同时，这个pass的source也绑定了
		LinkSinks(*pass);

		m_passes.push_back(move(pass));
	}

	Pass & RenderGraph::FindPassByName(const string & name)
	{
		for (auto & pass : m_passes)
		{
			if (pass->GetName() == name)
				return *pass;
		}
		throw runtime_error("Failed to find pass name");
	}

	void RenderGraph::SetSinkTarget(const string & sinkName, const string & target)
	{
		bool find = false;
		for (auto & sink : m_globalSinks)
		{
			if (sink->GetRegisteredName() == sinkName)
			{
				find = true;
				vector<string> targetSplitStr;
				SplitString(target, ".", targetSplitStr);
				if (targetSplitStr.size() != 2u)
				{
					OutPutError("%s:%d\nInput target has incorrect format", __FILE__, __LINE__);
					return;
				}
				sink->SetTarget(targetSplitStr[0], targetSplitStr[1]);
				break;
			}
		}

		if (!find)
			OutPutError("%s:%d\nGlobal sink does not exist:%s", __FILE__, __LINE__, sinkName.c_str());
	}

	void RenderGraph::AddGlobalSource(unique_ptr<Source> source)
	{
		m_globalSources.push_back(move(source));
	}

	void RenderGraph::AddGlobalSink(unique_ptr<Sink> sink)
	{
		m_globalSinks.push_back(move(sink));
	}

	void RenderGraph::Finalize()
	{
		for (auto & pass : m_passes)
		{
			pass->Finalize();
		}
		LinkGlobalSinks();
		m_finalized = true;
	}

	void RenderGraph::LinkSinks(Pass & pass)
	{
		for (auto & sink : pass.GetSinks())
		{
			const string& inputSourcePassName = sink->GetPassName();

			if (inputSourcePassName.empty())
			{
				OutPutError("%s:%d\nPass name[%s], sink name[%s] has no targetSource", __FILE__, __LINE__,
					pass.GetName().c_str(), sink->GetRegisteredName().c_str());
				return;
			}

			if (inputSourcePassName == "$")
			{
				bool bound = false;
				for (auto & source : m_globalSources)
				{
					if (source->GetName() == sink->GetOutputName())
					{
						sink->Bind(*source);
						bound = true;
						break;
					}
				}
				if (!bound)
				{
					OutPutError("%s:%d\nSink Output name[%s] not found in globalSources",
						__FILE__,__LINE__, sink->GetOutputName().c_str());
					return;
				}
			}
			else
			{
				bool bound = false;
				for (auto & existingPass : m_passes)
				{
					if (existingPass->GetName() == inputSourcePassName)
					{
						auto & source = existingPass->GetSource(sink->GetOutputName());
						sink->Bind(source);
						bound = true;
						break;
					}
				}
				if (!bound)
				{
					OutPutError("%s:%d\nPass name[%s] not found", __FILE__,__LINE__, inputSourcePassName.c_str());
					return;
				}
			}
		}
	}

	void RenderGraph::LinkGlobalSinks()
	{
		for (auto & sink : m_globalSinks)
		{
			const string & outputPassName = sink->GetPassName();
			for (auto & pass : m_passes)
			{
				if (pass->GetName() == outputPassName)
				{
					auto & source = pass->GetSource(sink->GetOutputName());
					sink->Bind(source);
					break;
				}
			}
		}
	}
}