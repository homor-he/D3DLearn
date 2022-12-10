#ifndef __RENDERGRAPH_H__
#define __RENDERGRAPH_H__

#pragma once

#include <vector>
#include <memory>

using namespace std;

class Graphic;

namespace Bind
{
	class RenderTarget;
	class DepthStencil;
}

namespace Rgph
{
	class Pass;
	class RenderQueuePass;
	class Source;
	class Sink;

	class RenderGraph
	{
	public:
		RenderGraph(Graphic & gfx);
		~RenderGraph();
		void Execute(Graphic & gfx);
		RenderQueuePass & GetRenderQueue(const string & passName);
	public:
		void AppendPass(std::unique_ptr<Pass> pass);
		Pass & FindPassByName(const string & name);
		void SetSinkTarget(const string& sinkName, const string & target);
		void Finalize();
	protected:
		void AddGlobalSource(unique_ptr<Source> source);
		void AddGlobalSink(unique_ptr<Sink> sink);
	protected:
		shared_ptr<Bind::RenderTarget> m_backBufferTarget;
		shared_ptr<Bind::DepthStencil> m_masterDepth;
	protected:
		void LinkSinks(Pass & pass);
		void LinkGlobalSinks();
	protected:
		vector<unique_ptr<Pass>> m_passes;
		vector<unique_ptr<Source>> m_globalSources;
		vector<unique_ptr<Sink>> m_globalSinks;
		bool m_finalized = false;
	};
}

#endif // !__RENDERGRAPH_H__


