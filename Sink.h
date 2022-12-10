#ifndef __SINK_H__
#define __SINK_H__

#pragma once

#include <string>
#include "Source.h"

using namespace std;

namespace Rgph
{
	class Sink
	{
	public:
		const string & GetRegisteredName();
		const string & GetPassName();
		const string & GetOutputName();
		void SetTarget(string passName, string outputName);
		virtual void Bind(Source & source) = 0;
		virtual void LinkValidate() = 0;
		virtual ~Sink() = default;
	protected:
		Sink(string registeredName);
	private:
		string m_registeredName;
		string m_passName;
		string m_outputName;
	};

	template<class T>
	class DirectBufferSink : public Sink
	{
		static_assert(is_base_of_v<Bind::BufferResource, T>, "DirectBufferSink target type must ne a BufferResource type");
	public:
		static unique_ptr<Sink> Make(string registeredName, shared_ptr<T> & buffer)
		{
			return make_unique<DirectBufferSink>(move(registeredName), buffer);
		}

		void Bind(Source & source) override
		{
			auto p = std::dynamic_pointer_cast<T>(source.YieldBuffer());
			if (!p)
			{
				OutPutError("%s:%d\nBinding input [%s] to output [%s.%s] {%s} not compatible with {%s}\n",
					__FILE__, __LINE__, GetRegisteredName().c_str(), GetPassName().c_str(), GetOutputName().c_str(),
					typeid(T).name(), typeid(*source.YieldBuffer().get()).name());
			}
			m_target = move(p);
			m_linked = true;
		}

		DirectBufferSink(string registeredName, shared_ptr<T> & buffer):
			Sink(move(registeredName)), m_target(buffer)
		{}

		void LinkValidate()
		{
			if (!m_linked)
			{
				OutPutError("%s:%d\nUnlinked directBufferSink [%s], sourcePassName [%s], sourceName [%s]", GetRegisteredName().c_str(),
					GetPassName().c_str(), GetOutputName().c_str());
			}
		}
	private:
		shared_ptr<T> & m_target;
		bool m_linked = false;
	};


	template<class T>
	class DirectBindableSink : public Sink
	{
		static_assert(is_base_of_v<Bind::Bindable, T>, "DirectBindableSink target type must ne a Bindable type");
	public:
		static unique_ptr<Sink> Make(string registeredName, shared_ptr<T> & bind)
		{
			return make_unique<DirectBindableSink>(move(registeredName), bind);
		}

		DirectBindableSink(string registeredName, shared_ptr<T> & bind):
			Sink(move(registeredName)),m_target(bind)
		{}

		void Bind(Source & source) override
		{
			auto p = dynamic_pointer_cast<T>(source.YieldBindable());
			if (!p)
			{
				OutPutError("%s:%d\nBinding input [%s] to output [%s.%s] {%s} does not match {%s}\n",
					__FILE__, __LINE__, GetRegisteredName().c_str(), GetPassName().c_str(), GetOutputName().c_str(),
					typeid(T).name(), typeid(*source.YieldBindable().get()).name());
			}
			m_target = move(p);
			m_linked = true;
		}

		void LinkValidate()
		{
			if (!m_linked)
			{
				OutPutError("%s:%d\nUnlinked directBindableSink [%s], sourcePassName [%s], sourceName [%s]", GetRegisteredName().c_str(),
					GetPassName().c_str(), GetOutputName().c_str());
			}
		}
	private:
		shared_ptr<T> & m_target;
		bool m_linked = false;
	};

	template<class T>
	class ContainerBindableSink : public Sink
	{
		static_assert(is_base_of_v<Bind::Bindable, T>, "ContainerBindableSink target type must ne a Bindable type");
	public:
		ContainerBindableSink(string name, vector<shared_ptr<Bind::Bindable>> & container, size_t index):
			Sink(move(name)),m_container(container),m_index(index)
		{}

		void Bind(Source & source) override
		{
			auto p = dynamic_pointer_cast<T>(source.YieldBindable());
			if (!p)
			{
				OutPutError("%s:%d\nBinding input [%s] to output [%s.%s] {%s} does not match {%s}\n",
					__FILE__, __LINE__, GetRegisteredName().c_str(), GetPassName().c_str(), GetOutputName().c_str(),
					typeid(T).name(), typeid(*source.YieldBindable().get()).name());
			}
			m_container[m_index] = move(p);
			m_linked = true;
		}

		void LinkValidate()
		{
			if (!m_linked)
			{
				OutPutError("%s:%d\nUnlinked containerBindableSink [%s], sourcePassName [%s], sourceName [%s]", GetRegisteredName().c_str(),
					GetPassName().c_str(), GetOutputName().c_str());
			}
		}
	private:
		vector<shared_ptr<Bind::Bindable>> & m_container;
		size_t m_index;
		bool m_linked = false;
	};
}

#endif // !__SINK_H__
