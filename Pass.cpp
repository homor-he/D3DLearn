#include "Pass.h"
#include "Source.h"
#include "Sink.h"

namespace Rgph
{
	Pass::Pass(string name):m_name(name)
	{
	}

	string & Pass::GetName()
	{
		return m_name;
	}

	const vector<unique_ptr<Sink>>& Pass::GetSinks()
	{
		return m_sinks;
	}

	void Pass::SetSinkLinkage(const string & registeName, const string & target)
	{
		vector<string> targetSplit;
		Sink & sink = GetSink(registeName);
		SplitString(target, ".", targetSplit);
		if (targetSplit.size() != 2u)
		{
			OutPutError("%s:%d\nInput target has incorrect format");
			return;
		}
		sink.SetTarget(targetSplit[0], targetSplit[1]);
	}

	Sink & Pass::GetSink(const string & registeName)
	{
		for (auto & sink : m_sinks)
		{
			if (sink->GetRegisteredName() == registeName)
			{
				return *sink;
			}
		}
		OutPutError("%s:%d\nSink registeName[%s] not found in pass[%s]", __FILE__, __LINE__, registeName.c_str(),GetName().c_str());
		throw runtime_error("Sink registeName not found");
	}

	Source & Pass::GetSource(const string & registerName)
	{
		for (auto & source : m_sources)
		{
			if (source->GetName() == registerName)
			{
				return *source;
			}
		}
		OutPutError("%s:%d\nSource registeName[%s] not found in pass[%s]", __FILE__, __LINE__, registerName.c_str(), GetName().c_str());
		throw runtime_error("Source registeName not found");
	}

	bool Pass::FindSource(const string & registerName)
	{
		for (auto & source : m_sources)
		{
			if (source->GetName() == registerName)
			{
				return true;
			}
		}
		return false;
	}

	void Pass::Finalize()
	{
		for (auto & sink : m_sinks)
		{
			sink->LinkValidate();
		}

		for (auto & source : m_sources)
		{
			source->LinkValidate();
		}
	}

	void Pass::Reset()
	{
	}

	Pass::~Pass()
	{
	}

	void Pass::RegisterSink(unique_ptr<Sink> sink)
	{
		for (auto & s : m_sinks)
		{
			if (s->GetRegisteredName() == sink->GetRegisteredName())
			{
				OutPutError("%s:%d\nRegistered input name overlaps, name:%s",
					__FILE__, __LINE__, sink->GetRegisteredName().c_str());
			}
		}
		m_sinks.push_back(move(sink));
	}

	void Pass::RegisterSource(unique_ptr<Source> source)
	{
		for (auto & s : m_sources)
		{
			if (s->GetName() == source->GetName())
			{
				OutPutError("%s:%d\nRegistered output name overlaps, name:%s",
					__FILE__, __LINE__, source->GetName().c_str());
			}
		}
		m_sources.push_back(move(source));
	}
}


