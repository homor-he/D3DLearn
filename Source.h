#ifndef __SOURCE_H__
#define __SOURCE_H__

#pragma once

#include "CmnFunc.h"
#include <string>
#include <memory>

using namespace std;

namespace Bind
{
	class Bindable;
	class BufferResource;
}

namespace Rgph
{
	class Source
	{
	public:
		string & GetName();
		virtual shared_ptr<Bind::Bindable> YieldBindable();
		virtual shared_ptr<Bind::BufferResource> YieldBuffer();
		virtual void LinkValidate() = 0;
		virtual ~Source() = default;
	protected:
		Source(string name);
	private:
		string m_name;
	};

	template<class T>
	class DirectBufferSource : public Source
	{
	public:
		static unique_ptr<DirectBufferSource> Make(string name, shared_ptr<T>& buffer)
		{
			return make_unique<DirectBufferSource>(move(name), buffer);
		}

		DirectBufferSource(string name, shared_ptr<T>& buffer) :
			Source(move(name)), m_buffer(buffer)
		{}

		shared_ptr<Bind::BufferResource> YieldBuffer() override
		{
			//if (m_linked)
				//OutPutError("%s:%d\nMutable output bound twice\n",__FILE__,__LINE__);
			m_linked = true;
			return m_buffer;
		}

		void UpdateBuffer(shared_ptr<T>& buffer)
		{
			m_buffer = buffer;
		}

		void LinkValidate() {}
	private:
		shared_ptr<T> & m_buffer;
		bool m_linked = false;
	};

	template<class T>
	class DirectBindableSource : public Source
	{
	public:
		static unique_ptr<DirectBindableSource> Make(string name, shared_ptr<T>& buffer)
		{
			return make_unique<DirectBindableSource>(move(name), buffer);
		}

		DirectBindableSource(string name, shared_ptr<T>& bind):
			Source(move(name)), m_bind(bind)
		{}

		shared_ptr<Bind::Bindable> YieldBindable() override
		{
			return m_bind;
		}

		void LinkValidate() {}
	private:
		shared_ptr<T> & m_bind;
	};
}


#endif // !__SOURCE_H__
