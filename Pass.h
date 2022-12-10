#ifndef __PASS_H__
#define __PASS_H__

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "RenderTarget.h"
#include "DepthStencil.h"

using namespace std;

class Graphic;

namespace Rgph
{
	class Sink;
	class Source;

	class Pass
	{
	public:
		Pass(string name);
		virtual void Execute(Graphic & gfx) = 0;
		virtual void Reset();
		string & GetName();
		const vector<unique_ptr<Sink>> & GetSinks();
		void SetSinkLinkage(const string & registeName, const string & target);
		Sink & GetSink(const string & registeName);
		Source & GetSource(const string & registerName);
		bool FindSource(const string & registerName);
		virtual void Finalize();
		virtual ~Pass();
	public:
		void RegisterSink(unique_ptr<Sink> sink);
		void RegisterSource(unique_ptr<Source> source);
	private:
		vector<unique_ptr<Sink>> m_sinks;
		vector<unique_ptr<Source>> m_sources;
		string m_name;
	};
}

#endif // !__PASS_H__
