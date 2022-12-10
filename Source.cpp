#include "Source.h"

namespace Rgph
{
	string & Source::GetName()
	{
		return m_name;
	}

	shared_ptr<Bind::Bindable> Source::YieldBindable()
	{
		OutPutError("%s:%d\nOutput cannot be accessed as bindable\n", __FILE__, __LINE__);
		return nullptr;
	}

	shared_ptr<Bind::BufferResource> Source::YieldBuffer()
	{
		OutPutError("%s:%d\nOutput cannot be accessed as buffer\n", __FILE__, __LINE__);
		return nullptr;
	}

	Source::Source(string name):
		m_name(move(name))
	{
	}
}